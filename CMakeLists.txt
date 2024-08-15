cmake_minimum_required(VERSION 3.5.0)
project(bedside-clock VERSION 0.1.0 LANGUAGES C)

set(EXECUTABLE_OUTPUT_PATH build)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

add_executable(bedside-clock main.c)

