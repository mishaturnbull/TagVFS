#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import shlex
import subprocess
import sys

try:
    import kconfiglib
except ImportError:
    msg = f"""
Unable to import kconfiglib.  Please ensure it's installed via Pip before
proceeding!

    pip install kconfiglib


"""
    print(msg)
    exit(1)

def run_cmd(cmd, **kwargs):
    p = subprocess.run(shlex.split(cmd),
            capture_output=True,
            **kwargs)
    return p.stdout.decode('utf-8')

###############################################################################

def bldhelp(cmd):
    """
    bldhelp for bld.py.
    """
    maxlen = 0
    for entry in REGISTRY:
        maxlen = max(maxlen, len(entry))
    maxlen = maxlen + 3
    for entry in REGISTRY:
        print(f"    {entry.ljust(maxlen)}{REGISTRY[entry].__doc__.strip()}")

def doc(cmd):
    """
    Makes documentation!
    """
    os.chdir("doc")
    
    if len(cmd) < 2:
        # if not specified, build latexpdf
        doctype = 'latexpdf'
    else:
        # user said something in particular
        doctype = cmd[1]

    out = run_cmd(f"make {doctype}")
    print(out)

def menuconfig(cmd):
    """
    Kconfig menuconfig TUI.
    """
    os.system("menuconfig")
    os.system("genconfig --header-path src/config.h")

def build(cmd):
    """
    Main CMake target.
    """
    # check if the .config file exists
    if not os.path.exists(".config"):
        # it does not.  create it with all options at default
        os.system("alldefconfig")
    # it definitely exists now -- header-ize it
    os.system("genconfig --header-path src/config.h")

    os.chdir("build")
    # call os.system instead of run_cmd -- os.system causes live,
    # while-still-happening printouts, while run_cmd collects them all and
    # dumps it back at the end
    os.system("cmake ..")
    os.system("make")

def clean(cmd):
    """
    Call cmake's clean target.
    """
    os.chdir("build")
    os.system("make clean")

def fullclean(cmd):
    """
    rm -rf build/
    """
    os.chdir("build")
    os.system("rm -rf ./*")

REGISTRY = {
        # don't remove this first 'help' entry!
        "help": bldhelp,
        "doc": doc,
        "menuconfig": menuconfig,
        "build": build,
        "clean": clean,
        "fullclean": fullclean,
    }

###############################################################################
# Run machinery

def process_arg(arg):
    parts = arg.split(':')
    try:
        func = REGISTRY[parts[0]]
    except KeyError:
        return [f"Unknown argument {parts[0]}"]
    return func(parts)

def main():
    args = sys.argv[1:]
    errs = []
    start_dir = os.getcwd()

    if len(args) == 0:
        process_arg('help')
        return

    for arg in args:
        os.chdir(start_dir)
        new_errs = process_arg(arg)
        if new_errs is not None:
            errs = [*errs, *new_errs]

    if len(errs) == 0:
        print("Done - apparent success!")
    else:
        print("Done - did not succeed.  See errors below:\n\n")
        for i, err in enumerate(errs):
            print(f"{i+1}: {err.strip()}\n")
        print(f"{len(errs)} errors")

if __name__ == '__main__':
    main()

