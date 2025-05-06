set(PROJECT_WIDE_LIBRARIES
  libwesos-builtin
  libwesos-types
  libwesos-cpu
  libwesos-sync
  libwesos-assert
  libwesos-stream
  libwesos-mem
  libwesos-smartptr
  libwesos-lambda
)

foreach(LIB ${PROJECT_WIDE_LIBRARIES})
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/include")
  message(STATUS "Added include directory: ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/include")
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
  message(STATUS "Adding subdirectory: ${LIB}")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${LIB}")
endforeach()
