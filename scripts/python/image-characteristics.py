import csv
import os
from datetime import datetime

import numpy as np
from PIL import Image


def extract_characteristics(directory_path, output_filename):
    with open(output_filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(
            [
                "Nombre",
                "Clase",
                "Fecha_Captura",
                "Brillo_Medio",
                "Contraste",
                "Saturacion_Media",
                "Tono_Medio",
            ]
        )

        for _class in os.listdir(directory_path):
            class_path = os.path.join(directory_path, _class)

            if os.path.isdir(class_path):
                for image_name in os.listdir(class_path):
                    if image_name.lower.endswith(".jpg"):
                        image_path = os.path.join(class_path, image_name)

                        with Image.open(image_path) as img:
                            img_array = np.array(img)

                            brightness = np.mean(img_array)
                            contrast = np.std(img_array)

                            img_hsv = img.convert("HSV")
                            hsv_array = np.array(img_hsv)

                            saturation = np.mean(hsv_array[:, :, 1])
                            hue = np.mean(hsv_array[:, :, 0])

                            writer.writerow(
                                [
                                    image_name,
                                    _class,
                                    datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                                    brightness,
                                    contrast,
                                    saturation,
                                    hue,
                                ]
                            )


extract_characteristics("../../data/processed/labeled", "salguero.csv")
