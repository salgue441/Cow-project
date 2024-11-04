#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Usage: $0 <queue_directory> <persons_file>"
  exit 1
fi

queue_dir="$1"
persons_file="$2"
target_dir="../data/divided"
shuffled_persons_file=$(mktemp)

if [ ! -d "$queue_dir" ]; then
  echo "Error: Queue directory does not exist."
  exit 1
fi

if [ ! -f "$persons_file" ]; then
  echo "Error: Persons file does not exist."
  exit 1
fi

if [ ! -d "$target_dir" ]; then
  echo "Error: Target directory /data/divided does not exist."
  exit 1
fi

sed 's/^[0-9 ]*//g' "$persons_file" | grep -v '^$' >"$shuffled_persons_file"
if [ $(wc -l <"$shuffled_persons_file") -ne 10 ]; then
  echo "Error: The persons file must contain exactly 10 names."
  exit 1
fi

shuf "$shuffled_persons_file" -o "$shuffled_persons_file"

readarray -t all_images < <(find "$queue_dir" -maxdepth 1 -type f \( -iname "*.jpg" -o -iname "*.jpeg" -o -iname "*.png" -o -iname "*.gif" \))

total_images=${#all_images[@]}
num_persons=10
images_per_person=$((total_images / num_persons))
remaining_images=$((total_images % num_persons))

echo "Total images in Queue: $total_images"
echo "Number of persons: $num_persons"
echo "Base images per person: $images_per_person"
echo "Remaining images: $remaining_images"

start_index=0
count=0
while IFS= read -r person; do
  mkdir -p "$target_dir/$person"

  end_index=$((start_index + images_per_person))
  for ((i = start_index; i < end_index; i++)); do
    if [ $i -lt ${#all_images[@]} ]; then
      cp "${all_images[i]}" "$target_dir/$person/"
    fi
  done

  start_index=$end_index
  count=$((count + 1))
  echo "Processed person $count: $person (Copied $images_per_person images)"
done <"$shuffled_persons_file"

if [ $remaining_images -gt 0 ]; then
  echo "Distributing remaining $remaining_images images..."
  for ((i = 0; i < remaining_images; i++)); do
    random_person=$(shuf -n 1 "$shuffled_persons_file")
    image_index=$((total_images - remaining_images + i))
    cp "${all_images[image_index]}" "$target_dir/$random_person/"
    echo "Assigned extra image to: $random_person"
  done
fi

rm "$shuffled_persons_file"

echo "Image sorting and copying completed for 10 persons!"
echo "Remaining $remaining_images images were randomly distributed."
