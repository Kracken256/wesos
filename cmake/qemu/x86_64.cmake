set(QEMU_MEMORY_MEGABYTES 128M)

if(WESOS_TARGET_TRIPLE MATCHES "^x86_64-")
  find_program(QEMU_SYSTEM qemu-system-x86_64 REQUIRED)

  add_custom_target(boot-qemu-x86_64
    DEPENDS ${WESOS_ISO_FILE}
    COMMAND ${QEMU_SYSTEM}
    -m ${QEMU_MEMORY_MEGABYTES}
    -no-reboot
    -bios /usr/share/qemu/OVMF.fd
    -drive format=raw,media=disk,file=${WESOS_ISO_FILE}
    -serial stdio
    -smp 1
    VERBATIM
  )
endif()
