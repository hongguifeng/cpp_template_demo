cmake_minimum_required(VERSION 3.10)
project(cpp_template_demo CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all example .cpp files
file(GLOB EXAMPLE_SOURCES "examples/*.cpp")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
endforeach()

# C++17 examples (need special handling)
file(GLOB CPP17_SOURCES "examples/cpp17/*.cpp")
foreach(EXAMPLE_SOURCE ${CPP17_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    set_target_properties(${EXAMPLE_NAME} PROPERTIES CXX_STANDARD 17)
endforeach()
