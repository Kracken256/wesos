cmake_minimum_required(VERSION 3.15)
project(libwesos-assert LANGUAGES CXX)

file(GLOB_RECURSE INCLUDE_FILES "src/*.hh")
file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-assert STATIC ${SOURCE_FILES})
target_include_directories(wesos-assert PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
install(TARGETS wesos-assert)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-assert ${TEST_FILES})
target_link_libraries(test-wesos-assert wesos-assert gtest gtest_main)
install(TARGETS test-wesos-assert)
add_dependencies(test-wesos-assert wesos-assert)
