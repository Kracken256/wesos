#!/bin/sh

ISO_NAME="WesOS Live.iso"

# WARNING: The EFI File System type depends on the size of the image file.
dd if=/dev/zero of=drive.hdd bs=1000 count=256000 status=none
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

###########################################################################################
# START: CREATE EFI FAT12 PARTITION
###########################################################################################

# Create a FAT12 EFI partition 5 MiB in size
parted -s $DEVICE_PATH mkpart primary 1MiB 6MiB
if [ $? -ne 0 ]; then
  echo "Failed to create FAT12 partition"
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

# Format the partition with FAT12
mkfs.fat -F12 -n "EFI" ${DEVICE_PATH}p1 >/dev/null
if [ $? -ne 0 ]; then
  echo "Failed to format partition with FAT12"
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
mkdir -p $TEMP_MOUNT_POINT/EFI/BOOT $TEMP_MOUNT_POINT/WESOS
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

# Copy the kernel to the EFI directory
cp ../kern/wesos-kernel $TEMP_MOUNT_POINT/WESOS/PROTO.ELF
if [ $? -ne 0 ]; then
  echo "Failed to copy kernel to EFI directory"
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

###########################################################################################
# END: CREATE EFI FAT12 PARTITION
###########################################################################################

###########################################################################################
# START: CREATE KERNEL MODULE PARTITION
###########################################################################################

# Create an FAT32 partition 64 MiB in size
parted -s $DEVICE_PATH mkpart primary 6MiB 70MiB
if [ $? -ne 0 ]; then
  echo "Failed to create FAT32 partition"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set the partition name to "MODULE"
parted -s $DEVICE_PATH name 2 "MODULE"
if [ $? -ne 0 ]; then
  echo "Failed to set partition name to MODULE"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set the partition type to basic data
parted -s $DEVICE_PATH set 2 msftdata on
if [ $? -ne 0 ]; then
  echo "Failed to set partition type to basic data"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Format the partition with FAT32
mkfs.fat -F32 -n "MODULE" ${DEVICE_PATH}p2 >/dev/null
if [ $? -ne 0 ]; then
  echo "Failed to format partition with FAT32"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Create a mount point for the FAT32 partition
TEMP_MOUNT_POINT=$(mktemp -d)
if [ $? -ne 0 ]; then
  echo "Failed to create mount point for FAT32 partition"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Mount the FAT32 partition
mount ${DEVICE_PATH}p2 $TEMP_MOUNT_POINT
if [ $? -ne 0 ]; then
  echo "Failed to mount FAT32 partition"
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set up the MODULE directory structure
mkdir -p $TEMP_MOUNT_POINT/modules
if [ $? -ne 0 ]; then
  echo "Failed to create MODULE directory structure"
  umount $TEMP_MOUNT_POINT
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Unmount the FAT32 partition
umount $TEMP_MOUNT_POINT
if [ $? -ne 0 ]; then
  echo "Failed to unmount FAT32 partition"
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

rmdir $TEMP_MOUNT_POINT

###########################################################################################
# END: CREATE KERNEL MODULE PARTITION
###########################################################################################

###########################################################################################
# START: CREATE APPLICATION PARTITION
###########################################################################################

# Create an EXT4 partition 1 GiB in size
parted -s $DEVICE_PATH mkpart primary 70MiB 100%
if [ $? -ne 0 ]; then
  echo "Failed to create EXT4 partition"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set the partition name to "APPLICATION"
parted -s $DEVICE_PATH name 3 "APPLICATION"
if [ $? -ne 0 ]; then
  echo "Failed to set partition name to APPLICATION"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set the partition type to basic data
parted -s $DEVICE_PATH set 3 msftdata on
if [ $? -ne 0 ]; then
  echo "Failed to set partition type to basic data"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Format the partition with EXT4
mkfs.ext4 -L "APPLICATION" ${DEVICE_PATH}p3 >/dev/null
if [ $? -ne 0 ]; then
  echo "Failed to format partition with EXT4"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Create a mount point for the EXT4 partition
TEMP_MOUNT_POINT=$(mktemp -d)
if [ $? -ne 0 ]; then
  echo "Failed to create mount point for EXT4 partition"
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Mount the EXT4 partition
mount ${DEVICE_PATH}p3 $TEMP_MOUNT_POINT
if [ $? -ne 0 ]; then
  echo "Failed to mount EXT4 partition"
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Set up the APPLICATION directory structure
mkdir -p $TEMP_MOUNT_POINT/applications
if [ $? -ne 0 ]; then
  echo "Failed to create APPLICATION directory structure"
  umount $TEMP_MOUNT_POINT
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

# Unmount the EXT4 partition
umount $TEMP_MOUNT_POINT
if [ $? -ne 0 ]; then
  echo "Failed to unmount EXT4 partition"
  rmdir $TEMP_MOUNT_POINT
  losetup -d $DEVICE_PATH
  rm -f drive.hdd
  exit 1
fi

rmdir $TEMP_MOUNT_POINT

###########################################################################################
# END: CREATE APPLICATION PARTITION
###########################################################################################

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
