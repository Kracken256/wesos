cmake_minimum_required(VERSION 3.15)
set(THIS_PROJECT_NAME "wesos")
project(${THIS_PROJECT_NAME})

################################################################################
# USER CONFIGURATION SECTION
################################################################################

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

################################################################################
# PERFORM BUILD
################################################################################
add_subdirectory(src/func)
add_subdirectory(src/boot)
add_subdirectory(src/kern)
add_subdirectory(src/user)
