# expose header files
include_directories(.)

# define compression functions library
add_library(tvcompression
    tvcompression.c
    compression_table.c
    comp_passthru.c
    )

# define table dump target
add_executable(tvctool
    tvcomp_exe.c
)
target_link_libraries(tvctool tvcompression)
target_link_libraries(tvctool tvmisc)

