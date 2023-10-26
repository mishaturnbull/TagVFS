===========
``tvctool``
===========

.. program:: tvctool

.. comment
   yes, we could use the name section from the man_pages var in conf.py.  but,
   if docs are built in a different format (html), the name doesn't show up.

Name
----

tvctool - interact with TagVFS compression library

Synopsis
--------

``tvctool`` [option]...

Description
-----------

This page describes the behavior of the ``tvctool`` program.  It is a
command-line utility for interacting with TagVFS's transparent compression
library.

It provides a means of listing compression algorithms available and their IDs,
as well as compressing and decompressing arbitrary data using any available
algorithm.

``tvctool`` is built alongside the ``tvcompression`` library, which is
statically linked into it at compile time.

Options
-------

.. option:: -p, --print-table

   Prints the table of available compression algorithms, then exits.

   No further options are required.

.. option:: -c, --compress

   Compress the given data in accordance with other supplied options.

.. option:: -x, --extract

   Extract (decompress) the given data in accordance with other supplied
   options.

.. option:: -a, --algo=ALGO

   Use the specified algorithm to de/compress data.

.. option:: -i, --input=FILE

   Read input data from the given file.  If not supplied, a prompt will be
   given and then standard input will be read.

   In compress mode (:option:`-c`), this file should contain the *uncompressed*
   data.  In extract mode (:option:`-x`), this file should contain the
   *compressed* data.

.. option:: -o, --output=FILE

   Write output data to the given file.  If not supplied, stdout will be used.

   In compress mode (:option:`-c`), this file will contain the *compressed*
   data.  In extract mode (:option:`-x`), this file will contain the
   *decompressed* data.

.. option:: -V, --version

   Print version information and exit.

.. option:: -?, --help

   Print a help message and exit.

.. option:: --usage

   Print a short usage message and exit.

See also
--------

:doc:`tvwdump`, :doc:`tvwmake`

