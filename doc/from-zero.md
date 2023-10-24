# Zero-to-docs guide

Because TagVFS doesn't yet have online documentation hosted anywhere, you'll
have to build and view it yourself.  Some day in the hopefully not-so-distant
future, this whole document will be unnecessary -- you'll just click a link to
the premade docs to explore.

This document intends to show the fastest possible way to build the Sphinx
documentation yourself for further reading.  It assumes you have only the
following prerequisites to start:

1. A Linux machine
2. Git
3. Python 3

## Acquire documentation

You'll first need to *get* the documentation source.

1. Clone this repository somewhere of your choosing:
   `git clone https://github.com/mishaturnbull/TagVFS.git`
2. Move into the repo you just cloned: `cd TagVFS`

## Set up environment

A few packages are required for the documentation.  Once you've build the
documentation, you can see these in the development environment page.  For now:

1. Install Python Clang library: `sudo apt install python3-clang`
2. Install the necessary Pip packages:
   `sudo pip install -r scripts/env/packages-pip.lst`

This will set you up for building all forms of documentation except PDF.

## Build documentation

I recommend starting with the structured HTML documentation first:

1. `python3 bld.py doc:html`

## View documentation

You've now built the documentation.  To see it:

1. Open your favorite web browser
2. In the URL bar, enter `file://` followed by the full path to the directory
   you cloned the repo into (as an example, for me it's
   `file:///home/misha/shared/Projects/TagVFS/`.  Note the THREE slashes --
   it's from the `/` filesystem!)
3. Add `doc/_build/html/` to the end of the path, so it looks like
   `file:///home/misha/shared/Projects/TagVFS/doc/_build/`
4. Hit Enter
5. Get your reading glasses

That's it!  Enjoy!

