cmake_minimum_required(VERSION 3.18)

# in theory, this is the first line executed by CMake
message("Starting!  Source dir = ${CMAKE_SOURCE_DIR}")

# put output stuffs in the build folder, top level
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# load version info, so we have program name & version data to put into the
# project function
include(scripts/version.cmake)
do_version_cmake_defs()

# declare the project
project(
    ${VERSION_PROG_NAME}
    VERSION ${VERSION_FULL}
    )

# import kconfig'd options for CMake.  the config.h file in src/ will be
# included by the src/CmakeLists.txt
include(scripts/kconfig.cmake)
import_kconfig(CONFIG_ .config)

# included from version.cmake, a bit further up
do_version_header(src/version_autogen.h)

# set standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

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
if (CONFIG_OPT_NONE)
    add_compile_options(-O0)
elseif(CONFIG_OPT_O1)
    add_compile_options(-O1)
elseif(CONFIG_OPT_O2)
    add_compile_options(-O2)
elseif(CONFIG_OPT_O3)
    add_compile_options(-O3)
endif()
if(CONFIG_OPT_OFAST)
    add_compile_options(-Ofast)
endif()
if(CONFIG_OPT_OG)
    add_compile_options(-Og)
endif()
if(CONFIG_OPT_OS)
    add_compile_options(-Os)
endif()

# these flags need to be enabled here -- in order to apply to all compiled
# execs / libs
if(CONFIG_UT_ENABLE_COVERAGE)
    add_compile_options(-fprofile-arcs -ftest-coverage)
    link_libraries(gcov)
endif()

# and off to the sourcecode!
add_subdirectory(src)

if(CONFIG_UT_ENABLE)
    add_subdirectory(test)
endif()

