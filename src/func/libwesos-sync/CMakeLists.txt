cmake_minimum_required(VERSION 3.15)
project(libwesos-sync LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

add_library(wesos-sync STATIC ${SOURCE_FILES})
target_include_directories(wesos-sync PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
install(TARGETS wesos-sync)
install(DIRECTORY "include/" DESTINATION "include")
