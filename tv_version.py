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

# all variable names that start with this prefix will be picked up and
# converted to hdr/.config format
AUTOVAR_PREFIX = "VERSION_"

# program name
VERSION_PROG_NAME = "TagVFS"

# bug submission address
VERSION_HOMEPAGE = "https://github.com/mishaturnbull/TagVFS.git"

# int >= 0
VERSION_MAJOR = 0

# int >= 0
VERSION_MINOR = 0

# int >= 0
VERSION_PATCH = 0

# str matches regexp: [0-9A-Za-z-]*
VERSION_PREREL = 'prealpha'

def _run_cmd(cmd):
    return subprocess.run(
            shlex.split(cmd),
            stdout=subprocess.PIPE,
            check=True).stdout.decode('utf-8').strip()

VERSION_RAW_COMMIT_HASH = _run_cmd("git rev-parse HEAD")
VERSION_RAW_COMMIT_SHORT = _run_cmd("git rev-parse --short HEAD")
VERSION_ISDIRTY = len(_run_cmd("git status --porcelain")) > 0
if VERSION_ISDIRTY:
    _dirty_str = "-dirty"
else:
    _dirty_str = ""
VERSION_COMMIT_HASH = VERSION_RAW_COMMIT_HASH + _dirty_str
VERSION_COMMIT_SHORT = VERSION_RAW_COMMIT_SHORT + _dirty_str

VERSION_NOW = time.strftime("%Y%m%d%H%M%S", time.gmtime(time.time()))

# str matches regexp: [0-9A-Za-z-]*
VERSION_BUILD = f"{VERSION_NOW}-{VERSION_COMMIT_SHORT}"

VERSION_FULL = f"{VERSION_MAJOR}.{VERSION_MINOR}.{VERSION_PATCH}"

if len(VERSION_PREREL):
    VERSION_FULL += f"-{VERSION_PREREL}"

if len(VERSION_BUILD):
    VERSION_FULL += f"+{VERSION_BUILD}"


