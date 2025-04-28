cmake_minimum_required(VERSION 3.15)
project(libhello-world LANGUAGES CXX)

add_library(hello-world SHARED
  hello-world.cc
)
install(TARGETS hello-world)
