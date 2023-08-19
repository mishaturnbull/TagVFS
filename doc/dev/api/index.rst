============
TagVFS C API
============

This page documents the TagVFS C API.  All sourcecode is found in the ``src/``
folder within this repo.

General-purpose macros & helpers
--------------------------------

This section describes things that are defined immediately under ``src/``
intended for general-purpose usage across the entire program.

``debugs.h``
^^^^^^^^^^^^
.. c:autodoc:: debugs.h

``errors.h``
^^^^^^^^^^^^
.. c:autodoc:: errors.h

``tvwio`` library
-----------------

This section describes the functions available in the ``tvwio`` library.  All
headers are available under ``src/tvwio/``.  Linking may be performed
statically at compile time by ``CMakeLists.txt``, as seen in both the
``tvwmake`` and ``tvwdump`` executable definitions:

.. code-block:: cmake
   :caption: CMakeLists excerpt

    # tvwdump executable target
    add_executable(tvwdump wrapper_dump.c)
    target_link_libraries(tvwdump tvwio)

``tvwrapper.h``
^^^^^^^^^^^^^^^
.. c:autodoc:: tvwio/tvwrapper.h

``hashing.h``
^^^^^^^^^^^^^
.. c:autodoc:: tvwio/hashing.h

``tvw_i.h``
^^^^^^^^^^^
.. c:autodoc:: tvwio/tvw_i.h

``tvw_o.h``
^^^^^^^^^^^
.. c:autodoc:: tvwio/tvw_o.h

