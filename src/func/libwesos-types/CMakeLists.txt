cmake_minimum_required(VERSION 3.15)
project(libwesos-types LANGUAGES CXX)

file(GLOB_RECURSE INCLUDE_FILES "*.hh")
file(GLOB_RECURSE SOURCE_FILES "*.cc")

add_library(wesos-types STATIC ${SOURCE_FILES})
install(TARGETS wesos-types)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-types ${TEST_FILES})
target_link_libraries(test-wesos-types wesos-types gtest gtest_main)
install(TARGETS test-wesos-types)
