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
if (CONFIG_WARN_SHUTUP)
    add_compile_options(-fsyntax-only)
elseif(CONFIG_WARN_NONE)
    add_compile_options(-w)
elseif(CONFIG_WARN_NORMAL)
    # nothing to do in this one!
elseif(CONFIG_WARN_WALL)
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
    # set compiler debug flag globally
    add_compile_options(-g)
    # provide the BREAKPOINT; macro
    add_compile_definitions(BREAKPOINT=asm\("int $3"\))
else()
    # make sure BREAKPOINT;'s don't do anything, but also don't issue a
    # useless-statement warning
    add_compile_definitions(BREAKPOINT=)
endif()

# and off to the sourcecode!
add_subdirectory(src)

