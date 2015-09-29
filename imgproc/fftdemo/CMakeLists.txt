cmake_minimum_required(VERSION 2.8)
project(fftdemo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

include(FindPkgConfig)
find_package(OpenGL REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
find_library(FFTW_LIBRARY NAMES fftw3)
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
                      ${OPENGL_LIBRARY}
                      ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
                      ${FFTW_LIBRARY} m)

add_custom_target(report
                  COMMAND pandoc --output report.pdf ../report.md)
