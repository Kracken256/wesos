project(libwesos-builtin LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

set(WESOS_LIBS_DEPS wesos-types)

add_library(wesos-builtin STATIC ${SOURCE_FILES})
target_include_directories(wesos-builtin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(wesos-builtin ${WESOS_LIBS_DEPS})
install(TARGETS wesos-builtin)
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-wesos-builtin ${TEST_FILES})
target_compile_options(test-wesos-builtin PRIVATE -O0)
target_link_libraries(test-wesos-builtin wesos-builtin gtest gtest_main)
install(TARGETS test-wesos-builtin)
add_test(NAME test-wesos-builtin COMMAND test-wesos-builtin)
