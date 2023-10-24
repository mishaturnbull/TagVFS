# Work in progress!

TagVFS is **extremely** young in its lifespan.  Treat all described features as
future-speak until you see it working.  During the 0.x.y version, changes may
be made without warning; including API details, removal of small or major
features, and complete changes in scope.

# TagVFS

TagVFS is, or will be one day, a FUSE filesystem driver that organizes
arbitrary files by tags.

Conceptually, it is similar to many other photo managers that can present the
same photo under many different tags.  TagVFS differs in that it can be used
for any type of file, and requires no client software to use -- it simply
presents a mounted filesystem.  This could even be exported as a network share
for mounting on remote systems.  It also does not keep one central index of
files / tags; each file stores its own metadata (including tags) in a special
wrapper file.

## Documentation

Sphinx is used for all documentation.  Before you can read the documentation in
a pretty fashion, you currently must build it yourself.  See the [documentation
from-zero instructions][docfromzero] for how to do this.

[docfromzero]: doc/from-zero.md

# Contributions

Git Flow model for core contributors; GitHub fork-and-pull model for non-core
contributors.  All welcome!

