include_directories(../../src/)
include_directories(${CONFIG_CRITERION_INCLUDE_PATH})

if(CONFIG_CRITERION_API_V2)
FILE(GLOB ut_sources ut2_*.c)
elseif(CONFIG_CRITERION_API_V3)
FILE(GLOB_RECURSE ut_sources *.ut.c)
endif()

add_executable(unittest ${ut_sources})
target_link_libraries(unittest
    criterion
    tvcompression
    tvwio
    )

