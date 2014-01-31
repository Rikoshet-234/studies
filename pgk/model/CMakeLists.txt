cmake_minimum_required(VERSION 2.8)
project(OPENGL_MODEL_LOADING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenGL REQUIRED)

include_directories("${OPENGL_MODEL_LOADING_SOURCE_DIR}/lib/c11gl/include")
add_subdirectory("lib/c11gl")

file(GLOB SOURCE_FILES ${OPENGL_MODEL_LOADING_SOURCE_DIR}/src/*.cpp)
file(GLOB SHADER_FILES ${OPENGL_MODEL_LOADING_SOURCE_DIR_}/shaders/*)

add_executable(model ${SOURCE_FILES})
target_link_libraries(model ${OPENGL_LIBRARY} C11GL SOIL GLEW glfw)

add_custom_command(
	TARGET model
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${OPENGL_MODEL_LOADING_SOURCE_DIR}/shaders/"
		$<TARGET_FILE_DIR:model>/shaders/)

add_custom_command(
	TARGET model
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${OPENGL_MODEL_LOADING_SOURCE_DIR}/models/"
		$<TARGET_FILE_DIR:model>/models/)

add_custom_command(
	TARGET model
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${OPENGL_MODEL_LOADING_SOURCE_DIR}/textures/"
		$<TARGET_FILE_DIR:model>/textures/)
