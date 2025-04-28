cmake_minimum_required(VERSION 3.15)
project(wesos-kernel LANGUAGES CXX ASM)

file(GLOB_RECURSE KERNEL_CXX_SOURCES "src/*.cc")
file(GLOB_RECURSE KERNEL_ASM_SOURCES "src/*.s")

add_executable(wesos-kernel ${KERNEL_CXX_SOURCES} ${KERNEL_ASM_SOURCES})
target_link_options(wesos-kernel PRIVATE
  -nostdlib
  -static
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
  -Wl,--build-id=none
  -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/kernel.map
  -Wl,--fatal-warnings
)

install(TARGETS wesos-kernel)
install(DIRECTORY "include/" DESTINATION "include")
