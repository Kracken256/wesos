if(WESOS_COVERAGE)
  add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
  add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif()

if(WESOS_BUILD_TESTING)
  enable_testing()

  add_executable(wesos-test)
  target_link_libraries(wesos-test gtest gtest_main)
  target_compile_options(wesos-test PRIVATE -O0)
  install(TARGETS wesos-test)
  add_test(NAME wesos-test COMMAND wesos-test)

  if(WESOS_ADDRESS_SANITIZER)
    target_compile_options(wesos-test PRIVATE -fsanitize=address)
    target_link_options(wesos-test PRIVATE -fsanitize=address)
  endif()

  if(WESOS_UNDEFINED_SANITIZER)
    target_compile_options(wesos-test PRIVATE -fsanitize=undefined)
    target_link_options(wesos-test PRIVATE -fsanitize=undefined)
  endif()
endif()

if(WESOS_COVERAGE)
  add_custom_target(wesos-coverage ALL
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/coverage/wesos.dat"
    VERBATIM
  )

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/coverage/wesos.dat"
    DEPENDS wesos-test
    COMMAND echo "Generating coverage report..."
    COMMAND wesos-test
    COMMAND mkdir -p "${CMAKE_CURRENT_SOURCE_DIR}/coverage"
    COMMAND llvm-profdata-18 merge -sparse "${CMAKE_CURRENT_BINARY_DIR}/default.profraw" -o default.profdata
    COMMAND llvm-cov-18 export "${CMAKE_CURRENT_BINARY_DIR}/wesos-test" --format=lcov -instr-profile=default.profdata > "${CMAKE_CURRENT_SOURCE_DIR}/coverage/wesos.dat"
    VERBATIM
  )
  add_dependencies(wesos-coverage wesos-test)
endif()
