cmake_minimum_required(VERSION 3.15)
project(wesos-kernel LANGUAGES CXX ASM)

file(GLOB_RECURSE KERNEL_CXX_SOURCES "src/*.cc")
file(GLOB_RECURSE KERNEL_ASM_SOURCES "src/*.s")

set(WESOS_KERN_DEPENDENCIES
  wesos-types
)

add_executable(wesos-kernel ${KERNEL_CXX_SOURCES} ${KERNEL_ASM_SOURCES})
target_include_directories(wesos-kernel PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_options(wesos-kernel PRIVATE
  -nostdlib
  -static
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
  -Wl,--build-id=none
  -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/kernel.map
  -Wl,--fatal-warnings
  -Wl,--no-warn-rwx-segments
)

foreach(dep ${WESOS_KERN_DEPENDENCIES})
  target_link_libraries(wesos-kernel PRIVATE ${dep})
  add_dependencies(wesos-kernel ${dep})
endforeach()

install(TARGETS wesos-kernel)
install(DIRECTORY "include/" DESTINATION "include")
