cmake_minimum_required(VERSION 3.12)
project(project0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Platform Macros
if(WIN32)
    add_compile_definitions(ZX_WINDOWS VK_USE_PLATFORM_WIN32_KHR)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)

# Source Files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${sources})

find_package(Vulkan REQUIRED)

include_directories("src/" ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

# Run Target

if(WIN32)
    set(RUN_COMMAND start cmd.exe /k ${PROJECT_NAME}.exe)
endif()

add_custom_target(run
	COMMAND ${RUN_COMMAND}
	DEPENDS ${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/
)
