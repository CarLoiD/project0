cmake_minimum_required(VERSION 3.12)
project(Demo VERSION 0.0.1 LANGUAGES CXX)

# Toolchain
set(CMAKE_CXX_COMPILER g++)

# Output Directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "Source/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT}/Build/)

# Output
add_executable(${PROJECT_NAME} ${SOURCES})

# Compile Options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)

# Libraries and Includes

target_include_directories(${PROJECT_NAME} PRIVATE "${ROOT}/Engine/Include/" "Source/")
target_link_directories(${PROJECT_NAME} PRIVATE "${ROOT}/Engine/Lib/")

target_link_libraries(${PROJECT_NAME} PRIVATE ZxEngine)

# Run

if(WIN32)
    set(RUN_CMD start cmd.exe /c ${ROOT}/Build/${PROJECT_NAME}.exe)
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${RUN_CMD}
)