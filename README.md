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

Aquí te preparo una tabla en formato Markdown para tu README que organiza las competencias por tipo y permite agregar enlaces a los entregables:

| Tipo | Competencia | Evidencia |
|------|-------------|-----------|
| **Modelo** | Selecciona un modelo adecuado al problema presentado en el reto | [[10]Modeling V](CRISP-DM) |
| **Modelo** | Emplea arquitecturas del estado de arte deep learning como parte de la solución del reto | |
| **Modelo** | Elabora un modelo benchmark (regresión lineal, logística o un modelo arima) para la evaluación de la solución y explica detalladamente la forma en la que se genera el modelo | |
| **Evaluación** | Plantea pruebas del estado del arte respaldadas en la literatura, que sean válidas y relevantes en el contexto del reto | |
| **Evaluación** | Selecciona métricas (o indicadores de desempeño) adecuadas tanto para el modelo como para el reto | |
| **Evaluación** | Interpreta los resultados del modelo de manera correcta | |
| **Refinamiento** | Refina el modelo después de su primera iteración para mejorar su desempeño | |
| **Refinamiento** | Prueba con otras arquitecturas (además del benchmark) para ver si obtiene mejores rendimientos | |

