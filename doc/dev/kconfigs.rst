:orphan:

.. _kconfig-search:

Kconfigs
========

TagVFS utilizes the Linux Kconfig system for configuring builds in an easy
manner (I know, I know... it's a tiny project next to the kernel and other
projects that typically use Kconfig, but I just wanted to see if I could).
Support is handled by @ulfalizer's kconfiglib for Python.

Options are exposed to both the CMake build system and the source code.  Note
that, as usual with kconfig, option names specified in the ``Kconfig`` files
are prepended with ``CONFIG_`` by the processor (so, if you make a Kconfig
option named ``FOO``, the reference in code needs to be ``CONFIG_FOO``).

You may search for Kconfig options below.  If you came here hoping for just a
list of every option available, fear not -- enter ``.*`` and you'll see all
results!

.. kconfig:search::

