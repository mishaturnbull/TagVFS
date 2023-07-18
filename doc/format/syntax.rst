===================
Wrapper file syntax
===================

Introduction
------------

The wrapper file is composed of three primary sections:

1. Header
2. Metadata
3. File contents

These sections are separated by an ASCII group separator (GS) character,
``0x1d``.

.. note::
   There is only **ONE** ``0x1d`` GS character declared by the wrapper file.
   If a second, or more, is/are present, they are part of the file contents or
   metadata!  (i.e., **everything** including more GS characters after the
   second one is part of the file contents.

All data in wrapper files is stored as **big endian** (i.e., MSB first).

Header
------

The header section consists of the following information and is intended to
improve the efficiency of the RM half of WORM (write-once-read-many)
operations.  It consists of the following:

1. Format file version
2. Offset of start-of-file-content GS marker, in bytes
3. Length of the file contents, in bytes
4. Compression algorithm applicable to metadata
5. Compression algorithm applicable to file contents
6. SHA-512 hash of the file contents

This information is stored at fixed-byte offsets, starting from the zeroth byte
in the file:

+-----------------------+------------------+-----------------+----------------+
| Content               | First byte index | Last byte index | Length (bytes) |
+-----------------------+------------------+-----------------+----------------+
| Format file version   | 0                | 3               | 4              |
| (32-bit unsinged int) |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+
| Offset of start-of-   | 4                | 11              | 8              |
| contents GS marker    |                  |                 |                |
| (64-bit unsigned int) |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+
| Length of the file    | 12               | 19              | 8              |
| contents, in bytes    |                  |                 |                |
| (64-bit unsigned int) |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+
| Compression algorithm | 20               | 21              | 2              |
| selector for metadata |                  |                 |                |
| (16-bit unsigned int) |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+
| Compression algorithm | 22               | 23              | 2              |
| selector for file     |                  |                 |                |
| contents              |                  |                 |                |
| (16-bit unsigned int) |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+
| SHA-512 hash of file  | 24               | 87              | 64             |
| contents              |                  |                 |                |
| (512-bit unsigned     |                  |                 |                |
| int)                  |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+

The total length of the header is therefore 88 bytes.

Metadata
--------

The metadata section consists of XML data describing arbitrary metadata about
the wrapped file.  XML was chosen due to its flexibility for describing
arbitrary data types, custom tag definitions, and wealth of preexisting
libraries to work with XML data.

The metadata section begins immediately after the header section (e.g., if the
header section is 88 bytes, the metadata begins at the 89th byte (``fp[88]`` in
a zero-indexed array).  It is ended by an ASCII GS character (``0x1d``).  The
index of this character is specified in the header section.

.. note::
   There **may** be ASCII GS (``0x1d``) characters present in the metadata, either in the tags themselves or in the data they contain.

File contents
-------------

The file contents section consists of arbitrary data representing the entire,
whole, unaltered contents of the "wrapped" file.  It begins immediately after
an ASCII GS (``0x1d``) character, which is in turn immediately after the end of
the metadata section.  The last byte in the file contents section is the last
byte in the overall wrapper file.

.. note::
   Null terminators **may** be present within the file contents section that do
   **not** represent the end of the wrapper file.  Always work based off the
   known size of the file contents section as stored in the header section.


The size of the file contents, in bytes, is specified in the header section; as
is the SHA-512 digest of the contents.

