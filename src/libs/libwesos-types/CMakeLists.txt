set(COMPONENT_NAME wesos-types)
project(lib${COMPONENT_NAME} LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

set(WESOS_LIBS_DEPS wesos-assert wesos-builtin)

add_library(${COMPONENT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${COMPONENT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${COMPONENT_NAME} PUBLIC ${WESOS_LIBS_DEPS})
install(TARGETS ${COMPONENT_NAME})
install(DIRECTORY "include/" DESTINATION "include")

file(GLOB_RECURSE TEST_FILES "test/*.cc")
add_executable(test-${COMPONENT_NAME} ${TEST_FILES})
target_link_libraries(test-${COMPONENT_NAME} ${COMPONENT_NAME} gtest gtest_main)
install(TARGETS test-${COMPONENT_NAME})
add_test(NAME test-${COMPONENT_NAME} COMMAND test-${COMPONENT_NAME})
