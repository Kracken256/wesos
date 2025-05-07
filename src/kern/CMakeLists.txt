set(COMPONENT_NAME wesos-kernel)
project(${COMPONENT_NAME} LANGUAGES CXX ASM)

file(GLOB_RECURSE KERNEL_CXX_SOURCES "src/*.cc")
file(GLOB_RECURSE KERNEL_ASM_SOURCES "src/*.s")

set(WESOS_KERN_DEPENDENCIES
  wesos-builtin
  wesos-types
  wesos-cpu
  wesos-sync
  wesos-assert
  wesos-stream
  wesos-mem
  wesos-smartptr
  wesos-lambda
  wesos-kernconf
)

add_executable(${COMPONENT_NAME} ${KERNEL_CXX_SOURCES} ${KERNEL_ASM_SOURCES})
target_include_directories(${COMPONENT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_options(${COMPONENT_NAME} PRIVATE
  -fuse-ld=lld
  -nostdlib
  -static
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
  -Wl,--build-id=none
  -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/kernel.map
  -Wl,--fatal-warnings
)

if(WESOS_TARGET_TRIPLE)
  target_link_options(${COMPONENT_NAME} PRIVATE -target ${WESOS_TARGET_TRIPLE})
endif()

foreach(dep ${WESOS_KERN_DEPENDENCIES})
  target_link_libraries(${COMPONENT_NAME} PRIVATE ${dep})
  add_dependencies(${COMPONENT_NAME} ${dep})
endforeach()

if(WESOS_ADDRESS_SANITIZER)
  target_compile_options(${COMPONENT_NAME} PRIVATE -fsanitize=address)
  target_link_options(${COMPONENT_NAME} PRIVATE -fsanitize=address)
endif()

if(WESOS_UNDEFINED_SANITIZER)
  target_compile_options(${COMPONENT_NAME} PRIVATE -fsanitize=undefined)
  target_link_options(${COMPONENT_NAME} PRIVATE -fsanitize=undefined)
endif()

install(TARGETS ${COMPONENT_NAME})
install(DIRECTORY "include/" DESTINATION "include")
