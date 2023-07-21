cmake_minimum_required(VERSION 3.18)

project(
    TagVFS
    VERSION 0.0.0
    )

message("Starting.  TagVFS_SOURCE_DIR = ${TagVFS_SOURCE_DIR}")

# put output stuffs in the build folder, top level
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TagVFS_SOURCE_DIR}/build)

# import kconfig'd options for CMake.  the config.h file in src/ will be
# included by the src/CmakeLists.txt
include(scripts/kconfig.cmake)
import_kconfig(CONFIG_ .config)

# compiler flags for everywhere
if(CONFIG_WARN_WALL)
    add_compile_options(-Wall)
elseif(CONFIG_WARN_WEXTRA)
    add_compile_options(-Wall -Wextra)
elseif(CONFIG_WARN_PEDANTIC)
    add_compile_options(-Wall -Wextra -pedantic)
elseif(CONFIG_WARN_WERROR)
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()
if(CONFIG_DEBUG_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE on)
endif()
if(CONFIG_DEBUG)
    add_compile_options(-g)
endif()

# and off to the sourcecode!
add_subdirectory(src)

