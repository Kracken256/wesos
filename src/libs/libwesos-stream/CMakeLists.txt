cmake_minimum_required(VERSION 3.15)
project(libwesos-stream LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

set(WESOS_LIBS_DEPS wesos-builtin wesos-types)

add_library(wesos-stream STATIC ${SOURCE_FILES})
target_include_directories(wesos-stream PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-stream ${WESOS_LIBS_DEPS})
install(TARGETS wesos-stream)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-stream ${TEST_FILES})
target_link_libraries(test-wesos-stream wesos-stream gtest gtest_main)
install(TARGETS test-wesos-stream)
add_test(NAME test-wesos-stream COMMAND test-wesos-stream)
