``tvwio`` library
=================

This page describes the functions available in the ``tvwio`` library.  All
headers are available under ``src/tvwio/``.  Linking may be performed
statically at compile time by ``CMakeLists.txt``, as seen in both the
``tvwmake`` and ``tvwdump`` executable definitions:

.. code-block:: cmake
   :caption: Linking ``tvwio`` with CMake

    # tvwdump executable target
    add_executable(tvwdump wrapper_dump.c)
    target_link_libraries(tvwdump tvwio)

``tvwrapper.h``
^^^^^^^^^^^^^^^
.. c:autodoc:: tvwio/tvwrapper.[ch]

``hashing.h``
^^^^^^^^^^^^^
.. c:autodoc:: tvwio/hashing.[ch]

``tvw_i.h``
^^^^^^^^^^^
.. c:autodoc:: tvwio/tvw_i.[ch]

``tvw_o.h``
^^^^^^^^^^^
.. c:autodoc:: tvwio/tvw_o.[ch]

