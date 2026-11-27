# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

from pathlib import Path
import sys

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Zigbee R23 add-on for nRF Connect SDK'
copyright = '2026, Nordic Semiconductor'
author = 'Nordic Semiconductor'
release = 'Latest'
version = 'Latest'

# Paths

DOC_BASE = Path(__file__).resolve().parent
REPO_BASE = DOC_BASE.parent

# -- NCS version for intersphinx ---------------------------------------------

NCS_VERSION_NUMBER = "3.4.2"

try:
    import west.manifest

    manifest = west.manifest.Manifest.from_file(REPO_BASE / "west.yml")
    nrf_projects = manifest.get_projects(["nrf"])
    if nrf_projects:
        revision = nrf_projects[0].revision
        if revision.startswith("v"):
            revision = revision[1:]
        NCS_VERSION_NUMBER = revision
except Exception:
    pass

_NCS_DOCS_BASE = f"https://nrfconnectdocs.nordicsemi.com/ncs/{NCS_VERSION_NUMBER}"
_NCS_DOCS_STORAGE = f"https://ncsdoc.z6.web.core.windows.net/ncs/{NCS_VERSION_NUMBER}"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

sys.path.insert(0, str(DOC_BASE / "_extensions"))

extensions = [
    'sphinx.ext.intersphinx',
    'breathe',
    'sphinxcontrib.mscgen',
    'sphinx_tabs.tabs',
    'sphinx_togglebutton',
    'sphinx_copybutton',
    'page_filter',
    'memory_data',
    'memory_viz',
    'stack_viz',
]

intersphinx_mapping = {
    "nrf": (
        f"{_NCS_DOCS_BASE}/nrf/",
        f"{_NCS_DOCS_STORAGE}/nrf/objects.inv",
    ),
    "zephyr": (
        f"{_NCS_DOCS_BASE}/zephyr/",
        f"{_NCS_DOCS_STORAGE}/zephyr/objects.inv",
    ),
    "mcuboot": (
        f"{_NCS_DOCS_BASE}/mcuboot/",
        f"{_NCS_DOCS_STORAGE}/mcuboot/objects.inv",
    ),
    "nrfxlib": (
        f"{_NCS_DOCS_BASE}/nrfxlib/",
        f"{_NCS_DOCS_STORAGE}/nrfxlib/objects.inv",
    ),
}

# The root document.
root_doc = 'index'

templates_path = ['_templates']
exclude_patterns = ['_build_sphinx', '_build_doxygen', 'venv', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_ncs_theme'

html_theme_options = {
    'docsets': {},
    "ncs_url": "https://nrfconnectdocs.nordicsemi.com/ncs/latest/nrf/",
    "ncs_label": "nRF Connect SDK Docs",
    "addons_url": "https://nrfconnect.github.io/ncs-app-index/",
    "bare_metal_url": "",
    "logo_url": "https://docs.nordicsemi.com",
}

html_show_sphinx = False
html_extra_path = ['versions.json']

## -- Options for Breathe ----------------------------------------------------
# https://breathe.readthedocs.io/en/latest/index.html
#
# WARNING: please, check breathe maintainership status before using this
# extension in production!

breathe_projects = {'ncs-zigbee': '_build_doxygen/xml'}
breathe_default_project = 'ncs-zigbee'
breathe_default_members = ('members', )

# Include following files at the end of each .rst file.
rst_epilog = """
.. include:: /links.txt
.. include:: /shortcuts.txt
"""