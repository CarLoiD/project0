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

# Run Target
# On Windows, start a new cmd process and execute the application

set(RUN_TARGET_DIR ${ROOT}/Build)
set(RUN_TARGET_EXECUTABLE ${RUN_TARGET_DIR}/${CURRENT_TARGET})

if(WIN32)
    set(RUN_COMMAND start cmd.exe /c ${RUN_TARGET_EXECUTABLE})
endif()

# TODO: On Android, push the application to the device and invoke the NativeActivity via ADB
# TODO: On PS2, push the application to the console and invoke the main entrypoint via ngp-dbg

add_custom_target(run
	COMMAND ${RUN_COMMAND}
	DEPENDS ${BUILD_TARGET_EXECUTABLE}
	WORKING_DIRECTORY ${ROOT}
)
