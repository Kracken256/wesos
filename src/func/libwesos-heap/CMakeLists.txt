cmake_minimum_required(VERSION 3.15)
project(libwesos-heap LANGUAGES CXX)

file(GLOB_RECURSE INCLUDE_FILES "src/*.hh")
file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-heap STATIC ${SOURCE_FILES})
target_include_directories(wesos-heap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-heap wesos-builtin wesos-assert wesos-types wesos-sync wesos-lambda)
install(TARGETS wesos-heap)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-heap ${TEST_FILES})
target_link_libraries(test-wesos-heap wesos-heap gtest gtest_main)
install(TARGETS test-wesos-heap)
add_dependencies(test-wesos-heap wesos-heap)
