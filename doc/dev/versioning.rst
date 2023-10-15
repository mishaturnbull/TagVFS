Version control & management
============================

TagVFS uses Git for sourcecode management, and `semantic versioning`_ 2.x for
version numbers.

Program version numbering
-------------------------

All program numbers are controlled through *one* place.  It is my firm belief
that duplicating constants such as a version number is an antipattern.  Fight
me.

That one place is the :file:`tv_version.py`.  Its constants are documented
below.  From this file, compile-time constants are generated and provided to
sourcecode via the :file:`src/version_autogen.h` (which is then included in
:file:`src/version.h` -- this is probably the include you want to use
primarily) and to CMake environments via ... magic.

.. _this-version:

Version constants
^^^^^^^^^^^^^^^^^

The following is generated from :file:`tv_version.py`.

-------------------------------------------------------------------------------

.. automodule:: tv_version
   :members:

.. _`semantic versioning`: https://semver.org

