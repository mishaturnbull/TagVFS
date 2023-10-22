# include the config.h file that's right here
include_directories(.)
add_compile_options(-imacros "config.h")

add_subdirectory(tvmisc)
add_subdirectory(tvcompression)
add_subdirectory(tvwio)

