# expose header files
include_directories(.)

# define compression functions library
add_library(tvcompression
    tables.c
    passthru.c
    )


