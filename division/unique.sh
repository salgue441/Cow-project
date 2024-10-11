#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <divided_directory>"
  exit 1
fi

divided_dir="$1"

if [ ! -d "$divided_dir" ]; then
  echo "Error: Divided directory does not exist."
  exit 1
fi

calculate_checksum() {
  md5sum "$1" | awk '{print $1}'
}

declare -A dir_checksums
for person_dir in "$divided_dir"/*; do
  if [ -d "$person_dir" ]; then
    person=$(basename "$person_dir")
    dir_checksums["$person"]=""

    for image in "$person_dir"/*; do
      if [ -f "$image" ]; then
        checksum=$(calculate_checksum "$image")
        dir_checksums["$person"]+="$checksum "
      fi
    done
  fi
done

for person1 in "${!dir_checksums[@]}"; do
  for person2 in "${!dir_checksums[@]}"; do
    if [ "$person1" != "$person2" ]; then
      common_images=$(comm -12 <(echo ${dir_checksums["$person1"]} | tr ' ' '\n' | sort) <(echo ${dir_checksums["$person2"]} | tr ' ' '\n' | sort) | wc -l)

      if [ $common_images -gt 0 ]; then
        echo "Found $common_images common images between $person1 and $person2"

        for image in "$divided_dir/$person1"/*; do
          if [ -f "$image" ]; then
            checksum=$(calculate_checksum "$image")
            if echo ${dir_checksums["$person2"]} | grep -q "$checksum"; then
              echo "  Common image: $(basename "$image")"
              echo "    In $person1: $image"
              echo "    In $person2: $(find "$divided_dir/$person2" -type f | xargs md5sum | grep "$checksum" | awk '{print $2}')"
            fi
          fi
        done
      fi
    fi
  done
done

echo "Comparison complete."
