cmake_minimum_required(VERSION 3.12)
project(ZxEngine VERSION 0.0.1 LANGUAGES CXX ASM)

# Toolchain
set(CMAKE_CXX_COMPILER g++)

# Output Directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Lib/)

# Static Library Output

set(SOURCES
    "Source/AppBase.cpp"
    "Source/Core/Utils.cpp"
    "Source/RHI/Color.cpp"
    "Source/RHI/Vulkan/VkRenderDevice.cpp"
)

add_library(${PROJECT_NAME} ${SOURCES})

# Compile Options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)

# Platform Macros
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ZX_WINDOWS VK_USE_PLATFORM_WIN32_KHR)
endif()

# Libraries and Includes

# Vulkan Required
find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE "Include" "Include/ZxEngine" ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})