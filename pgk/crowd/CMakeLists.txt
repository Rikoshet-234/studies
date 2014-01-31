cmake_minimum_required(VERSION 2.8)
project(CROWD_SIMULATION)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenGL REQUIRED)

include_directories("${CROWD_SIMULATION_SOURCE_DIR}/lib/c11gl/include")
add_subdirectory("lib/c11gl")

file(GLOB SOURCE_FILES ${CROWD_SIMULATION_SOURCE_DIR}/src/*.cpp)

add_executable(simulation ${SOURCE_FILES})
target_link_libraries(simulation ${OPENGL_LIBRARY} C11GL glfw)

add_custom_command(
	TARGET simulation
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CROWD_SIMULATION_SOURCE_DIR}/shaders/"
		$<TARGET_FILE_DIR:simulation>/shaders/)
