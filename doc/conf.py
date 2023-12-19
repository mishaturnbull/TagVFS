# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys
from hawkmoth.util import compiler, readthedocs

# ensure Clang is configured correctly on readthedocs
readthedocs.clang_setup()

topdir = os.path.split(os.path.split(__file__)[0])[0]
sys.path.insert(0, topdir)
import tv_version

# TODO: Remove once chadnorvell/sphinx-kconfig PR #2 merge
sys.path.insert(1, "/home/misha/Projects/sphinx-kconfig/")

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = tv_version.VERSION_PROG_NAME
copyright = tv_version.VERSION_COPYRIGHT
author = 'Misha Turnbull'
version = tv_version.VERSION_MAJMIN
release = tv_version.VERSION_FULL

primary_domain = "c"
keep_warnings = True

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# more verbose warnings
nitpicky = True
# but not some hawkmoth stuff without intersphinx references to the C language
nitpick_ignore = [
        ("c:identifier", "int"),
        ("c:identifier", "uint8_t"),
        ("c:identifier", "uint16_t"),
        ("c:identifier", "uint32_t"),
        ("c:identifier", "uint64_t"),
        ("c:identifier", "size_t"),
        ("c:identifier", "FILE"),
        ("c:identifier", "xmlDocPtr"),
        ("c:identifier", "xmlErrorPtr"),
        ]

extensions = [
        'hawkmoth',
        'myst_parser',
        'sphinx_kconfig',
        'sphinx.ext.todo',
        'sphinx.ext.autodoc',
        'sphinx.ext.intersphinx',
        'sphinx_copybutton',
        ]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store',
        "from-zero.md",
        ]

# Ensure the right parser is picked for file types
source_suffix = {
        '.rst': 'restructuredtext',
        '.md': 'markdown',
        }

rst_prolog = r"""
.. role:: python(code)
   :language: python
.. role:: c(code)
   :language: c
"""

# -- Options for InterSphinx -------------------------------------------------
intersphinx_mapping = {
    'python': ('https://docs.python.org/3', None)
    }

# -- Options for Kconfig plugin ----------------------------------------------
kconfig_generate_db = True
kconfig_root_path = os.path.abspath("../Kconfig")

# -- Options for HawkMoth ----------------------------------------------------
hawkmoth_root = os.path.abspath("../src/")

_extra_includes = [f"-I{d[0]}" for d in os.walk(hawkmoth_root)]
hawkmoth_clang = [
        *compiler.get_include_args('gcc'),
        *_extra_includes,

        # TODO: does this break on anyone else's setup?
        "-I/usr/include/libxml2",
        ]

# if you wound up here debugging hawkmoth "I cAnT fInD tHaT iNcLuDe FiLe"
# errors, uncomment these two VV and you'll get a printout of where it's
# looking for them
#from pprint import pprint
#pprint(hawkmoth_clang)

# -- Options for ToDo ext ----------------------------------------------------
todo_include_todos = True

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# juuuuust in case
html_theme = "sphinx_book_theme"

html_theme_options = {
        "repository_url": tv_version.VERSION_HOMEPAGE,
        "repository_branch": tv_version.VERSION_GIT_BRANCH,
        "path_to_docs": "doc",
        "use_repository_button": True,
        "use_source_button": True,
        "use_edit_page_button": True,
        "use_issues_button": True,
    }

if tv_version.VERSION_MAJOR == 0:
    html_theme_options["announcement"] = \
        f"<b style=\"color:red;\">TagVFS is in unstable version {version}, " \
        "and may change at any time!</b>"

html_static_path = ['_static']

# -- Options for man page output ---------------------------------------------
man_pages = [
        ("manpages/tvctool", "tvctool", "", "", 1),
        ("manpages/tvwdump", "tvwdump", "", "", 1),
        ("manpages/tvwmake", "tvwmake", "", "", 1),
        ("manpages/tvcfgsnapshot", "tvcfgsnapshot", "", "", 1),
    ]

