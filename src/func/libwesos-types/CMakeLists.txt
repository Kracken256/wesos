cmake_minimum_required(VERSION 3.15)
project(libwesos-types LANGUAGES CXX)

file(GLOB_RECURSE INCLUDE_FILES "src/*.hh")
file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-types STATIC ${SOURCE_FILES})
target_include_directories(wesos-types PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
install(TARGETS wesos-types)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-types ${TEST_FILES})
target_link_libraries(test-wesos-types wesos-types gtest gtest_main)
install(TARGETS test-wesos-types)
add_dependencies(test-wesos-types wesos-types)
