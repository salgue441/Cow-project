# CAETEC ~ Análisis de Fila de Ordeño de Ganado Vacuno

Sistema automatizado de detección y conteo de vacas para monitoreo de filas de ordeño en granjas lecheras mediante visión por computadora, con capturas cada 5 minutos.

![banner](readme/images/banner.jpg)

## 🎯 Resumen

Este proyecto implementa un sistema automatizado de conteo de vacas utilizando detección de objetos YOLOv8 para monitorear la ocupación de la cola de ordeño en granjas lecheras. El sistema captura imágenes cada 5 minutos, detecta vacas individuales y mantiene registros detallados de ocupación de la cola.

## ⭐ Características Destacadas

- **Monitoreo Automatizado**: Captura de imágenes cada 5 minutos
- **Detección Precisa**: 95% de precisión en el conteo de vacas usando YOLOv8
- **Listo para Producción**: Implementación containerizada con Docker
- **Integración en la Nube**: AWS S3 para almacenamiento de imágenes

## 📁 Estructura del Proyecto

```bash
.
├── CRISP-DM/           # Documentación completa del desarrollo del proyecto
│   └── README.md       # Documentación detallada de la metodología CRISP-DM
├── deployment/         # Configuración de implementación en producción
│   └── README.md       # Guía de implementación y configuración
├── docs/              # Documentación adicional
├── notebooks/         # Notebooks de desarrollo
│   ├── yolo/         # Implementación de YOLOv8 en producción
│   ├── research/     # Experimentos con modelos
│   └── analysis/     # Investigación de datos
└── scripts/          # Scripts de utilidad
```

## 📊 Métricas de Rendimiento

| Métrica                 | Valor          |
| ----------------------- | -------------- |
| Precisión promedio      | 0.91           |
| Precisión de conteo     | 95%            |
| Tiempo de procesamiento | <3s por imagen |
| Intervalo de monitoreo  | 5 minutos      |

## 🚀 Inicio Rápido

1. Clonar el repositorio
2. Seguir la guía de implementación en `deployment/README.md`
3. Para detalles de desarrollo, ver `CRISP-DM/README.md`

## 🔬 Stack Tecnológico

- **Framework CV**: YOLOv9c para detecció de objetos
- **Implementación**: Contenedores Docker
- **Almacenamiento**: AWS S3

## 📫 Contacto

Para preguntas o soporte, por favor abra un issue en el repositorio.
