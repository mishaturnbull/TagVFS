include(scripts/kconfig.cmake)

function(do_version_cmake_defs)

    # make the call to the version info generator.  found out the hard way
    # (30min?  ish? of troubleshooting) that this command needs to NOT be in
    # quotes, otherwise it fails with no error or output.
    execute_process(
        COMMAND python ${CMAKE_SOURCE_DIR}/scripts/version_config_generator.py 
            .version
    )

    # just use this to do all the dirty work for us.  the .version output of
    # that version config generator script was tailor-made to work as input to
    # this function.  exposes all the version infos to CMake variables
    import_kconfig(VERSION_ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/.version)

    message("Version info loaded: ${VERSION_PROG_NAME} ${VERSION_FULL}")

endfunction()

function(do_version_header hdrfile)

    # again, command needs to *NOT* be in quotes
    execute_process(
        COMMAND python ${CMAKE_SOURCE_DIR}/scripts/version_config_generator.py 
            ${CMAKE_SOURCE_DIR}/${hdrfile}
    )

endfunction()

