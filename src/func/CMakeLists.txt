cmake_minimum_required(VERSION 3.15)

set(PROJECT_WIDE_LIBRARIES
  libwesos-assert
  libwesos-types
  libhello-world
)

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/include")
  message(STATUS "Added include directory: ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/include")
endforeach()

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  message(STATUS "Adding subdirectory: ${LIB}")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${LIB}")
endforeach()
