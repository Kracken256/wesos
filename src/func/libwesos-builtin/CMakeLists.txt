cmake_minimum_required(VERSION 3.15)
project(libwesos-builtin LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-builtin STATIC ${SOURCE_FILES})
target_include_directories(wesos-builtin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
install(TARGETS wesos-builtin)
install(DIRECTORY "include/" DESTINATION "include")
