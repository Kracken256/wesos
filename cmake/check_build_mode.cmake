cmake_minimum_required(VERSION 3.15)

################################################################################
# VALIDATE BUILD MODE
################################################################################
if(NOT CMAKE_BUILD_TYPE)
  message(FATAL_ERROR "CMAKE_BUILD_TYPE variable not set.       \
  Please set the CMAKE_BUILD_TYPE variable to 'Rapid', 'Debug', or 'Release'")
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Rapid" AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  message(FATAL_ERROR "Unknown build mode: ${CMAKE_BUILD_TYPE}. \
  Valid options are 'Rapid', 'Debug', 'Release'")
endif()

################################################################################
# SET BUILD DEFAULT CONFIGURATION
################################################################################
if(CMAKE_BUILD_TYPE STREQUAL "Rapid")
  message(STATUS "Rapid build mode selected")
  include(clang-global/rapid-mode)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Debug build mode selected")
  include(clang-global/debug-mode)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "Release build mode selected")
  include(clang-global/release-mode)
else()
  message(FATAL_ERROR "Unknown build mode: ${CMAKE_BUILD_TYPE}.")
endif()
