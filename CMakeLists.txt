cmake_minimum_required(VERSION 3.15)
set(THIS_PROJECT_NAME "wesos")
project(${THIS_PROJECT_NAME})

################################################################################
# USER CONFIGURATION SECTION
################################################################################
option(WESOS_BUILD_TESTING "Build WesOS test programs and instrumentation" OFF)
option(WESOS_BUILD_BENCHMARKING "Build WesOS benchmark programs (primarily for the src/libs)" OFF)
option(WESOS_BUILD_LIBS "Build WesOS core libraries (you should do this)" ON)
option(WESOS_BUILD_BOOT "Build WesOS boot loader (you will likely want that too)" ON)
option(WESOS_BUILD_KERN "Build WesOS kernel (good luck)" ON)
option(WESOS_BUILD_USER "Build WesOS userspace (the hard part)" ON)
option(WESOS_ADDRESS_SANITIZER "Enable the -fsanitize=address compiler flag" OFF)
option(WESOS_UNDEFINED_SANITIZER "Enable the -fsanitize=undefined compiler flag" OFF)

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

if(WESOS_BUILD_LIBS)
  add_subdirectory(src/libs)
endif()

if(WESOS_BUILD_BOOT)
  add_subdirectory(src/boot)
endif()

if(WESOS_BUILD_KERN)
  add_subdirectory(src/kern)
endif()

if(WESOS_BUILD_USER)
  add_subdirectory(src/user)
endif()
