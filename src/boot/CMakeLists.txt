set(COMPONENT_NAME wesos-boot)
project(${COMPONENT_NAME})

set(WESOS_ISO_FILE "${CMAKE_CURRENT_BINARY_DIR}/WesOS\ Live.iso")
set(WESOS_ISO_LABEL "WesOS Live")
set(WESOS_ISO_PRODUCT_NAME "WesOS")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(WESOS_GRUB_ISO_COMPRESSION gz)
else()
  set(WESOS_GRUB_ISO_COMPRESSION none)
endif()

file(GLOB_RECURSE EVERYTHING_MATTERS "*")

message(${EVERYTHING_MATTERS})

add_custom_target(${COMPONENT_NAME} ALL
  DEPENDS ${WESOS_ISO_FILE}
  VERBATIM
)
add_dependencies(${COMPONENT_NAME} wesos-kernel)

add_custom_command(
  OUTPUT ${WESOS_ISO_FILE}
  DEPENDS wesos-kernel ${EVERYTHING_MATTERS}

  # Make a copy of the ISO template directory for grub2
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/iso ${CMAKE_CURRENT_BINARY_DIR}

  # Copy the wesos-kernel as God.elf
  COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/../kern/wesos-kernel ${CMAKE_CURRENT_BINARY_DIR}/iso/kern/God.elf

  # Remove debug symbols from God.elf
  COMMAND strip ${CMAKE_CURRENT_BINARY_DIR}/iso/kern/God.elf

  # Make bootable Grub image to host God.elf
  COMMAND grub-mkrescue
  --compress=${WESOS_GRUB_ISO_COMPRESSION}
  --product-name=${WESOS_ISO_PRODUCT_NAME}
  -o ${WESOS_ISO_FILE} ${CMAKE_CURRENT_BINARY_DIR}/iso
  -- -volid ${WESOS_ISO_LABEL}
)


install(PROGRAMS "${WESOS_ISO_FILE}" DESTINATION bin)

include("qemu/all")
