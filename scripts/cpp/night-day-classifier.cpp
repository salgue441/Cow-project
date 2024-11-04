#include <filesystem>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <regex>
#include <queue>
#include <condition_variable>

namespace fs = std::filesystem;

template <typename T>
class ThreadSafeQueue
{
public:
  void push(T value)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(value));
    condition_.notify_one();
  }

  bool try_pop(T &value)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty())
      return false;
    value = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;
  std::condition_variable condition_;
};

struct ImageInfo
{
  fs::path src_path;
  std::string dataset_split;  // "train", "test", or "val"
  std::string classification; // "0", "1", "2", etc.
};

class ImageSorter
{
public:
  ImageSorter(const fs::path &source_root, const fs::path &destination_root)
      : source_root_(source_root), destination_root_(destination_root)
  {
  }

  void sort_images()
  {
    // Scan all dataset splits (train/test/val)
    for (const auto &split_entry : fs::directory_iterator(source_root_))
    {
      if (!split_entry.is_directory())
        continue;
      std::string split_name = split_entry.path().filename().string();

      // Scan classification directories within each split
      for (const auto &entry : fs::recursive_directory_iterator(split_entry.path()))
      {
        if (!entry.is_regular_file())
          continue;

        std::string filename = entry.path().filename().string();
        if (filename.find(".Zone.Identifier") != std::string::npos)
          continue;

        ImageInfo info{
            entry.path(),
            split_name,
            entry.path().parent_path().filename().string()};
        work_queue_.push(info);
      }
    }

    unsigned int thread_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < thread_count; ++i)
      threads.emplace_back(&ImageSorter::worker_thread, this);

    for (auto &thread : threads)
      thread.join();
  }

private:
  const fs::path source_root_;
  const fs::path destination_root_;
  std::mutex console_mutex_;
  ThreadSafeQueue<ImageInfo> work_queue_;

  int extract_hour(const std::string &filename)
  {
    std::regex time_pattern(R"(\d{4}-\d{2}-\d{2}-(\d{2})-\d{2}-\d{2})");
    std::smatch match;

    if (std::regex_search(filename, match, time_pattern) && match.size() > 1)
    {
      try
      {
        return std::stoi(match[1]);
      }
      catch (const std::exception &e)
      {
        return -1;
      }
    }
    return -1;
  }

  bool is_daytime(int hour)
  {
    return hour >= 6 && hour < 18;
  }

  void process_file(const ImageInfo &info)
  {
    try
    {
      int hour = extract_hour(info.src_path.filename().string());
      if (hour == -1)
      {
        std::lock_guard<std::mutex> lock(console_mutex_);
        std::cerr << "Could not extract hour from file "
                  << info.src_path.filename() << std::endl;
        return;
      }

      std::string time_of_day = is_daytime(hour) ? "day" : "night";

      // Create path: dataset_split/time_of_day/classification/filename
      fs::path dest_path = destination_root_ / info.dataset_split / time_of_day /
                           info.classification / info.src_path.filename();

      fs::create_directories(dest_path.parent_path());
      fs::copy(info.src_path, dest_path, fs::copy_options::overwrite_existing);

      {
        std::lock_guard<std::mutex> lock(console_mutex_);
        std::cout << "File " << info.src_path.filename()
                  << " (Hour: " << hour << ") sorted to "
                  << info.dataset_split << "/" << time_of_day
                  << "/" << info.classification << std::endl;
      }
    }
    catch (std::exception &e)
    {
      std::lock_guard<std::mutex> lock(console_mutex_);
      std::cerr << "Error processing file " << info.src_path.filename()
                << ": " << e.what() << std::endl;
    }
  }

  void worker_thread()
  {
    ImageInfo info;
    while (work_queue_.try_pop(info))
    {
      process_file(info);
    }
  }
};

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0]
              << " <source_root> <destination_root>" << std::endl;
    return EXIT_FAILURE;
  }

  try
  {
    fs::path source_dir(argv[1]);
    fs::path destination_dir(argv[2]);

    if (!fs::exists(source_dir))
    {
      std::cerr << "Source directory does not exist." << std::endl;
      return EXIT_FAILURE;
    }

    ImageSorter sorter(source_dir, destination_dir);

    auto start = std::chrono::high_resolution_clock::now();
    sorter.sort_images();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Execution time: " << duration.count() << "ms" << std::endl;
  }
  catch (std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}