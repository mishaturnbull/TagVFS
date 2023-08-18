#!/usr/bin/python
# -*- coding: utf-8 -*-

import glob
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

if not os.path.exists("build"):
    print("!!! No `build` directory found -- I'm creating it for you!")
    os.mkdir("build")

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
    mc = os.system("menuconfig Kconfig")
    # not much point calling genconfig if menuconfig died
    if mc == 0:
        gc = os.system("genconfig Kconfig --header-path src/config.h")
        if gc != 0:
            return [f"Genconfig failed; return code {gc}.  " + \
                    "Keeping Kconfig!"]
    else:
        return [f"Menuconfig failed; return code {mc}." + \
                "  Keeping Kconfig (and not calling genconfig!)"]

def defaultconfig(cmd):
    """
    Set all Kconfig options to default.
    """
    ac = os.system("alldefconfig Kconfig")
    if ac == 0:
        gc = os.system("genconfig Kconfig --header-path src/config.h")
        if gc != 0:
            return [f"Genconfig failed; return code {gc}.  " + \
                    "Keeping Kconfig!"]
    else:
        return [f"Alldefconfig failed; return code {mc}."]

def build(cmd):
    """
    Main CMake target.
    """
    out = []
    if not os.path.exists('.config'):
        if os.path.exists("src/config.h"):
            # this is weird...
            out.append(f".config is gone, but src/config.h exists.  Please" + \
                    " fix by either deleting src/config.h, or creating " + \
                    ".config.  `bld.py menuconfig` should help.")
        else:
            # nothing at all.  most likely a fresh clone.  blast out default
            # config
            defaultconfig('')
    elif not os.path.exists('src/config.h'):
        # .config existed, but the header doesn't.  need user to invoke the
        # menuconfig command which will call genconfig and headerize it
        out.append("config.h does not exist.  Run `bld.py menuconfig`!")
    if len(out) > 0:
        return out

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

def cleanconf(cmd):
    """
    Cleans out generated configuration files (.config, src/config.h) and reset
    to defaults.
    """
    files = [".config", ".config.old", "src/config.h"]
    for f in files:
        try:
            os.remove(f)
        except:
            pass

REGISTRY = {
        # don't remove this first 'help' entry!
        "help": bldhelp,
        "doc": doc,
        "menuconfig": menuconfig,
        "defaultconfig": defaultconfig,
        "build": build,
        "clean": clean,
        "fullclean": fullclean,
        "cleanconf": cleanconf,
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

