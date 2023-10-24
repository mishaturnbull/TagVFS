``tvmisc`` library
==================

This page describes the functions available in the ``tvmisc`` library.  This library holds numerous small, not necessarily correlated functions, which can be used for their general utility anywhere in the program.

It may be statically linked at compile time by ``CMakeLists.txt``, as shown:

.. code-block:: cmake
   :caption: Linking ``tvmisc`` library with CMake
   :linenos:
   :emphasize-lines: 8

   # define the executable
   add_executable(tvctool
       tvcomp_exe.c
   )

   # link necessary libraries
   target_link_libraries(tvctool tvcompression)
   target_link_libraries(tvctool tvmisc)

``ioutils.h``
^^^^^^^^^^^^^
.. c:autodoc:: tvmisc/ioutils.[ch]

