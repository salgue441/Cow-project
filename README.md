# CAETEC ~ Cow Queue Analysis

Automated cow detection and counting system for dairy farm queue monitoring using computer vision, with snapshots taken at 5-minute intervals.

![banner](readme/images/banner.jpg)

## 🎯 Overview

This project implements an automated cow counting system using YOLOv8 object detection to monitor milking queue occupancy in dairy farms. The system captures images every 5 minutes, detects individual cows, and maintains detailed queue occupancy logs.

## ⭐ Highlights

- **Automated Monitoring**: captures queue images every 5 minutes.
- **Precise Detection**: 95% accuracy in cow counting using YOLOv8.
- **Production Ready**: containerized deployment with <3s processing time.
- **Cloud Integration**: AWS S3 for image storage

## 📁 Project Overview

```{bash}
.
├── CRISP-DM/            # Complete project development documentation
│   └── README.md        # Detailed CRISP-DM methodology documentation
├── deployment/          # Production deployment configuration
│   └── README.md        # Deployment guide and configuration
├── docs/                # Additional documentation
├── notebooks/           # Development notebooks
│   ├── yolo/            # Production YOLOv8 implementation
│   ├── research/        # Model experiments
│   └── analysis/        # Data investigation and analysis
└── scripts/             # Utility scripts
```

## 🚀 Quick Start

1. Clone the repository
2. Follow the deployment guide in `deployment/README.md`
3. For development details, see `CRISP-DM/README.md`

## 📫 Contact

For questions or support, please open an issue in the repository.
