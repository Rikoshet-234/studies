cmake_minimum_required(VERSION 2.8)
project(C11GL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenGL REQUIRED)

include_directories(${C11GL_SOURCE_DIR}/include/)

file(GLOB SOURCE_FILES ${C11GL_SOURCE_DIR}/src/*)
add_library(C11GL ${SOURCE_FILES})
