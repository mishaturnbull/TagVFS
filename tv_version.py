#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
This file defines version information and string constants for TagVFS.

***This is the ONLY place version information should be updated!!***

TagVFS uses Semantic Versioning v2.x (2.0.0 at time of writing).
See https://semver.org .
"""

import shlex
import subprocess
import time

#: all variable names that start with this prefix will be picked up and
#: converted to hdr/.config format
#:
#: .. note::
#:    this variable is *NOT* output to auto-generated header / cmake inclusions
#:
#: :type: str
AUTOVAR_PREFIX = "VERSION_"

#: program name
#:
#: :type: str
VERSION_PROG_NAME = "TagVFS"

#: bug submission address
#:
#: :type: str
VERSION_HOMEPAGE = "https://github.com/mishaturnbull/TagVFS.git"

#: copyright string (keep it short here)
#:
#: :type: str
VERSION_COPYRIGHT = "2023, Michael Turnbull (Misha)"

#: major version number (the X in vX.Y.Z)
#:
#: :type: int >= 0
VERSION_MAJOR = 0

#: minor version number (the Y in vX.Y.Z)
#:
#: :type: int >= 0
VERSION_MINOR = 0

#: patch version number (the Z in vX.Y.Z)
#:
#: :type: int >= 0
VERSION_PATCH = 0

#: prerelase version text
#:
#: if not blank, this is appended to the version identifier after a dash.  for
#: example, setting this to :python:`'prealpha'` will add a :code:`-prealpha`
#: appended to the end of the :code:`vX.Y.Z` string.
#:
#: :type: str matches regexp: :regexp:`[0-9A-Za-z-]*`
VERSION_PREREL = 'prealpha'

def _run_cmd(cmd):
    """
    Run a command and return its output.
    """
    return subprocess.run(
            shlex.split(cmd),
            stdout=subprocess.PIPE,
            check=True).stdout.decode('utf-8').strip()

#: The full-length hash of the current git HEAD.
#:
#: :type: str matches regexp: :regexp:`[0-9a-f]{41}`
VERSION_RAW_COMMIT_HASH = _run_cmd("git rev-parse HEAD")

#: The abbreviated hash of the current git HEAD.
#:
#: :type: str matches regexp: :regexp:`[0-9a-f]{7,41}`
VERSION_RAW_COMMIT_SHORT = _run_cmd("git rev-parse --short HEAD")

#: whether or not the current git state is "dirty"
#:
#; :type: bool
VERSION_ISDIRTY = len(_run_cmd("git status --porcelain")) > 0

if VERSION_ISDIRTY:
    _dirty_str = "-dirty"
else:
    _dirty_str = ""

#: The full-length hash of the current git HEAD, plus :python:`'-dirty'` if the
#: git state is dirty.
#:
#: :type: str matches regexp: :regexp:`[0-9a-f]{41}(-dirty)?`
VERSION_COMMIT_HASH = VERSION_RAW_COMMIT_HASH + _dirty_str

#: The abbreviated hash of the current git HEAD, plus :python:`'-dirty'` if the
#: git state is dirty.
#:
#: :type: str matches regexp: :regexp:`[0-9a-f]{7,41}(-dirty)?`
VERSION_COMMIT_SHORT = VERSION_RAW_COMMIT_SHORT + _dirty_str

#: Build timestamp.
#:
#: Note that this timestamp is automatically generated from the current system
#: whenever the :file:`tv_version.py` file is run -- which is very near the
#: first thing that happens in a build.  It is also run every time the
#: *documentation* is built -- so the string you see in the docs below may not
#: be exactly what you get in the compiled exec.
#:
#: This is represented as a string parseable with the strptime code
#: :code:`"%Y%m%d%H%M%S"`.
#:
#: :type: str matches regexp: :regexp:`[0-9]{14}`
VERSION_NOW = time.strftime("%Y%m%d%H%M%S", time.gmtime(time.time()))

#: Build metadata text
#:
#: Defines extra build-related metadata.  By default, is a dash joining the
#: values of :py:attr:`VERSION_NOW` and :py:attr:`VERSION_COMMIT_SHOrt`.
#:
#: :type: str matches regexp: :regexp:`[0-9A-Za-z-]*`
VERSION_BUILD = f"{VERSION_NOW}-{VERSION_COMMIT_SHORT}"

#: Major + minor version numbers joined by a dot.
#:
#: :type: str matches regexp: :regexp:`[0-9]+\.[0-9]+`
VERSION_MAJMIN = f"{VERSION_MAJOR}.{VERSION_MINOR}"

#: Major + minor + patch version numbers joined by dots, plus prerelase if
#: not blank.
#:
#: :type: str matches regexp: :regexp:`[0-9]+\.[0-9]+\.[0-9]+-[0-9A-Za-z-]*`.
VERSION_FULL = f"{VERSION_MAJOR}.{VERSION_MINOR}.{VERSION_PATCH}"

if len(VERSION_PREREL):
    VERSION_FULL += f"-{VERSION_PREREL}"

#: Major + minor + patch + prerelease + build metadata.  The whole shebang.
#:
#: :type: str
VERSION_EXTRA = VERSION_FULL

if len(VERSION_BUILD):
    VERSION_EXTRA += f"+{VERSION_BUILD}"


