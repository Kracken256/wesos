##################################################################################################
# EFI BOOTLOADER APPLICATION
##################################################################################################

set(COMPONENT_NAME wesos-boot-efi)
project(${COMPONENT_NAME})

file(GLOB_RECURSE EFI_BOOT_CXX_SOURCES "src/*.cc")
file(GLOB_RECURSE EFI_BOOT_ASM_SOURCES "src/*.s")

set(WESOS_EFI_BOOT_DEPENDENCIES
  wesos-builtin
  wesos-types
  wesos-assert
  wesos-mem
  wesos-kernconf
)

add_executable(${COMPONENT_NAME} ${EFI_BOOT_CXX_SOURCES} ${EFI_BOOT_ASM_SOURCES})
target_include_directories(${COMPONENT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(${COMPONENT_NAME} PRIVATE -fno-pic --target=x86_64-unknown-windows -fno-stack-protector)
target_link_options(${COMPONENT_NAME} PRIVATE
  -fuse-ld=lld
  -nostdlib
  -Wl,-entry:efi_main
  -Wl,-wx
  -Wl,-subsystem:EFI_APPLICATION

  -target x86_64-unknown-windows
)

foreach(dep ${WESOS_EFI_BOOT_DEPENDENCIES})
  target_link_libraries(${COMPONENT_NAME} PRIVATE ${dep})
  add_dependencies(${COMPONENT_NAME} ${dep})
endforeach()

install(TARGETS ${COMPONENT_NAME})

##################################################################################################
# BUILD ISO IMAGE
##################################################################################################

set(COMPONENT_NAME wesos-boot-iso)
project(${COMPONENT_NAME})

set(WESOS_ISO_FILE "${CMAKE_CURRENT_BINARY_DIR}/WesOS\ Live.iso")

add_custom_target(${COMPONENT_NAME} ALL
  DEPENDS ${WESOS_ISO_FILE}
  VERBATIM
)
add_dependencies(${COMPONENT_NAME} wesos-kernel)

find_program(PARTED parted REQUIRED)
find_program(MKFS_FAT mkfs.fat REQUIRED)
find_program(MKFS_EXT4 mkfs.ext4 REQUIRED)

add_custom_command(
  OUTPUT ${WESOS_ISO_FILE}
  DEPENDS wesos-kernel wesos-boot-efi ${CMAKE_CURRENT_SOURCE_DIR}/create_disk.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/create_disk.sh
)

install(PROGRAMS "${WESOS_ISO_FILE}" DESTINATION bin)

##################################################################################################
# EMULATION
##################################################################################################

include("qemu/all")
