cmake_minimum_required(VERSION 3.15)

################################################################################
# LINTING AND REPORTING
################################################################################
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_compile_options(-Wshadow -Wconversion -Wsign-conversion)
add_compile_options(-Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unused-function)
add_compile_options(-Wno-error=deprecated)

################################################################################
# SECURITY
################################################################################
add_compile_options(-fstack-protector) # Ensure stack canaries are present
add_compile_options(-fPIC) # Position Independent Code for ASLR and stuff

################################################################################
# OTHER
################################################################################
add_compile_options(-fno-omit-frame-pointer) # Concepts like "backtrace()" require this.
add_compile_options(-fno-rtti -fno-exceptions -fno-strict-aliasing) # Because I want it to work.
add_compile_options(-nostdlib -fno-asynchronous-unwind-tables -fno-unwind-tables)
add_compile_options(-Wno-unused-command-line-argument)
add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
