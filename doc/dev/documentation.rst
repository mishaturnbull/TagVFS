====================
TagVFS documentation
====================

TagVFS uses Sphinx for documentation (which apparently you've found), aided by
the ``hawkmoth`` Python package to pick up and parse code comments.  All
documentation source is under the ``/doc/`` folder in the repo and written in
reStructuredText format.

Code comments
-------------

Code comments in C source files (and headers) can be parsed by the ``hawkmoth``
extension to Sphinx.  In C, the necessary syntax to mark a comment as an
autodoc comment is to begin it with ``/**`` rather than just ``/*``:

    /**
     * This is an autodoc comment.
     */

   /*
    * and this is not.
    */ /*

Any reStructuredText syntax can be used from within an autodoc comment, and the
position of the comment relative to code dictates what it gets attached to (for
example, putting such a comment directly above a function declaration will
attach that documentation to that function).

Compiling documentation
-----------------------

To compile the documentation, your system must at least meet the Sphinx
requirements, and additionally install the ``hawkmoth`` package.

There are two options to compile:

1. Using ``bld.py``: from the main repo directory, run ``bld.py doc:<target>``.
2. Using Sphinx's makefile: from ``/doc/``, run ``make <target>``.

In either case, the ``<target>`` can be any of the Sphinx targets.  Use target
``help`` to see a list.  Generated documentation will be placed in
``doc/_build/``.

