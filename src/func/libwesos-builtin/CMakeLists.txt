cmake_minimum_required(VERSION 3.15)
project(libwesos-builtin LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-builtin STATIC ${SOURCE_FILES})
target_include_directories(wesos-builtin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-builtin wesos-types)
install(TARGETS wesos-builtin)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-builtin ${TEST_FILES})
target_compile_options(test-wesos-builtin PRIVATE -O0)
target_link_libraries(test-wesos-builtin wesos-builtin gtest gtest_main)
install(TARGETS test-wesos-builtin)
add_dependencies(test-wesos-builtin wesos-builtin)
