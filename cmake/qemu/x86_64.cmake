set(QEMU_MEMORY_MEGABYTES 128M)

if(WESOS_TARGET_TRIPLE MATCHES "^x86_64-")
  find_program(QEMU_SYSTEM qemu-system-x86_64 REQUIRED)

  add_custom_target(boot-qemu-x86_64
    DEPENDS ${WESOS_ISO_FILE}
    COMMAND ${QEMU_SYSTEM}
    -accel tcg,thread=single
    -cpu core2duo
    -m ${QEMU_MEMORY_MEGABYTES}
    -no-reboot
    -drive format=raw,media=cdrom,file=${WESOS_ISO_FILE}
    -serial stdio
    -smp 1
    -usb
    -vga std
    VERBATIM
  )
endif()
