cmake_minimum_required(VERSION 3.15)
project(libwesos-lambda LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-lambda STATIC ${SOURCE_FILES})
target_include_directories(wesos-lambda PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-lambda wesos-types wesos-builtin wesos-assert wesos-heap)
install(TARGETS wesos-lambda)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-lambda ${TEST_FILES})
target_compile_options(test-wesos-lambda PRIVATE -O0)
target_link_libraries(test-wesos-lambda wesos-lambda gtest gtest_main)
install(TARGETS test-wesos-lambda)
add_dependencies(test-wesos-lambda wesos-lambda)
