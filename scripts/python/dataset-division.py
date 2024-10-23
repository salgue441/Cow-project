import os
import shutil
import random
from collections import defaultdict
from typing import Dict, List, Tuple, Optional


def stratified_directory_split(
    source_dir: str,
    outputs_dir: Dict[str, str],
    split_ratios: Dict[str, float],
    stratify_by_subdirs: bool = True,
    random_seed: Optional[int] = None,
) -> Dict[str, Dict[str, int]]:
    """
    Split files from a source directory into multiple output directories while
    mantaining stratification.

    Args:
      source_dir: The directory to split.
      outputs_dir: Dictionary with the output directories.
      split_ratios: Dictionary with the split ratios.
      stratify_by_subdirs: If True, the function will stratify the files by
        subdirectories.
      random_seed: Random seed for reproducibility.

    Returns:
      Dictionary containing statistics and the split for each category
    """

    if random_seed is not None:
        random.seed(random_seed)

    if abs(sum(split_ratios.values()) - 1.0) > 1e-9:
        raise ValueError("The split ratios must sum to 1.0")

    for output_dir in outputs_dir.values():
        os.makedirs(output_dir, exist_ok=True)

    print(f"\nScanning source directory: {source_dir}")

    files_by_category = defaultdict(list)
    total_files_found = 0

    for category in sorted(os.listdir(source_dir)):
        category_path = os.path.join(source_dir, category)

        if os.path.isdir(category_path):
            print(f"Processing category: {category}")

            for root, _, files in os.walk(category_path):
                for file in files:
                    file_path = os.path.join(root, file)
                    files_by_category[category].append(file_path)
                    total_files_found += 1

    print(f"\nTotal files found: {total_files_found}")
    print(f"\nFiles by category: ")
    for category, files in files_by_category.items():
        print(f"Category {category}: {len(files)} files")

    split_stats = {split_name: defaultdict(int) for split_name in outputs_dir.keys()}

    # processing each category
    for category, files in files_by_category.items():
        print(f"\nSplitting category {category} with {len(files)} files")
        random.shuffle(files)

        total_files = len(files)
        split_sizes = {}
        cumulative = 0

        for split_name, ratio in split_ratios.items():
            if split_name == list(split_ratios.keys())[-1]:
                split_sizes[split_name] = total_files - cumulative

            else:
                size = int(total_files * ratio)
                split_sizes[split_name] = size
                cumulative += size

        print(f"Split sizes for category {category}: {split_sizes}")

        start_idx = 0
        for split_name, size in split_sizes.items():
            split_files = files[start_idx : start_idx + size]
            output_dir = outputs_dir[split_name]

            for file_path in split_files:
                rel_path = os.path.relpath(file_path, source_dir)
                dest_path = os.path.join(output_dir, rel_path)

                os.makedirs(os.path.dirname(dest_path), exist_ok=True)
                shutil.copy2(file_path, dest_path)
                split_stats[split_name][category] += 1

            start_idx += size

        print("\nFinal split statistics:")
        for split_name, categories in split_stats.items():
            total = sum(categories.values())
            print(f"\n{split_name}:")

            for category, count in categories.items():
                print(f"  Category {category}: {count} files")

            print(f"  Total: {total} files")

        return split_stats


source = "../../data/processed/labeled"
output_dirs = {
    "train": "../../data/processed/split/train",
    "val": "../../data/processed/split/val",
    "test": "../../data/processed/split/test",
}

split_ratios = {
    "train": 0.80,
    "val": 0.10,
    "test": 0.10,
}

print("Starting directory split...")
print(f"Source directory: {os.path.abspath(source)}")
print(f"Output directories: {output_dirs}")
print(f"Split ratios: {split_ratios}")

stats = stratified_directory_split(
    source_dir=source,
    outputs_dir=output_dirs,
    split_ratios=split_ratios,
    random_seed=42,
)
