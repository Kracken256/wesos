set(PROJECT_WIDE_LIBRARIES
  wesos-builtin
  wesos-types
  wesos-cpu
  wesos-sync
  wesos-assert
  wesos-stream
  wesos-mem
  wesos-smartptr
  wesos-lambda
  wesos-kernconf
)

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib${LIB}/include")
  message(STATUS "Added include directory: ${CMAKE_CURRENT_SOURCE_DIR}/lib${LIB}/include")
endforeach()

if(WESOS_ADDRESS_SANITIZER)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

if(WESOS_UNDEFINED_SANITIZER)
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
endif()

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  message(STATUS "Adding subdirectory: lib${LIB}")
  set(COMPONENT_NAME "${LIB}")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib${LIB}")
endforeach()
