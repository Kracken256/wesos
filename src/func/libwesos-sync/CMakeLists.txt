cmake_minimum_required(VERSION 3.15)
project(libwesos-sync LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-sync STATIC ${SOURCE_FILES})
target_include_directories(wesos-sync PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-sync wesos-builtin)
install(TARGETS wesos-sync)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-sync ${TEST_FILES})
target_link_libraries(test-wesos-sync wesos-sync gtest gtest_main)
install(TARGETS test-wesos-sync)
add_dependencies(test-wesos-sync wesos-sync)
