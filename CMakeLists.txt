cmake_minimum_required(VERSION 3.18)
project(
    TagVFS
    VERSION 0.0.0
    )

# put output stuffs in the build folder, top level
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TagVFS_SOURCE_DIR}/build)

# compiler flags for everywhere
SET(CMAKE_CXX_FLAGS "-Wall -Wextra")

add_subdirectory(src)

