cmake_minimum_required(VERSION 2.8)
project(OPENGL_TERRAIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenGL REQUIRED)

include_directories("${OPENGL_TERRAIN_SOURCE_DIR}/lib/c11gl/include")
add_subdirectory("lib/c11gl")

file(GLOB SOURCE_FILES ${OPENGL_TERRAIN_SOURCE_DIR}/src/*.cpp)
file(GLOB SHADER_FILES ${OPENGL_TERRAIN_SOURCE_DIR_}/shaders/*)

add_executable(terrain ${SOURCE_FILES})
target_link_libraries(terrain ${OPENGL_LIBRARY} C11GL GLEW glfw)

add_custom_command(
	TARGET terrain
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${OPENGL_TERRAIN_SOURCE_DIR}/shaders/"
		$<TARGET_FILE_DIR:terrain>/shaders/)

add_custom_command(
	TARGET terrain
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${OPENGL_TERRAIN_SOURCE_DIR}/maps/"
		$<TARGET_FILE_DIR:terrain>/maps/)
