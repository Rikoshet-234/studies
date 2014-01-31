cmake_minimum_required(VERSION 2.8)
project(Mastermind)

find_package(OpenGL REQUIRED)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++11")

add_executable(mastermind
	src/Mastermind.cpp
	src/Renderer.cpp
	src/Board.cpp)
target_link_libraries(mastermind ${OPENGL_LIBRARY} glfw)
