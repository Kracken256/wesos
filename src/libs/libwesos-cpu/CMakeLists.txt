project(libwesos-cpu LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

set(WESOS_LIBS_DEPS wesos-builtin wesos-types)

add_library(wesos-cpu STATIC ${SOURCE_FILES})
target_include_directories(wesos-cpu PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-cpu ${WESOS_LIBS_DEPS})
install(TARGETS wesos-cpu)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-cpu ${TEST_FILES})
target_link_libraries(test-wesos-cpu wesos-cpu gtest gtest_main)
install(TARGETS test-wesos-cpu)
add_test(NAME test-wesos-cpu COMMAND test-wesos-cpu)
