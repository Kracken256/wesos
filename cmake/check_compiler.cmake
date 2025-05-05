set(CLANG_CXX_MINIMUM_VERSION 12)
set(CLANG_C_MINIMUM_VERSION 12)

################################################################################
# ENSURE COMPILER PATHS ARE SET
################################################################################
if(NOT CMAKE_CXX_COMPILER)
  message(FATAL_ERROR "CMake C++ compiler not found.                         \
  Please set the CMAKE_CXX_COMPILER cmake variable to the path to clang++.")
endif()

if(NOT DEFINED CMAKE_C_COMPILER)
  message(FATAL_ERROR "CMake C compiler not found.                           \
  Please set the CMAKE_C_COMPILER cmake variable to the path to clang.")
endif()

message(STATUS "Found C++ compiler at ${CMAKE_CXX_COMPILER}")
message(STATUS "Found C   compiler at ${CMAKE_C_COMPILER}")

################################################################################
# ENSURE COMPILERS ARE SUPPORTED
################################################################################
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(STATUS "Confirmed that the C++ compiler is Clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR "C++ compiler is GNU. Only Clang is supported.         \
  Please set the CMAKE_CXX_COMPILER cmake variable to the path to clang++.")
else()
  message(FATAL_ERROR "C++ compiler is not Clang. Only Clang is supported.   \
  Please set the CMAKE_CXX_COMPILER cmake variable to the path to clang++.")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  message(STATUS "Confirmed that the C compiler is Clang")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR "C compiler is GNU. Only Clang is supported.           \
  Please set the CMAKE_C_COMPILER cmake variable to the path to clang.")
else()
  message(FATAL_ERROR "C compiler is not Clang. Only Clang is supported.     \
  Please set the CMAKE_C_COMPILER cmake variable to the path to clang.")
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${CLANG_CXX_MINIMUM_VERSION})
  message(FATAL_ERROR "C++ compiler version is less than ${CLANG_CXX_MINIMUM_VERSION}. \
  Please set the CMAKE_CXX_COMPILER cmake variable to the path to clang++              \
  version ${CLANG_CXX_MINIMUM_VERSION} or higher.")
endif()

if(CMAKE_C_COMPILER_VERSION VERSION_LESS ${CLANG_C_MINIMUM_VERSION})
  message(FATAL_ERROR "C compiler version is less than ${CLANG_C_MINIMUM_VERSION}. \
  Please set the CMAKE_C_COMPILER cmake variable to the path to clang              \
  version ${CLANG_C_MINIMUM_VERSION} or higher.")
endif()
