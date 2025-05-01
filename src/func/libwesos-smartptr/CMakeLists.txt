cmake_minimum_required(VERSION 3.15)
project(libwesos-smartptr LANGUAGES CXX)

file(GLOB_RECURSE INCLUDE_FILES "src/*.hh")
file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-smartptr STATIC ${SOURCE_FILES})
target_include_directories(wesos-smartptr PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-smartptr wesos-builtin wesos-assert wesos-types wesos-sync wesos-mem)
install(TARGETS wesos-smartptr)
install(DIRECTORY "include/" DESTINATION "include")
add_dependencies(wesos-smartptr wesos-builtin wesos-assert wesos-types wesos-sync wesos-mem)

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-smartptr ${TEST_FILES})
target_link_libraries(test-wesos-smartptr wesos-smartptr gtest gtest_main)
install(TARGETS test-wesos-smartptr)
add_dependencies(test-wesos-smartptr wesos-smartptr)

file(GLOB_RECURSE BENCHMARK_FILES "bench/*.cc")
add_executable(bench-wesos-smartptr ${BENCHMARK_FILES})
target_link_libraries(bench-wesos-smartptr wesos-smartptr benchmark::benchmark benchmark::benchmark_main)
install(TARGETS bench-wesos-smartptr)
add_dependencies(bench-wesos-smartptr wesos-smartptr)
