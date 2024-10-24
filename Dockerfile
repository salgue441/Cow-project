FROM tensorflow/tensorflow:latest-gpu
WORKDIR /app

COPY requirements.txt .
RUN pip install -r requirements.txt
RUN pip install jupyter nbconvert nbformat papermill

COPY model/ model/
COPY src/ src/
COPY notebooks/ notebooks/
COPY converted_notebooks/ converted_notebooks/

COPY saved_model/ saved_model/

RUN jupyter nbconvert --to script notebooks/*.ipynb --output-dir=./src/notebooks/

ENV MODEL_PATH=/app/saved_model
ENV TF_FORCE_GPU_ALLOW_GROWTH=true
ENV PYTHONUNBUFFERED=1

RUN apt-get update && apt-get install -y \
  curl \
  && rm -rf /var/lib/apt/lists/*

HEALTHCHECK --interval=30s --timeout=30s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:8501/v1/models/model || exit 1

EXPOSE 8501 8888
CMD ["python", "src/serve_model.py"]