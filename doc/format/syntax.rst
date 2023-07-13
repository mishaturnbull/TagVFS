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
   There are only TWO (2) ``0x1d`` GS characters declared by the wrapper file.
   If a third, or more, is/are present, they are part of the file contents!
   (i.e., **everything** including more GS characters after the second one is
   part of the file contents.

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
| SHA-512 hash of file  | 24               | 88              | 64             |
| contents              |                  |                 |                |
| (512-bit unsigned     |                  |                 |                |
| int)                  |                  |                 |                |
+-----------------------+------------------+-----------------+----------------+

The total length of the header is therefore 88 bytes.

