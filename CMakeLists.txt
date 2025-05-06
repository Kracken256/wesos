cmake_minimum_required(VERSION 3.15)
set(THIS_PROJECT_NAME "wesos")
project(${THIS_PROJECT_NAME})

################################################################################
# USER CONFIGURATION SECTION
################################################################################
option(WESOS_BUILD_TESTING ON)
option(WESOS_BUILD_BENCHMARKING ON)

set(WESOS_TARGET_TRIPLE x86_64-pc-linux-gnu)

################################################################################
# GLOBAL VARIABLE SECTION
################################################################################
set(WESOS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${WESOS_SOURCE_DIR}/cmake")

################################################################################
# BUILD CONFIGURATION ASSERTIONS AND SETUP
################################################################################
include(check_compiler)
include(check_build_mode)
include(configure)

################################################################################
# PERFORM BUILD
################################################################################
add_subdirectory(third_party)
add_subdirectory(src/libs)
add_subdirectory(src/boot)
add_subdirectory(src/kern)
add_subdirectory(src/user)
