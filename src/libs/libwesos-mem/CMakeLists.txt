cmake_minimum_required(VERSION 3.15)
project(libwesos-mem LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

set(WESOS_LIBS_DEPS wesos-builtin wesos-assert wesos-types wesos-sync)

add_library(wesos-mem STATIC ${SOURCE_FILES})
target_include_directories(wesos-mem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-mem ${WESOS_LIBS_DEPS})
install(TARGETS wesos-mem)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-mem ${TEST_FILES})
target_link_libraries(test-wesos-mem wesos-mem gtest gtest_main)
install(TARGETS test-wesos-mem)
add_test(NAME test-wesos-mem COMMAND test-wesos-mem)

file(GLOB_RECURSE BENCHMARK_FILES "bench/*.cc")
add_executable(bench-wesos-mem ${BENCHMARK_FILES})
target_link_libraries(bench-wesos-mem wesos-mem benchmark::benchmark benchmark::benchmark_main)
install(TARGETS bench-wesos-mem)
