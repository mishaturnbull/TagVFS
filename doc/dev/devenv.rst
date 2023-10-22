Setting up a development environment
====================================

Setting up an environment to compile TagVFS from source is, *currently*, not
too harsh :).  You'll need a Linux machine to do it on (maybe it'll work on
Windows?  I honestly haven't tried.  If you get it to compile on Windows (or
something else), please let me know).

Prerequisites
-------------

1. A Linux machine
2. Git
3. Clone this repository: ``git clone git@github.com:mishaturnbull/TagVFS.git``
4. Move into the repo: ``cd TagVFS``
5. Install the necessary toolchain:

   1. Install prerequisites from APT:

   .. code-block:: shell

      sudo apt install $(cat scripts/env/packages-apt.list)

   2. Install prereqisites from Pip:

   .. code-block:: shell

      sudo pip install -r scripts/env/packages-pip.lst

   .. note::

      I get yelled at a lot for not using venv's, or conda, or just any other
      manner of installing Python packages.  Do it however you like; just make
      sure the packages in :file:`scripts/env/packages-pip.lst` are available
      to the build script.  However, know that I hate venvs and conda, and will
      not provide support relating to pip module installation should you choose
      to use them.

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
   See the :ref:`kconfig-search` page for more information and a list of/search
   options.

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
With the above given environment setup, you should be able to build all formats
except PDF.  To build PDF documentation, you will need a working installation
of LaTeX as per the `Sphinx LaTeX instructions`_.

To make Sphinx documentation, the ``doc`` target passes through its
colon-separated command to the Sphinx makefile.  For example:

.. code-block:: shell

   bld.py doc:latexpdf
   bld.py doc:dirhtml
   bld.py doc:help

If you so desire, you can also just go run the Sphinx makefile as usual:

.. code-block:: shell

   cd doc/
   make html
   make latexpdf
   make man

Sphinx generated documentation will be placed in ``doc/_build/``.

.. note::
   Unlike many C projects using Sphinx, Doxygen is *not* a component in
   TagVFS's documentation.  Instead, the `hawkmoth`_ project is used to read
   the sourcecode and interpret documentation comments directly.

.. _hawkmoth: https://hawkmoth.readthedocs.io/en/stable/index.html
.. _Sphinx LaTeX instructions: https://www.sphinx-doc.org/en/master/usage/builders/index.html#sphinx.builders.latex.LaTeXBuilder

