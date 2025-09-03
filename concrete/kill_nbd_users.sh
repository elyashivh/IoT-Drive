#!/bin/bash

DEVICE="/dev/nbd0"

PIDS=$(sudo lsof -t "$DEVICE")

if [ -z "$PIDS" ]; then
    echo "No processes are using $DEVICE"
    exit 0
fi

echo $PIDS

for pid in $PIDS; do
    echo "Killing PID $pid"
    sudo kill -9 "$pid"
done
