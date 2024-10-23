#include <algorithm>
#include <concepts>
#include <filesystem>
#include <iostream>
#include <map>
#include <random>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <future>
#include <mutex>
#include <execution>
#include <thread>
#include <memory>

namespace fs = std::filesystem;

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

/**
 * @brief Split statistics for the stratified directory splitter
 *
 * Contains the statistics for the split
 */
struct SplitStats
{
  std::unordered_map<
      std::string, std::unordered_map<std::string, std::size_t>>
      stats;

  std::unique_ptr<std::mutex> stats_mutex;

  SplitStats() : stats_mutex(std::make_unique<std::mutex>()) {}

  // Deleted constructions
  SplitStats(const SplitStats &) = delete;
  SplitStats &operator=(const SplitStats &) = delete;

  SplitStats(SplitStats &&other) noexcept
      : stats(std::move(other.stats)),
        stats_mutex(std::move(other.stats_mutex)) {}

  // Operator overload
  SplitStats &operator=(SplitStats &&other) noexcept
  {
    if (this != &other)
    {
      stats = std::move(other.stats);
      stats_mutex = std::move(other.stats_mutex);
    }
    return *this;
  }

  void add_file(const std::string &split_name, const std::string &category)
  {
    std::lock_guard<std::mutex> lock(*stats_mutex);
    stats[split_name][category]++;
  }

  void print() const
  {
    std::lock_guard<std::mutex> lock(*stats_mutex);
    std::cout << "\nFinal split statistics:\n";
    std::size_t total_processed = 0;

    for (const auto &[split_name, categories] : stats)
    {
      std::size_t split_total = 0;
      std::cout << "Split: " << split_name << "\n";

      for (const auto &[category, count] : categories)
      {
        std::cout << "Category: " << category << " - " << count << "\n";
        split_total += count;
      }

      std::cout << "Total: " << split_total << "\n";
      total_processed += split_total;
    }

    std::cout << "Total processed: " << total_processed << "\n";
  }
};

/**
 * @brief Stratified directory splitter
 *
 * Splits a directory into multiple directories based on a stratified split
 */
class StratifiedDirectorySplitter
{
public:
  struct Config
  {
    fs::path source_dir;
    std::unordered_map<std::string, fs::path> output_dirs;
    std::unordered_map<std::string, double> split_ratios;
    std::optional<unsigned int> random_seed;
    size_t num_threads = std::thread::hardware_concurrency();
  };

private:
  Config config;
  std::mt19937 rng;
  SplitStats stats;
  std::unordered_map<std::string, std::vector<fs::path>> files_by_category;
  mutable std::mutex files_mutex;

  void validate_config()
  {
    if (!fs::exists(config.source_dir))
    {
      throw std::runtime_error("Source directory does not exist");
    }

    double total_ratio = 0.0;
    for (const auto &[_, ratio] : config.split_ratios)
    {
      total_ratio += ratio;
    }

    if (std::abs(total_ratio - 1.0) > 1e-9)
    {
      throw std::runtime_error("Split ratios must sum to 1.0");
    }
  }

  void collect_files()
  {
    std::cout << "\nScanning source directory: " << config.source_dir << "\n";
    std::vector<std::future<std::pair<std::string, std::vector<fs::path>>>> futures;

    std::vector<fs::path> categories;
    for (const auto &entry : fs::directory_iterator(config.source_dir))
    {
      if (entry.is_directory())
      {
        categories.push_back(entry.path());
      }
    }

    for (const auto &category_path : categories)
    {
      futures.push_back(std::async(std::launch::async, [this, category_path]()
                                   {
                std::string category = category_path.filename().string();
                std::vector<fs::path> category_files;
                
                for (const auto &file : fs::recursive_directory_iterator(category_path)) {
                    if (file.is_regular_file()) {
                        category_files.push_back(file.path());
                    }
                }

                std::cout << "Found " << category_files.size() << " files in category " << category << "\n";
                return std::make_pair(category, std::move(category_files)); }));
    }

    for (auto &future : futures)
    {
      auto [category, files] = future.get();
      std::lock_guard<std::mutex> lock(files_mutex);
      files_by_category[category] = std::move(files);
    }

    if (files_by_category.empty())
    {
      throw std::runtime_error("No categories found in source directory");
    }
  }

