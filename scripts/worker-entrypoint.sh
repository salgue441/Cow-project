#!/bin/bash

register_with_master() {
  echo "Registering with master node..."
}

# Function to check health
check_health() {
  if pgrep -f "org.apache.spark" >/dev/null; then
    echo "Spark worker is healthy"
  else
    echo "Spark worker is not healthy"
  fi
}

register_with_master

/opt/bitnami/spark/sbin/start-worker.sh spark://spark-master:7077

while true; do
  check_health
  sleep 300
done
