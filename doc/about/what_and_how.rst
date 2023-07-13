===============================================
What exactly *is* TagVFS, and how does it work?
===============================================

TagVFS utilizes Linux's `FUSE`_ system to present a virtual file system (VFS)
that allows you to browse a central database of files based on metadata (tags)
associated with each file.  Each file is only stored once, but can appear in
many places on this VFS based on its tags.

For example, a file ``IMG_1234.png`` may be tagged with the ``Dog`` and
``Lake`` tags.  Though the file is stored only once on disk, in the VFS, it
would be found under both the ``Dog`` and ``Lake`` folders.  Opening the file
from either place would give you the exact same file (not "identical files" --
the *same* file).  If you wanted to add the ``Summer`` tag to the file, simply
make a ``Summer`` folder and copy ``IMG_1234.png`` into it.

Because the organization system is, to the operating system, simply a folder
tree, it can be trivially exported to any other system for browsing files.
This makes TagVFS easily multi-client; a server hosting the central database
and running TagVFS can export the VFS to an NFS or Samba share, which can be
picked up by any client machine without that client requiring any special
software whatsoever.

.. _FUSE: https://www.kernel.org/doc/html/latest/filesystems/fuse.html

