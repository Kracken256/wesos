cmake_minimum_required(VERSION 3.15)

################################################################################
# VALIDATE BUILD MODE
################################################################################
if(NOT BUILD_MODE)
  message(FATAL_ERROR "BUILD_MODE variable not set.       \
  Please set the BUILD_MODE variable to '--rapid', '--debug', or '--release'")
elseif(NOT BUILD_MODE STREQUAL "--rapid" AND NOT BUILD_MODE STREQUAL "--debug" AND NOT BUILD_MODE STREQUAL "--release")
  message(FATAL_ERROR "Unknown build mode: ${BUILD_MODE}. \
  Valid options are '--rapid', '--debug', '--release'")
endif()

if(BUILD_MODE STREQUAL "--rapid")
  message(STATUS "Rapid build mode selected")
  # TODO: Add rapid build mode specific configurations
elseif(BUILD_MODE STREQUAL "--debug")
  message(STATUS "Debug build mode selected")
  # TODO: Add debug build mode specific configurations
elseif(BUILD_MODE STREQUAL "--release")
  message(STATUS "Release build mode selected")
  # TODO: Add release build mode specific configurations
else()
  message(FATAL_ERROR "Unknown build mode: ${BUILD_MODE}.")
endif()
