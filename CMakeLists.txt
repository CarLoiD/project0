cmake_minimum_required(VERSION 3.12)
project(project0 VERSION 0.0.1 LANGUAGES CXX ASM)

# Include CMakeModules
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ROOT}/CMakeModules")

# C++ Standard Required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Targets and Subdirectories

set(CURRENT_TARGET Demo)

add_subdirectory(Engine)
add_subdirectory(Projects/${CURRENT_TARGET})
