# Guía de Implementación

Este directorio contiene todo lo necesario para implementar el sistema de detección de vacas en producción.

## 🚀 Implementación Rápida

```bash
docker-compose up -d
```

## 📋 Prerequisitos

- Docker y Docker Compose instalados
- Acceso a cámara (webcam o cámara IP)
- Credenciales AWS configuradas

## ⚙️ Configuración

### Variables de Entorno

Crear un archivo `.env` con las siguientes variables:

```bash
AWS_ACCESS_KEY_ID=tu_access_key
AWS_SECRET_ACCESS_KEY=tu_secret_key
AWS_REGION=tu_region
S3_BUCKET_NAME=tu_bucket
```

### Requerimientos de Hardware

- Mínimo 2GB RAM
- Cámara USB o cámara IP
- Arquitectura ARM64 (ej. Raspberry Pi 4)

## 🔧 Componentes

### Configuración Docker

El sistema utiliza una imagen Docker personalizada basada en ARM64 Python:

- Base: `arm64v8/python:3.9-slim`
- Dependencias de OpenCV incluidas
- Modelo YOLOv8 precargado

### Configuración del Servicio

El `docker-compose.yml` configura:

- Mapeo de dispositivos de cámara
- Variables de entorno
- Rotación de logs
- Reinicio automáticos

### Script Principal

`script.py` maneja:

- Detección de vacas usando YOLOv9c
- Carga a AWS S3
- Manejo de errores y logging

## 📊 Monitoreo

- Los logs se almacenan en ./logs
- Logging en formato JSON habilitado
- Rotación de logs: 3 archivos, 10MB cada uno

## 🔍 Solución de Problemas

1. Problemas de acceso a cámara

   - Verificar conexión USB
   - Verificar permisos de dispotivo
   - Probar con `v4l2-ctl --list-devices`

2. Conexión AWS

   - Verificar credenciales
   - Comprobar conectividad de red
   - Confirmar permisos del bucket
