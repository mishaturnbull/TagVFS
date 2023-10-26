# expose header files
include_directories(.)

# conditionally compile the files needed for compression algos
set(COMP_ALGO_SOURCES )
list(APPEND COMP_ALGO_SOURCES
    tvcompression.c
    compression_table.c
    )

# wrap the appending of source files to that list in an if block with the
# Kconfig item that enables or disables those algorithms
if(CONFIG_TVCALG_NOCOMPRESSION OR CONFIG_TVCALG_TABLETEST)
    list(APPEND COMP_ALGO_SOURCES comp_passthru.c)
endif()

# define compression functions library
add_library(tvcompression ${COMP_ALGO_SOURCES})

# define table dump target
add_executable(tvctool
    tvcomp_exe.c
)
target_link_libraries(tvctool tvcompression)
target_link_libraries(tvctool tvmisc)

