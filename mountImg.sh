#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

IMG_FILE="./build/OS-boot.img"

LOOP_DEV=$(sudo losetup -f --show -P "$IMG_FILE") # Use -P to refresh partitions

# Check for partition devices
PART="${LOOP_DEV}p1"

if [ ! -e "$PART" ]; then
    echo "Error: Partition was not created properly."
    losetup -d "$LOOP_DEV"
    exit 1
fi

#mount partition
MNT_DIR="./build/mnt"
mkdir -p "$MNT_DIR"
sudo mount -t ext4 $PART $MNT_DIR
echo "Mounted partition to: $MNT_DIR"