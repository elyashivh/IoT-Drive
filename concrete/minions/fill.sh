#!/bin/bash

line=$(printf 'A%.0s' {1..80})  # a single line: 80 A's
lines=$((1024 * 1024 * 8 / 81)) # number of lines: 8MB / (80 chars + newline)

for i in {1..3}; do
    > "minion_${i}.txt" # truncate file
    for ((j = 0; j < lines; ++j)); do
        echo "$line" >> "minion_${i}.txt"
    done
done
