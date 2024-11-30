#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e


IMG_FILE="./build/OS-boot.img"


# Create a loop device for the image
LOOP_DEV=$(sudo losetup -f --show -P "$IMG_FILE")
if [ -z "$LOOP_DEV" ]; then
    echo "Error: Failed to set up loop device."
    exit 1
fi
echo "Created loop device: $LOOP_DEV"

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
sudo mount $PART $MNT_DIR
echo "Mounted partition to: $MNT_DIR"

#install bootloader
sudo cp ./limine.conf $MNT_DIR/boot/
echo "Updated Bootloader"

# Clean up
sudo umount $PART
sudo losetup -d "$LOOP_DEV"
echo "Detached loop device: $LOOP_DEV"