  void create_output_directories()
  {
    std::cout << "\nCreating output directories structure...\n";
    for (const auto &[split_name, output_path] : config.output_dirs)
    {
      std::cout << "Creating base directory: " << output_path << std::endl;
      fs::create_directories(output_path);
    }

    for (const auto &category_entry : fs::directory_iterator(config.source_dir))
    {
      if (category_entry.is_directory())
      {
        std::string category = category_entry.path().filename().string();
        for (const auto &[split_name, output_path] : config.output_dirs)
        {
          fs::path category_dir = output_path / category;
          fs::create_directories(category_dir);
        }
      }
    }

    bool all_created = true;
    for (const auto &[split_name, output_path] : config.output_dirs)
    {
      if (!fs::exists(output_path))
      {
        std::cerr << "Failed to create directory: " << output_path << std::endl;
        all_created = false;
        continue;
      }

      for (const auto &category_entry : fs::directory_iterator(config.source_dir))
      {
        if (category_entry.is_directory())
        {
          fs::path category_dir = output_path / category_entry.path().filename();
          if (!fs::exists(category_dir))
          {
            std::cerr << "Failed to create directory: " << category_dir << std::endl;
            all_created = false;
          }
        }
      }
    }

    if (!all_created)
    {
      throw std::runtime_error("Failed to create all required directories");
    }
  }

  void process_category(const std::string &category, std::vector<fs::path> &files)
  {
    if (files.empty())
    {
      return;
    }

    std::cout << "Processing category: " << category << " (" << files.size() << " files)\n";

    {
      std::lock_guard<std::mutex> lock(files_mutex);
      std::ranges::shuffle(files, rng);
    }

    const size_t total_files = files.size();
    size_t current_position = 0;

    std::string last_split_name;
    for (const auto &[split_name, _] : config.split_ratios)
    {
      last_split_name = split_name;
    }

    std::vector<std::future<void>> copy_futures;

    for (const auto &[split_name, ratio] : config.split_ratios)
    {
      size_t split_size;
      if (split_name == last_split_name)
        split_size = total_files - current_position;

      else
        split_size = static_cast<size_t>(total_files * ratio);

      std::vector<fs::path> batch_files;
      for (size_t i = 0; i < split_size && (current_position + i) < total_files; ++i)
      {
        batch_files.push_back(files[current_position + i]);
      }

      copy_futures.push_back(std::async(std::launch::async, [this, batch_files, split_name, category]()
                                        {
                for (const auto &source_file : batch_files) {
                    const fs::path rel_path = fs::relative(source_file, config.source_dir);
                    const fs::path dest_path = config.output_dirs[split_name] / rel_path;

                    fs::create_directories(dest_path.parent_path());
                    fs::copy_file(source_file, dest_path, fs::copy_options::overwrite_existing);
                    stats.add_file(split_name, category);
                } }));

      current_position += split_size;
    }

    for (auto &future : copy_futures)
      future.wait();
  }

  void process_splits()
  {
    std::vector<std::future<void>> category_futures;

    for (auto &[category, files] : files_by_category)
    {
      category_futures.push_back(std::async(std::launch::async, [this, &category, &files]()
                                            { process_category(category, files); }));
    }

    for (auto &future : category_futures)
      future.wait();
  }

public:
  explicit StratifiedDirectorySplitter(Config cfg) : config(std::move(cfg))
  {
    if (config.random_seed)
      rng.seed(*config.random_seed);

    else
      rng.seed(std::random_device{}());

    validate_config();
  }

  SplitStats split()
  {
    collect_files();
    create_output_directories();
    process_splits();
    return std::move(stats);
  }
};

int main()
{
  try
  {
    StratifiedDirectorySplitter::Config config{
        .source_dir = "../../data/processed/labeled",
        .output_dirs = {
            {"train", "../../data/processed/split/train"},
            {"val", "../../data/processed/split/val"},
            {"test", "../../data/processed/split/test"}},
        .split_ratios = {{"train", 0.75}, {"val", 0.15}, {"test", 0.10}},
        .random_seed = 42,
        .num_threads = std::thread::hardware_concurrency()};

    std::cout << "Starting parallel directory split using "
              << config.num_threads << " threads...\n";

    std::cout << "Source directory: " << fs::absolute(config.source_dir).string() << "\n";

    StratifiedDirectorySplitter splitter(std::move(config));
    SplitStats stats = splitter.split();
    stats.print();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}