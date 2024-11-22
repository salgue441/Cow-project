# Deployment Guide

This directory contains everything needed to deploy the cow detection system in production.

## 🚀 Quick Deploy

```bash
docker-compose up -d
```

## 📋 Prerequisites

- Docker installed
- Docker Compose installed
- Camera access (e.g., webcam or IP camera)
- AWS credentials configured

## ⚙️ Configuration

### Environment Variables

Create a `.env` file in this directory with the following environment variables:

```bash
AWS_ACCESS_KEY_ID=your_access_key
AWS_SECRET_ACCESS_KEY=your_secret_key
AWS_REGION=your_region
S3_BUCKET_NAME=your_bucket
```

### Hardware Requirements

- ARM or x86_64 architecture
- Minimum 2GB RAM
- USB camera or IP Camera
- Internet connection

## 🔧 Components

### Docker Configuration

The system uses a custom Docker image based on ARM64 Python:

- Base: `arm64v8/python:3.9-slim`
- Dependencies: `requirements.txt`
- YOLOv9c model pre-loaded

### Service Configuration

The `docker-compose.yml` configures:

- Camera device mapping
- Environment variables
- Log rotation
- Automatic restarts

### Main Script

`script.py` handles:

- Cow detection using YOLOv9c
- AWS S3 uploads
- Error handling and logging

## 📊 Monitoring

- Logs are stored in `./logs`
- JSON format logging enabled
- Log rotation: 3 files, 10 MB each

## 🔍 Troubleshooting

1. Camera access issues

   - Check USB connection
   - Verify device permissions
   - Test with `v4l2-ctl --list-devices`

2. AWS Connection
   - Verify credentials
   - Check network connectivity
   - Confirm bucket permissions
