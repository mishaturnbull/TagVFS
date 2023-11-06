include_directories(../../src/)
include_directories(${CONFIG_CRITERION_INCLUDE_PATH})

FILE(GLOB_RECURSE ut_sources *.ut.c)

add_executable(unittest ${ut_sources})
target_link_libraries(unittest
    criterion
    tvcompression
    tvwio
    )

