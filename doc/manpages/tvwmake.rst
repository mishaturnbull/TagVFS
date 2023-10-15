===========
``tvwmake``
===========

.. program:: tvwmake

Name
----

tvwmake - create a .tvw file

Synopsis
--------

``tvwmake`` [options]... *OUTFILE* [*INFILE*]

Description
-----------

``tvwmake`` is a command-line utility that creates a TagVFS wrapper file with
the given options.  It always prompts the user for metadata provided via
``stdin``, and can either accept an input file as the file contents or, if none
is provided, it will prompt the user again via ``stdin`` for the file contents.

It is primarily intended for human interaction, not for creating wrapper files
programmatically.  If you need to create a wrapper file programmatically, you
should probably just use the ``tvwio`` library.

Options
-------

.. option:: outfile

    Name of the file to output to.  Ideally, should end in ``.tvw``, but you
    won't be reprimanded if it doesn't.

.. option:: [infile]

    Name of the file to copy into the file contents section of the wrapper.  If
    provided, this file is opened, read, and copied byte-for-byte into the
    wrapper as its file contents.  The original input file is not altered in
    any way.  May be any file type.

.. option:: -m, --comp-meta

    Compression algorithm to be used for the metadata section.  If given, must
    be an integer corresponding to the compression algorithm table.  Default is
    zero (0, no compression).

.. option:: -f, --comp-file

    Compression algorithm to be used for the file contents section.  If given,
    must be an integer corresponding to the compression algorithm table.
    Default is zero (0, no compression).

.. option:: -V, --version

   Print version information and exit.

.. option:: -?, --help

   Print a help message and exit.

.. option:: --usage

   Print a short usage message and exit.

See also
--------

:doc:`tvwdump`

