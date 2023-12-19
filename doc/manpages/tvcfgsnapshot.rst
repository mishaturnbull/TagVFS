.. _man-tvcfgsnapshot:

=================
``tvcfgsnapshot``
=================

.. program:: tvcfgsnapshot

.. comment
   yes, we could use the name section from the man_pages var in conf.py.  but,
   if docs are built in a different format (html), the name doesn't show up.

Name
----

tvcfgsnapshot - display the configuration of this installation of TagVFS

Synopsis
--------

``tvcfgsnapshot``

Description
-----------

This page describes the behavior of the ``tvcfgsnapshot`` program.  It is a
command-line utility for printing the configuration state of this installation
of TagVFS.

When invoked, this utility prints current configuration information as
requested by the provided options.

Options
-------

.. option:: -h, --headers

   Prints C preprocessor language that describes the configuration and version
   information that was made available to the compiler when this built of
   TagVFS was built.

.. option:: -V, --version

   Print version information and exit.

.. option:: -?, --help

   Print a help message and exit.

.. option:: --usage

   Print a short usage message and exit.

See also
--------

:doc:`tvctool`

Where the information printed by :option:`-h` comes from:

* :ref:`kconfig-search`
* :ref:`version-meta`

