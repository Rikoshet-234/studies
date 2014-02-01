cmake_minimum_required(VERSION 2.8)
project(LISP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${LISP_SOURCE_DIR}/include/)

file(GLOB SOURCE_FILES ${LISP_SOURCE_DIR}/src/*)
add_library(LISP ${SOURCE_FILES})
