# CAETEC ~ Cow Queue Analysis

Automated cow detection and counting system for dairy farm queue monitoring using computer vision, with snapshots taken at 5-minute intervals.

![banner](readme/images/banner.jpg)

## 🎯 Overview

This project implements an automated cow counting system using YOLOv8 object detection to monitor milking queue occupancy in dairy farms. The system:

- Processes images captured every 5 minutes by a camera installed at the milking queue.
- Detects individual cows using bounding boxes.
- Mantains a time-series log of queue occupancy.
- Provides insights into queue patterns throughout the day.

## ⭐ Highlights

- **Automated Monitoring**: captures queue images every 5 minutes.
- **Precise Detection**: 95% accuracy in cow counting using YOLOv8.

## 📊 Key Metrics

| Metric               | Value         |
| -------------------- | ------------- |
| Average precision    | 0.91          |
| Queue Count Accuracy | 95%           |
| Processing time      | <3s per image |
| Monitoring interval  | 5 minutes     |

## 🔬 Technical Stack

- **CV Framework**: YOLOv8 for object detection
- **Deployment**: Docker containerized
- **Storage**: AWS S3 for image storage

## 📁 Project Overview

```{bash}
docs/
├── README.md          # Project Overview
├── CRISP-DM/
│   ├── Business Understanding/
│   ├── Data Understanding/
│   ├── Data Preparation I/
│   ├── Modeling I/
│   ├── Data Preparation II/
│   ├── Modeling II/
│   ├── Modeling III/
│   ├── Modeling IV/
│   ├── Data Preparation III/
│   ├── Modeling V/
│   ├── Evaluation/
│   └── Deployment/
deploy/
├── Dockerfile         # Docker Configuration
├── requirements.txt   # Python Dependencies
notebooks/
├── yolo/              # 🌟 Production YOLOv8 Implementation
├── research/          # Model Experiments
└── analysis/          # Data Investigation
```

## 📚 Documentation

- Installation guide
- Development guide
- Model training
- System architecture

## Competencias 

Para cualquier link dentro de los pdf sera requerido descargar el pdf para acceder a ese link

| Tipo | Competencia | Evidencia |
|------|-------------|-----------|
| **Modelo** | Selecciona un modelo adecuado al problema presentado en el reto | Sección 3 de [[10]Modeling V](CRISP-DM/[10]%20Modeling%20V.pdf) |
| **Modelo** | Emplea arquitecturas del estado de arte deep learning como parte de la solución del reto | Empleamos varias arquitecturas para solucionar el reto [[[04]Modeling I](CRISP-DM/[04]%20Modeling%20I.pdf) [[06]Modeling II](CRISP-DM/[06]%20Modeling%20II.pdf) [[07]Modeling III](CRISP-DM/[07]%20Modeling%20III.pdf) [[08]Modeling IV](CRISP-DM/[08]%20Modeling%20IV.pdf)[[10]Modeling V](CRISP-DM/[10]%20Modeling%20V.pdf)|
| **Modelo** | Elabora un modelo benchmark (regresión lineal, logística o un modelo arima) para la evaluación de la solución y explica detalladamente la forma en la que se genera el modelo | Sección 4 de [[10]Modeling V](CRISP-DM/[10]%20Modeling%20V.pdf)|
| **Refinamiento** | Refina el modelo después de su primera iteración para mejorar su desempeño | Sección 7 de [[10]Modeling V](CRISP-DM/[10]%20Modeling%20V.pdf) |
| **Refinamiento** | Prueba con otras arquitecturas (además del benchmark) para ver si obtiene mejores rendimientos | Se probaron con multiples arquitecturas. [[04]Modeling I](CRISP-DM/[04]%20Modeling%20I.pdf) [[06]Modeling II](CRISP-DM/[06]%20Modeling%20II.pdf) [[07]Modeling III](CRISP-DM/[07]%20Modeling%20III.pdf) [[08]Modeling IV](CRISP-DM/[08]%20Modeling%20IV.pdf) Y despues del benchmark se probo con otra arquitectura para ver si obteniamos mejores rendimientos el cual esta en la sección 7 [[10]Modeling V](CRISP-DM/[10]%20Modeling%20V.pdf)|

