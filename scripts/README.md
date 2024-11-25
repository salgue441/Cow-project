# Dataset Management and Classification Toolkit 📁

Welcome to the **Dataset Management and Classification Toolkit**!.
This directory provides tools for efficiently managing datasets, extracting
metadata, and performing manual labelling for the dataset. It's designed with
performance and modularity in mind, featuring multithreaded C++ scripts,
Python data analysis, and shell scripts for dataset organization.

## 📁 Project Structure

```bash
├── cpp/                           # High-performance C++ utilities
│   ├── dataset-division.cpp       # Divide dataset into train, test, and validation
│   ├── image_sorter               # Tool for sorting images (details TBD)
│   └── night-day-classifier.cpp   # Classify dataset into day and night categories
├── python/                        # Python scripts for analysis
│   └── image-characteristics.py   # Extract metadata for further analysis (e.g., Tableau)
├── shell/                         # Shell scripts for dataset preprocessing
    ├── division.sh                # Split raw dataset into subsets by persons
    ├── persons.txt                # List of persons for splitting
    └── unique.sh                  # Separate dataset for unique individuals
```

## ⚡ Features

### C++ Scripts (multithreaded)

- `dataset-division.cpp` splits the dataset into train, validation, and test subsets for model training.
- `night-day-classifier.cpp` classifies images into day and night while retaining the original dataset structure.

### Python scripts

- `image-characteristics.py` extracts image metadata for visualization and analysis in tools like Tableau, PowerBi, etc.

### Shell scripts

- `division.sh`: organize raw datasets by splitting images into subsets specified in persons.txt for manual labeling.
- `unique.sh`: verifies that the images are unique for each created directory.
- `persons.txt`: a customizable list of individuals for dataset division.

## 🔧 Requirements

### General

- C++ Compiler supporting multithreaded (GCC, Clang)
- Python version 3.10 with standard libraries
- Shell environment: unix-like environment (Linux / MacOS)

### Python libraries

Ensure Python scripts have dependencies installed:

```bash
pip install -r requirements.txt
```

## 🚀 Usage

### C++

1. Compile

```bash
g++ -std=c++17 dataset-division.cpp -o dataset-division
g++ -std=c++17 night-day-classifier.cpp -o image_sorter -pthread -static-libstdc++ -static-libgcc
```

2. Run:

```bash
./dataset-division
./night-day-classifier
```

### Python

Run the python script directly

```bash
python3 image-characteristics.py
```

### Shell

1. Ensure scripts have execution permissions

```bash
chmod +x division.sh unique.sh
```

2. Execute with input

```bash
./division.sh
./unique.sh
```

## 📌 Notes

- All Cpp scripts leverage multithreaded for improved performance.
- Ensure `persons.txt` file is correctly populated before using shell scripts.
- Python metadata extraction outputs are stored in a .csv
