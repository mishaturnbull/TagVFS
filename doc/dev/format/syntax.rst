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
3. Length of the (compressed) file contents, in bytes
4. Compression algorithm applicable to metadata
5. Compression algorithm applicable to file contents
6. SHA-512 hash of the file contents (when decompressed)

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

.. warning::

   The SHA-512 hash of the file contents is **NOT** intended to provide any
   security measures!!  Its only purpose is to provide an indication of whether
   the file contents may have become corrupted during processing.  Do **NOT**
   rely on this for any security purposes!

Metadata
--------

The metadata section consists of XML data describing arbitrary metadata about
the wrapped file.  XML was chosen due to its flexibility for describing
arbitrary data types, custom tag definitions, and wealth of preexisting
libraries to work with XML data.

The metadata section begins immediately after the header section (e.g., if the
header section is 88 bytes, the metadata begins at the 89th byte (:c:`fp[88]`
in a zero-indexed array).  It is ended by an ASCII GS character (``0x1d``).
The index of this character is specified in the header section.

.. note::

   There **may** be ASCII GS (``0x1d``) characters present in the metadata,
   either in the tags themselves or in the data they contain.


.. seealso::

   The :ref:`metadata_layout` page describes the contents of the metadata.

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

This is a hexdump (``xxd`` style) of a valid wrapper file of version zero.
Note that the metadata and file contents appear garbled; they are "compressed"
by the compression algorithm test function (a 1-shift Caesar cipher).

.. code-block::
   :caption: Example TagVFS wrapper file (:file:`test/resources/test08.tvw`)

   00000000: 0000 0000 0000 0000 0000 006f 0000 0000  ...........o....
   00000010: 0000 00d5 0001 0001 0b80 81b1 e7e4 014a  ...............J
   00000020: 4f8d e92f 1860 5953 dc27 04ac 1d31 b73f  O../.`YS.'...1.?
   00000030: cb01 6f86 b675 16dd 8841 fb7f f89b d8e9  ..o..u...A......
   00000040: bd93 5c80 fb05 78bf 4279 7be4 0268 6e4d  ..\...x.By{..hnM
   00000050: 7787 86fc 7718 0053 3d75 776e 3f0b 4e66  w...w..S=uwn?.Nf
   00000060: 7562 6562 7562 220b 3d30 7577 6e3f 0b1d  ubebub".=0uwn?..
   00000070: 2421 5562 6857 4754 0b0b 4a21 6f66 6665  $!UbhWGT..J!offe
   00000080: 2175 7021 7176 7521 626d 6d21 6e7a 2171  !up!qvu!bmm!nz!q
   00000090: 6970 7570 7421 7470 6e66 7869 6673 662f  ipupt!tpnfxifsf/
   000000a0: 0b0b 2b21 5c45 7064 766e 666f 7562 756a  ..+!\Epdvnfoubuj
   000000b0: 706f 5e5c 6570 6474 5e0b 0b5c 6570 6474  po^\epdt^..\epdt
   000000c0: 5e3b 2165 7064 300b 0b24 2144 706f 7573  ^;!epd0..$!Dpous
   000000d0: 6a63 7675 6a70 6f74 0b0b 486a 7521 476d  jcvujpot..Hju!Gm
   000000e0: 7078 216e 7065 666d 2167 7073 2164 7073  px!npefm!gps!dps
   000000f0: 6621 6470 6f75 736a 6376 7570 7374 3c21  f!dpousjcvupst<!
   00000100: 486a 7549 7663 2167 7073 6c2e 626f 652e  HjuIvc!gpsl.boe.
   00000110: 7176 6d6d 216e 7065 666d 2167 7073 216f  qvmm!npefm!gps!o
   00000120: 706f 2e64 7073 660b 6470 6f75 736a 6376  po.dpsf.dpousjcv
   00000130: 7570 7374 2f21 2142 6d6d 2178 666d 6470  upst/!!Bmm!xfmdp
   00000140: 6e66 220b 0b                             nf"..


