# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys
sys.path.insert(0, "/home/misha/Projects/sphinx-kconfig/")

# options are PDJ, RTD_DARK, or any of the normal sphinx builtins
HTML_DOC_MODE = "sphinx_book_theme"


# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'tagvfs'
copyright = '2023, Misha Turnbull'
author = 'Misha Turnbull'
primary_domain = "c"

keep_warnings = True

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
        'hawkmoth',
        'myst_parser',
        'sphinx_kconfig',
        ]

kconfig_generate_db = True
kconfig_root_path = os.path.abspath("../Kconfig")

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

source_suffix = {
        '.rst': 'restructuredtext',
        '.md': 'markdown',
        }

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# juuuuust in case
html_theme = "classic"

if HTML_DOC_MODE == "PDJ":
    import sphinx_pdj_theme
    html_theme = 'sphinx_pdj_theme'
    html_theme_path = [sphinx_pdj_theme.get_html_theme_path()]
    default_dark_mode = True
elif HTML_DOC_MODE == "RTD_DARK":
    extensions.append("sphinx_rtd_dark_mode")
else:
    html_theme = HTML_DOC_MODE

html_static_path = ['_static']


# -- Options for man page output ---------------------------------------------
man_pages = [
        ("manpages/tvwdump", "tvwdump", "", "", 1),
        ("manpages/tvwmake", "tvwmake", "", "", 1),
    ]

# -- Options for HawkMoth ----------------------------------------------------
hawkmoth_root = os.path.abspath("../src/")

