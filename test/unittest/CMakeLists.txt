include_directories(../../src/)
include_directories(${CONFIG_CRITERION_INCLUDE_PATH})

FILE(GLOB ut_sources ut_*.c)

add_executable(unittest ${ut_sources})
target_link_libraries(unittest
    criterion
    tvwio
    )

