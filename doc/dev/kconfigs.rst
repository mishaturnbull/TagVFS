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

.. note::

   If you want information about the program version or general metadata that
   *doesn't* change much between configurations, take a look at the
   :doc:`versioning` page.

Once you've found the option you want here, if you want to change it before a build of TagVFS:

1. Launch menuconfig from bash:

   .. code-block:: shell

      tagvfs/ $ bld.py menuconfig

2. Press ``/`` to enter the "Jump to symbol/choice/menu/config" menu
3. Type in the config name without the leading ``CONFIG_``
4. Press ``Enter`` to jump to the symbol

You may search for Kconfig options below.  If you came here hoping for just a
list of every option available, fear not -- enter ``.*`` and you'll see all
results!

.. kconfig:search::

