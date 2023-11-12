.. _unittests:

Writing unit tests
==================

As discussed in :ref:`devenv-unittest`, unit testing is performed with
`Criterion`_.  Criterion's documentation should be the primary source for
information about functions and macros available in unit tests.

Related unit tests should get their own C file under :file:`test/unittest/`.
Ensure that the file name starts with ``ut_`` and ends with ``.c``.  In this
case, the file will be discovered and built into the unittest executable
automatically.  Registration of tests is not required (Criterion does this
automagically).

Although unit tests are not part of the program documentation, to support good
practices and code quality, they should still be documented to the fullest
extent.  Further, should the unit testing or documentation framework ever be
changed, having the documentation already in place is much better than having
to go add it all retroactively.

Running unit tests
------------------

After configuring the build environment (:ref:`devenv-unittest`), the build
target ``build`` will create a ``unittest`` executable in the build directory.
Run this to run all unit tests.

.. code-block:: bash

   tagvfs$ build/unittest
   [...]
   [====] Synthesis: Tested: 4 | Passing: 4 | Failing: 0 | Crashing: 0


.. _criterion: https://github.com/Snaipe/Criterion/

Code Coverage
-------------

Unit test code coverage can be performed using ``gcov``.

1. Enable the unit test build as described above.
2. Enable the :kconfig:option:`CONFIG_COV_ENABLE` Kconfig option.
3. Build the ``build`` target as normal.
4. Build the ``doc:html`` documentation target (see :ref:`proj-docs`)
5. Run unit tests using the :file:`scripts/coverage.sh`:

   .. code-block:: bash

      tagvfs $ scripts/coverage.sh build/unittest

   .. note::

      You may need to remove the existing coverage report before rerunning it
      will be successful.  To do this, just ``bld.py doc:clean`` and go back to
      step 4.

6. When viewing your built HTML documentation locally, add ``cov/`` to the end
   of the root URL.  `If you're reading this page locally, click here
   <../cov>`_.

.. code-block:: bash

   tagvfs$ bld.py fullclean build
   [...]
   tagvfs$ bld.py doc:clean doc:html
   [...]
   tagvfs$ scripts/coverage.sh build/unittest
   [====] Synthesis: Tested: 4 | Passing: 4 | Failing: 0 | Crashing: 0
   File '/home/misha/Projects/photothing/test/unittest/ut_tvw_i_test07tvw.c'
   Lines executed:100.00% of 35
   Creating 'ut_tvw_i_test07tvw.c.gcov'

   [...]

   Writing directory view page.
   Overall coverage rate:
     lines......: 68.9% (173 of 251 lines)
     functions..: 84.6% (22 of 26 functions)

Though the coverage utility is extremely helpful for analyzing unit testing
coverage, it can also run coverage analyses on any executable, not just
``unittest``.  Reports are produced the same way.  For example:

.. code-block:: bash

   tagvfs$ bld.py fullclean build
   [...]
   tagvfs$ bld.py doc:clean doc:html
   [...]
   tagvfs$ scripts/coverage.sh build/tvwdump test/resources/test07.tvw

   [...]

   Writing directory view page.
   Overall coverage rate:
     lines......: 69.0% (214 of 310 lines)
     functions..: 81.0% (17 of 21 functions)


