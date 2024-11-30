#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e


IMG_FILE="./build/OS-boot.img"


# Create a fresh file
rm -rf $IMG_FILE
truncate -s 1G $IMG_FILE
echo "Created img file: $IMG_FILE"


# Create a loop device for the image
LOOP_DEV=$(sudo losetup -f --show "$IMG_FILE")
if [ -z "$LOOP_DEV" ]; then
    echo "Error: Failed to set up loop device."
    exit 1
fi
echo "Created loop device: $LOOP_DEV"

# Create the partition table
sudo parted --script "$LOOP_DEV" \
    mklabel msdos \
    mkpart primary ext4 1MiB 100% \
    set 1 boot on
echo "Configed partitions"

# Detach and reattach the loop device with partitions
sudo losetup -d "$LOOP_DEV"
LOOP_DEV=$(sudo losetup -f --show -P "$IMG_FILE") # Use -P to refresh partitions
echo "Reattached loop device with partitions: $LOOP_DEV"

# Check for partition devices
PART="${LOOP_DEV}p1"

if [ ! -e "$PART" ]; then
    echo "Error: Partition was not created properly."
    losetup -d "$LOOP_DEV"
    exit 1
fi

# Format the partitions
echo "Formatting partition as EXT4..."
sudo mkfs.ext4 -q "$PART"


#mount partition
MNT_DIR="./build/mnt"
mkdir -p "$MNT_DIR"
sudo mount -t ext4 $PART $MNT_DIR
echo "Mounted partition to: $MNT_DIR"

#install bootloader
sudo mkdir -p $MNT_DIR/boot
sudo cp /usr/share/limine/limine-bios.sys $MNT_DIR/boot
sudo limine bios-install $LOOP_DEV
sudo cp ./limine.conf $MNT_DIR/boot/
echo "Installed Bootloader"



# Clean up
sudo umount $PART
sudo losetup -d "$LOOP_DEV"
echo "Detached loop device: $LOOP_DEV"