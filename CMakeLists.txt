cmake_minimum_required(VERSION 2.6)
project(FluidVisualisation)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x -O3)
endif()

include_directories("src/")
add_subdirectory("src/")

