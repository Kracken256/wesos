cmake_minimum_required(VERSION 3.15)
project(libwesos-types LANGUAGES CXX)

file(GLOB_RECURSE INCLUDE_FILES "*.hh")
file(GLOB_RECURSE SOURCE_FILES "*.cc")

add_library(wesos-types STATIC ${SOURCE_FILES})

install(TARGETS wesos-types)
install(DIRECTORY "include/" DESTINATION "include")
