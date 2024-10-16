#!/bin/bash

# start spark master
/opt/bitnami/spark/sbin/start-master.sh

get_worker_count() {
  curl -s http://localhost:8080/json/ | jq '.workers | length'
}

check_resources() {
  cpu_usage=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}')
  memory_usage=$(free | grep Mem | awk '{print $3/$2 * 100.0}')

  echo "CPU Usage: $cpu_usage%"
  echo "Memory Usage: $memory_usage%"
}

while true; do
  worker_count=$(get_worker_count)
  echo "Current worker count: $worker_count"

  check_resources

  # Decision logic here

  sleep 60
done
