#!/bin/sh

ISO_NAME="WesOS Live.iso"

# Create zeroed 32MB raw image
# WARNING: The EFI File System type depends on the size of the image file.
dd if=/dev/zero of=drive.hdd bs=1k count=32000 status=none
if [ $? -ne 0 ]; then
  echo "Failed to create image file"
  exit 1
fi

# Create a loop device
DEVICE_PATH=$(losetup -f --show drive.hdd)
if [ $? -ne 0 ]; then
  echo "Failed to create loop device"
  rm -f drive.hdd
  exit 1
fi

# Create a GPT partition table
parted -s $DEVICE_PATH mklabel gpt
if [ $? -ne 0 ]; then
  echo "Failed to create partition table"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Create a EFI FAT16 partition
parted -s $DEVICE_PATH mkpart primary fat16 1MiB 100%
if [ $? -ne 0 ]; then
  echo "Failed to create FAT16 partition"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set the partition name to "EFI"
parted -s $DEVICE_PATH name 1 "EFI"
if [ $? -ne 0 ]; then
  echo "Failed to set partition name to EFI"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set the partition type to "EFI System"
parted -s $DEVICE_PATH set 1 esp on
if [ $? -ne 0 ]; then
  echo "Failed to set partition type to EFI System"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Format the partition with FAT16
mkfs.fat -F16 ${DEVICE_PATH}p1 >/dev/null
if [ $? -ne 0 ]; then
  echo "Failed to format partition with FAT16"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Create a mount point
TEMP_MOUNT_POINT=$(mktemp -d)
if [ $? -ne 0 ]; then
  echo "Failed to create mount point"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Mount the partition
mount ${DEVICE_PATH}p1 $TEMP_MOUNT_POINT
if [ $? -ne 0 ]; then
  echo "Failed to mount partition"
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Setup the EFI directory structure
mkdir -p $TEMP_MOUNT_POINT/EFI/BOOT
if [ $? -ne 0 ]; then
  echo "Failed to create EFI directory structure"
  umount $TEMP_MOUNT_POINT
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Copy the bootloader to the EFI directory
cp wesos-boot-efi $TEMP_MOUNT_POINT/EFI/BOOT/BOOTx64.EFI
if [ $? -ne 0 ]; then
  echo "Failed to copy bootloader to EFI directory"
  umount $TEMP_MOUNT_POINT
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Unmount the temporary partition
umount $TEMP_MOUNT_POINT
if [ $? -ne 0 ]; then
  echo "Failed to unmount partition"
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

rmdir $TEMP_MOUNT_POINT

# Detach the loop device
losetup -d $DEVICE_PATH
if [ $? -ne 0 ]; then
  echo "Failed to detach loop device"
  rm -f drive.hdd
  exit 1
fi

mv drive.hdd "$ISO_NAME"

chmod 644 "$ISO_NAME"

echo "✅ UEFI-bootable ISO created: $ISO_NAME"
