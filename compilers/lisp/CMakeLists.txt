cmake_minimum_required(VERSION 2.8)
project(LISP_REPL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory("lib/lisp")

include_directories(${LISP_SOURCE_DIR}/include/)
include_directories(${LISP_REPL_SOURCE_DIR}/include/)

file(GLOB SOURCE_FILES ${LISP_REPL_SOURCE_DIR}/src/*)

add_executable(repl ${SOURCE_FILES})
target_link_libraries(repl LISP)
