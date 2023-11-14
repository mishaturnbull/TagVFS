# expose our header files
include_directories(.)

# declare the shared library
add_library(tvmisc
   ioutils.c
)

# Okayyyyyy, this one is a bit wild.
# See also the :file:`cfgsnapshot_exe.c`; it has some good in-source docs
# explaining the *usage* of all this linking madness.
#
# We're declaring an executable here:
add_executable(tvcfgsnapshot
    cfgsnapshot_exe.c
    )

# but this executable needs a pinch of extra herbs and spices.  Specifically,
# it needs the full text of :file:`src/config.h` and
# :file:`src/version_autogen.h`.  Stubborn arse that I am, I refuse to re-type
# those files in every time there's a change -- I also refuse to
# programmatically generate a header file that's a wall of ``char config[] =
# {0x23, ... };`` even though that would probably work very well.
#
# so, instead, we can use the linker to generate object files containing more
# or less exactly that.  We do that with the linker (see ``man ld``), then take
# the output files it made and, a little later down this chain, push them into
# our executable as *source files*.
add_custom_command(
    OUTPUT "_config_h.o"
    COMMAND cp ${CMAKE_SOURCE_DIR}/src/config.h ./ && ${CMAKE_LINKER} --relocatable --format binary --output _config_h.o config.h
    DEPENDS ../config.h
    VERBATIM)
add_custom_command(
    OUTPUT "_version_autogen_h.o"
    COMMAND cp ${CMAKE_SOURCE_DIR}/src/version_autogen.h ./ && ${CMAKE_LINKER} --relocatable --format binary --output _version_autogen_h.o version_autogen.h
    DEPENDS ../version_autogen.h
    VERBATIM)

# now that we've told cmake where it can get those files from, add a bit of
# target machinery so it bothers to actually *do* it
add_custom_target(_config_h DEPENDS _config_h.o)
add_custom_target(_version_autogen_h DEPENDS _version_autogen_h.o)
add_dependencies(tvcfgsnapshot 
    _config_h
    _version_autogen_h
    )

# and finally, add those as sources to the executable.
#
# why is ``PRIVATE`` there?  Dunno!  but getting rid of it causes cmake errors
# -- so \shrug.
target_sources(tvcfgsnapshot
    PRIVATE _config_h.o
    PRIVATE _version_autogen_h.o
    )

# and that's it!

