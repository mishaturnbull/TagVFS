# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os

# TODO: Remove these two lines (VV) once chadnorvell/sphinx-kconfig PR #2 merge
import sys
sys.path.insert(0, "/home/misha/Projects/sphinx-kconfig/")

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'tagvfs'
copyright = '2023, Michael Turnbull (Misha)'
author = 'Misha Turnbull'
primary_domain = "c"

keep_warnings = True

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
        'hawkmoth',
        'myst_parser',
        'sphinx_kconfig',
        'sphinx.ext.todo',
        ]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# Ensure the right parser is picked for file types
source_suffix = {
        '.rst': 'restructuredtext',
        '.md': 'markdown',
        }

# -- Options for Kconfig plugin ----------------------------------------------

kconfig_generate_db = True
kconfig_root_path = os.path.abspath("../Kconfig")

# -- Options for HawkMoth ----------------------------------------------------
hawkmoth_root = os.path.abspath("../src/")

# -- Options for ToDo ext ----------------------------------------------------
todo_include_todos = True

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# juuuuust in case
html_theme = "sphinx_book_theme"

html_static_path = ['_static']


# -- Options for man page output ---------------------------------------------
man_pages = [
        ("manpages/tvwdump", "tvwdump", "", "", 1),
        ("manpages/tvwmake", "tvwmake", "", "", 1),
    ]

