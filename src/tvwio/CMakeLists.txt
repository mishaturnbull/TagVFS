# expose our header files
include_directories(.)

# Define wrapper IO library
add_library(tvwio
    tvwrapper.c
    tvw_i.c
    tvw_o.c
    hashing.c
    )

# OpenSSL needed for hash computations
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
target_link_libraries(tvwio OpenSSL::Crypto)

# libxml2 needed for metadata
find_package(LibXml2 REQUIRED)
target_link_libraries(tvwio LibXml2::LibXml2)

# compression library
target_link_libraries(tvwio tvcompression)

# tvwdump executable target
add_executable(tvwdump 
    wrapper_dump.c
    )
target_link_libraries(tvwdump tvwio)

# tvwmake executable target
add_executable(tvwmake 
    wrapper_write.c
    )
target_link_libraries(tvwmake tvwio)

