#!/bin/bash

mkdir -p ~/nbd_test/mount/
sudo mkfs.ext2 /dev/nbd$1
sudo mount /dev/nbd$1 ~/nbd_test/mount/
