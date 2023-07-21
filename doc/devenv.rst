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

   1. C compiler: ``sudo apt install gcc``
   2. Make and Cmake: ``sudo apt install make cmake``
   3. Python 3: ``sudo apt install python3``
   4. Python kconfiglib: ``sudo pip install kconfiglib``

3. OpenSSL development library: ``sudo apt install libssl-dev``
4. Clone this repository: ``git clone git@github.com:mishaturnbull/TagVFS.git``

Compiling
---------

Use ``bld.py`` for nearly everything.  Normally, when I have a terminal
specifically for compiling things, ``export PATH=$PATH:.`` is pretty helpful
(lets you just ``$ bld.py`` instead of ``$ ./bld.py``).

To compile:

   ``bld.py build``

To configure:

   ``bld.py menuconfig``

To reset the world:

   ``bld.py fullclean``

You may chain commands together in one shot to do multiple things in order:

   ``bld.py fullclean build``

Running with no arguments will produce a small help output describing the
allowed commands:

   ``bld.py``

Compiled executables are placed in the ``build`` directory.


