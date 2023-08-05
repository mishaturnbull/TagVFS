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

Example
-------

This is a hexdump (``xxd`` style) of a valid wrapper file of version zero:

.. code-block::
   :caption: Example TagVFS wrapper file

	00000000: 0000 0000 0000 0000 0000 00f1 0000 0000  ................
	00000010: 0000 0122 0000 0000 b543 fa8f 4cdb 7d20  ...".....C..L.}
	00000020: 8b2f 3332 1d96 e8eb 7a42 44ea 23c3 d50c  ./32....zBD.#...
	00000030: c906 2b50 7002 322c c1da 3b24 9283 7126  ..+Pp.2,..;$..q&
	00000040: c03c f3c0 9a79 992e 8d9b ab40 8ba4 08a0  .<...y.....@....
	00000050: 87d1 31e1 4036 c78a 3c74 6167 3e63 6d61  ..1.@6..<tag>cma
	00000060: 6b65 3c2f 7461 673e 0a3c 6e6f 7465 733e  ke</tag>.<notes>
	00000070: 3c6e 6f74 653e 5468 6973 2069 7320 736f  <note>This is so
	00000080: 6d65 2058 4d4c 206d 6574 6164 6174 613c  me XML metadata<
	00000090: 2f6e 6f74 653e 3c6e 6f74 653e 5468 6973  /note><note>This
	000000a0: 2069 7320 7468 6520 7072 6f6a 6563 7427   is the project'
	000000b0: 7320 434d 616b 654c 6973 7473 2e74 7874  s CMakeLists.txt
	000000c0: 2066 696c 6520 6174 2074 696d 6520 6f66   file at time of
	000000d0: 2063 7265 6174 652d 7468 6973 2d66 696c   create-this-fil
	000000e0: 652e 3c2f 6e6f 7465 3e3c 2f6e 6f74 6573  e.</note></notes
	000000f0: 3e1d 636d 616b 655f 6d69 6e69 6d75 6d5f  >.cmake_minimum_
	00000100: 7265 7175 6972 6564 2856 4552 5349 4f4e  required(VERSION
	00000110: 2033 2e31 3829 0a70 726f 6a65 6374 280a   3.18).project(.
	00000120: 2020 2020 5461 6756 4653 0a20 2020 2056      TagVFS.    V
	00000130: 4552 5349 4f4e 2030 2e30 2e30 0a20 2020  ERSION 0.0.0.
	00000140: 2029 0a0a 2320 7075 7420 6f75 7470 7574   )..# put output
	00000150: 2073 7475 6666 7320 696e 2074 6865 2062   stuffs in the b
	00000160: 7569 6c64 2066 6f6c 6465 722c 2074 6f70  uild folder, top
	00000170: 206c 6576 656c 0a53 4554 2843 4d41 4b45   level.SET(CMAKE
	00000180: 5f52 554e 5449 4d45 5f4f 5554 5055 545f  _RUNTIME_OUTPUT_
	00000190: 4449 5245 4354 4f52 5920 247b 5461 6756  DIRECTORY ${TagV
	000001a0: 4653 5f53 4f55 5243 455f 4449 527d 2f62  FS_SOURCE_DIR}/b
	000001b0: 7569 6c64 290a 0a23 2063 6f6d 7069 6c65  uild)..# compile
	000001c0: 7220 666c 6167 7320 666f 7220 6576 6572  r flags for ever
	000001d0: 7977 6865 7265 0a53 4554 2843 4d41 4b45  ywhere.SET(CMAKE
	000001e0: 5f43 5858 5f46 4c41 4753 2022 2d57 616c  _CXX_FLAGS "-Wal
	000001f0: 6c20 2d57 6578 7472 6122 290a 0a61 6464  l -Wextra")..add
	00000200: 5f73 7562 6469 7265 6374 6f72 7928 7372  _subdirectory(sr
	00000210: 6329 0a0a                                c)..


