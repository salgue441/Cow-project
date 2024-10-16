import os
import random
from typing import List, Dict, Tuple
from pathlib import Path
from collections import defaultdict


def stratified_division(
    directory: Path,
    train_ratio: float = 0.8,
    val_ratio: float = 0.10,
    test_ratio: float = 0.10,
) -> Dict[str, List[Path]]:
    """
    Perform stratified division on files in the given directory.

    Args:
      directory (Path): The directory containing the files to divide.
      train_ratio (float): The ratio of the training set.
      val_ratio (float): The ratio of the validation set.
      test_ratio (float): The ratio of the test set.

    Returns:
      Dict[str, List[Path]]: A dictionary containing the file paths for each set.
    """

    if abs(train_ratio + val_ratio + test_ratio - 1.0) > 1e-10:
        raise ValueError("Sum of ratios must be equal to 1.")

    result: Dict[str, List[Path]] = {"train": [], "val": [], "test": []}
    for class_dir in directory.iterdir():
        if not class_dir.is_dir():
            continue

        class_files = list(class_dir.glob("*.jpg"))
        random.shuffle(class_files)

        n_files = len(class_files)
        n_train = int(n_files * train_ratio)
        n_val = int(n_files * val_ratio)
        n_test = n_files - n_train - n_val

        if n_train == 0 or n_val == 0 or n_test == 0:
            raise ValueError("Invalid ratio values.")

        result["train"].extend(class_files[:n_train])
        result["val"].extend(class_files[n_train : n_train + n_val])
        result["test"].extend(class_files[n_train + n_val :])

    return result


def print_class_distribution(split_name: str, files: List[Path]):
    class_count = defaultdict(int)

    for file in files:
        class_name = file.parent.name
        class_count[class_name] += 1

    print(f"\n{split_name.capitalize()} set distribution:")
    for class_name, count in class_count.items():
        print(f"Class {class_name}: {count} files")


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description="Perform stratified division on files in the given directory."
    )

    parser.add_argument(
        "directory", type=Path, help="Path to the directory containing the files."
    )

    parser.add_argument(
        "--train",
        type=float,
        default=0.8,
        help="The ratio of the training set.",
    )

    parser.add_argument(
        "--val",
        type=float,
        default=0.10,
        help="The ratio of the validation set.",
    )

    parser.add_argument(
        "--test",
        type=float,
        default=0.10,
        help="The ratio of the test set.",
    )

    args = parser.parse_args()

    try:
        result = stratified_division(args.directory, args.train, args.val, args.test)

        for split, files in result.items():
            print(f"{split.capitalize()} set: {len(files)} files")
            print_class_distribution(split, files)

    except ValueError as e:
        print(e)

    except FileNotFoundError as e:
        print(e)


if __name__ == "__main__":
    main()
