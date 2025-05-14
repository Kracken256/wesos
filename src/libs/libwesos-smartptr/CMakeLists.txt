project(lib${COMPONENT_NAME} LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

set(WESOS_LIBS_DEPS wesos-builtin wesos-assert wesos-types wesos-sync wesos-mem)

add_library(${COMPONENT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${COMPONENT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${COMPONENT_NAME} ${WESOS_LIBS_DEPS})
install(TARGETS ${COMPONENT_NAME})
install(DIRECTORY "include/" DESTINATION "include")

if(WESOS_BUILD_TESTING)
  file(GLOB_RECURSE TEST_FILES "test/*.cc")
  target_sources(wesos-test PRIVATE ${TEST_FILES})
  target_link_libraries(wesos-test ${COMPONENT_NAME})
endif()

if(WESOS_BUILD_BENCHMARKING)
  file(GLOB_RECURSE BENCHMARK_FILES "bench/*.cc")
  add_executable(bench-${COMPONENT_NAME} ${BENCHMARK_FILES})
  target_link_libraries(bench-${COMPONENT_NAME} ${COMPONENT_NAME} benchmark::benchmark benchmark::benchmark_main)
  install(TARGETS bench-${COMPONENT_NAME})
endif()
