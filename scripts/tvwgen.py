#!python
# -*- coding: utf-8 -*-

import argparse
import hashlib

ENDIAN = 'big'

parser = argparse.ArgumentParser()

parser.add_argument("-fv",
        type=int,
        default=0,
        required=False,
        help="Format version (default = 0) -> uint32_t")
parser.add_argument("-cm",
        type=int,
        default=0,
        required=False,
        help="Compression algo for metadata (default = 0) -> uint16_t")
parser.add_argument("-cf",
        type=int,
        default=0,
        required=False,
        help="Compression algo for file contents (default = 0) -> uint16_t")
parser.add_argument('-i',
        type=str,
        dest='inputfile',
        required=False,
        help="Optionally specify an input file (otherwise STDIN prompt)")
parser.add_argument("filename",
        type=str,
        help="Destination file")
parser.add_argument('-n',
        action='store_false',
        dest='do_meta',
        required=False,
        help="Do not prompt or apply metadata")

def _read_multiline(prompt="Enter multi-line string:"):
    print(prompt)
    contents = []
    while True:
        try:
            line = input()
        except EOFError:
            break
        contents.append(line)
    return "\n".join(contents)

def gen_header(args, metadata, contents):
    m_offset = 88 + len(metadata)

    # NOTE: if for some reason `sha512` algo isn't available on target
    # platform, double-check equality between python's hashlib.sha3_512 and
    # Unix/OpenSSL sha512 digests before swapping this function out for it!
    hash_cont = hashlib.sha512(bytes(contents, 'ascii')).digest()

    # uint32_t format version
    hdr = args.fv.to_bytes(4, byteorder=ENDIAN)

    # uint64_t offset of marker between meta and file
    hdr += m_offset.to_bytes(8, byteorder=ENDIAN)

    # uint64_t length of file contents
    hdr += len(contents).to_bytes(8, byteorder=ENDIAN)

    # uint16_t compression algo for metadata
    hdr += args.cm.to_bytes(2, byteorder=ENDIAN)

    # uint16_t compression algo for file contents
    hdr += args.cf.to_bytes(2, byteorder=ENDIAN)

    # uint8_t[512] sha512 hash of file contents
    hdr += hash_cont

    return hdr

def gen_file(args, metadata, contents):
    hdr = gen_header(args, metadata, contents)
    written = 0
    with open(args.filename, 'wb') as fp:
        written += fp.write(hdr)
        written += fp.write(bytes(metadata, 'ascii'))
        written += fp.write(bytes((0x1d,)))
        written += fp.write(bytes(contents, 'ascii'))

    return written

if __name__ == '__main__':
    args = parser.parse_args()

    if args.do_meta:
        metadata = _read_multiline("Enter metadata:")
    else:
        metadata = ''

    if args.inputfile:
        with open(args.inputfile, 'r') as fp:
            contents = fp.read()
    else:
        contents = _read_multiline("Enter file contents:")
    written = gen_file(args, metadata, contents)
    print(f"Done; {written} bytes written")

