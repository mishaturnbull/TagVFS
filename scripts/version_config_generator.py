#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Generate a .config-style file from versioninfo.py data.
"""

import inspect
import sys
import os

topdir = os.path.split(os.path.split(__file__)[0])[0]
sys.path.insert(0, topdir)

import tv_version

def get_autovars():
    members = inspect.getmembers(tv_version)
    for i in range(len(members)-1, -1, -1):
        if not members[i][0].startswith(tv_version.AUTOVAR_PREFIX):
            del members[i]
    return dict(members)

def configify_one(key, val):
    out = f"{key}="

    if isinstance(val, bool):
        if val:
            out += 'y'
        else:
            out += 'n'
    elif isinstance(val, int) or isinstance(val, float):
        out += repr(val)
    elif isinstance(val, str):
        out += f'"{val}"'
    else:
        raise ValueError(f"I don't know how to translate {type(val)} into config!")

    return out + '\n'

def hdrify_one(key, val):
    out = f"#define {key} "

    if isinstance(val, bool):
        if val:
            out += '1'
        else:
            out += '0'
    elif isinstance(val, int) or isinstance(val, float):
        out += repr(val)
    elif isinstance(val, str):
        out += f'"{val}"'
    else:
        raise ValueError(f"I don't know how to translate {type(val)} into header!")

    return out + '\n'

def main(outfile):
    members = get_autovars()
    lines = []

    if outfile == 'stdout':
        func = lambda k, v: configify_one(k, v).strip()
    elif outfile.endswith('.version'):
        func = configify_one
    elif outfile.endswith('.h'):
        func = hdrify_one
    else:
        raise ValueError(f"Idk what to do with {outfile}!")

    for key, val in members.items():
        lines.append(func(key, val))

    if outfile == 'stdout':
        for line in lines:
            print(line)
    else:
        with open(outfile, 'w') as fp:
            fp.writelines(lines)

if __name__ == '__main__':
    main(sys.argv[1])


