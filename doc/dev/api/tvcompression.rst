``tvcompression`` library
=========================

This page describes the functions available in the ``tvcompression`` library.
All headers are available under ``src/tvcompression``.  Linking may be
performed statically at compile time by ``CMakeLists.txt``, as seen in the
``tvwio`` library:

.. code-block:: cmake
   :caption: Linking ``tvcompression`` with CMake

   # link compression library for use in tvwio
   target_link_libraries(tvwio tvcompression)

This also means that the ``tvwio`` library includes all items described here.

``tvcompression.h``
^^^^^^^^^^^^^^^^^^^
.. c:autodoc:: tvcompression/tvcompression.[ch]

``compression_table.h``
^^^^^^^^^^^^^^^^^^^^^^^
.. c:autodoc:: tvcompression/compression_table.[ch]

``comp_passthru.h``
^^^^^^^^^^^^^^^^^^^
.. c:autodoc:: tvcompression/comp_passthru.[ch]

