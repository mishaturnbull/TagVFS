Setting up a development environment
====================================

Setting up an environment to compile TagVFS from source is, *currently*, not
too harsh :).  You'll need a Linux machine to do it on (maybe it'll work on
Windows?  I honestly haven't tried.  If you get it to compile on Windows (or
something else), please let me know).

Prerequisites
-------------

1. A Linux machine
2. Build toolchain:

   1. Install prerequisites from APT:

   .. code-block:: shell

      sudo apt install git gcc make cmake python3 libssl-dev libxml2-dev

   2. Install prereqisites from Pip:

   .. code-block:: shell

      sudo pip install kconfiglib

3. Clone this repository: ``git clone git@github.com:mishaturnbull/TagVFS.git``


Compiling
---------

Use ``bld.py`` for nearly everything.  Normally, when I have a terminal
specifically for compiling things, ``export PATH=$PATH:.`` is pretty helpful
(lets you just ``$ bld.py`` instead of ``$ ./bld.py``).

To compile:

   ``bld.py build``

   You could also do the usual ``cd build && cmake .. && make`` -- that would
   be okay too.

To configure:

   ``bld.py menuconfig``

.. note::
   See the :ref:`kconfig-search` page for a list of options.

To reset the world:

   ``bld.py fullclean``

   This is equivalent to removing everything in ``build/``.

You may chain commands together in one shot to do multiple things in order:

   ``bld.py fullclean build``


Running with no arguments will produce a small help output describing the
allowed commands:

   ``bld.py``

Compiled executables are placed in the ``build`` directory.

Project documentation 
---------------------

If you only want to *read* the documentation, well, evidently you've found it!
If you'd like to work on/compile the documentation as well as the project, the
following additional prerequisites are needed as well:

1. Install documentation requisites from APT:

.. code-block:: shell

   sudo apt install python3-clang

2. Install documentation requisites from Pip:

.. code-block:: shell

   sudo pip install sphinx hawkmoth sphinx-book-theme myst_parser
   sudo pip install git+https://github.com/chadnorvell/sphinx-kconfig

To make Sphinx documentation, the ``doc`` target passes through its
colon-separated command to the Sphinx makefile.  For example:

   | ``bld.py doc:latexpdf``
   | ``bld.py doc:dirhtml``
   | ``bld.py doc:help``

Sphinx generated documentation will be placed in ``doc/_build/``.

.. note::
   Unlike many C projects using Sphinx, Doxygen is *not* a component in
   TagVFS's documentation.  Instead, the `hawkmoth`_ project is used to read
   the sourcecode and interpret documentation comments directly.

.. _hawkmoth: https://hawkmoth.readthedocs.io/en/stable/index.html

