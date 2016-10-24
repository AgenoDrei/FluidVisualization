cmake_minimum_required(VERSION 2.6)
project(FluidVisualisation)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

include_directories("src/")
add_subdirectory("src/")

