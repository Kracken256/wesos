cmake_minimum_required(VERSION 3.15)

set(PROJECT_WIDE_LIBRARIES
  libwesos-builtin
  libwesos-sync
  libwesos-assert
  libwesos-types
  libwesos-lambda
  libwesos-mem
)

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/include")
  message(STATUS "Added include directory: ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/include")
endforeach()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
endif()

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  message(STATUS "Adding subdirectory: ${LIB}")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${LIB}")
endforeach()
