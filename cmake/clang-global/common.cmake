################################################################################
# LINTING AND REPORTING
################################################################################
add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wold-style-cast -Wno-unused-command-line-argument)
add_compile_options(-Wshadow -Wconversion -Wsign-conversion)
add_compile_options(-Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unused-function)
add_compile_options(-Wno-error=deprecated -g3)

################################################################################
# SECURITY
################################################################################
add_compile_options(-fstack-protector) # Ensure stack canaries are present
add_compile_options(-fno-pic)

################################################################################
# OTHER
################################################################################
add_compile_options(-fno-omit-frame-pointer) # Concepts like "backtrace()" require this.
add_compile_options(-fno-rtti -fno-exceptions -fno-strict-aliasing) # Because I want it to work.
add_compile_options(-nostdlib -fno-asynchronous-unwind-tables -fno-unwind-tables)
add_compile_options(-Wl,--gc-sections)
add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden)

if(WESOS_TARGET_TRIPLE)
  add_compile_options(-target ${WESOS_TARGET_TRIPLE})
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
