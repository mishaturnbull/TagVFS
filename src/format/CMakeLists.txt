# Define wrapper IO library
add_library(tvwio
    tvw_i.c
    tvw_o.c
    hashing.c
    )

# and link in OpenSSL for it (needed for hash computations)
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
target_link_libraries(tvwio OpenSSL::Crypto)

# tvwdump executable target
add_executable(tvwdump wrapper_dump.c)
target_link_libraries(tvwdump tvwio)

# tvwmake executable target
add_executable(tvwmake wrapper_write.c)
target_link_libraries(tvwmake tvwio)

