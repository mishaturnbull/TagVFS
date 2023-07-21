/* TagVFS wrapper file format reader.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-13
 *
 * Implements functions for reading TagVFS wrapper format files.
 */

#include "tvfs_read.h"

int test_rd_file(char* filename, WRAPPER_FILE *out) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        // something went wrong
        int errsv = errno;
        fprintf(stderr, "Couldn't open %s: error=%d -> %s\n", 
                filename, errsv, strerror(errsv));
        return errsv;
    }

    out->filename = filename;
    out->fp = fp;

    int err = test_rd_hdr(out);

    return err;
}

int test_rd_hdr(WRAPPER_FILE *wrap) {
    size_t nread = fread(
            &(wrap->header.format_version), 
            sizeof(wrap->header.format_version),
            1, 
            wrap->fp);
    if (nread != 1) {
        fprintf(stderr, "fread() format_version failed: %zu\n", nread);
        return 1;
    }
    wrap->header.format_version = be32toh(wrap->header.format_version);

    nread = fread(
            &(wrap->header.start_of_contents_gs),
            sizeof(wrap->header.start_of_contents_gs),
            1,
            wrap->fp);
    if (nread != 1) {
        fprintf(stderr, "fread() start_of_contents_gs failed: %zu\n", nread);
        return 1;
    }
    wrap->header.start_of_contents_gs =
        be64toh(wrap->header.start_of_contents_gs);

    nread = fread(
            &(wrap->header.len_of_contents),
            sizeof(wrap->header.len_of_contents),
            1,
            wrap->fp);
    if (nread != 1) {
        fprintf(stderr, "fread() len_of_contents failed: %zu\n", nread);
        return 1;
    }
    wrap->header.len_of_contents = be64toh(wrap->header.len_of_contents);

    nread = fread(
            &(wrap->header.comp_algo_meta),
            sizeof(wrap->header.comp_algo_meta),
            1,
            wrap->fp);
    if (nread != 1) {
        fprintf(stderr, "fread() comp_algo_meta failed: %zu\n", nread);
        return 1;
    }
    wrap->header.comp_algo_meta = be16toh(wrap->header.comp_algo_meta);

    nread = fread(
            &(wrap->header.comp_algo_file),
            sizeof(wrap->header.comp_algo_file),
            1,
            wrap->fp);
    if (nread != 1) {
        fprintf(stderr, "fread() comp_algo_file failed: %zu\n", nread);
        return 1;
    }
    wrap->header.comp_algo_file = be16toh(wrap->header.comp_algo_file);

    nread = fread(
            &(wrap->header.sha512),
            sizeof(wrap->header.sha512[0]),
            sizeof(wrap->header.sha512),
            wrap->fp);
    if (nread != (sizeof(wrap->header.sha512) / sizeof(wrap->header.sha512[0]))) {
        fprintf(stderr, "fread() sha512 failed: %zu\n", nread);
        return 1;
    }

    return 0;
}

int read_metadata(WRAPPER_FILE *wrap, char *buf) {
    // figure out how big the metadata is, reportedly
    size_t size_meta = wrap->header.start_of_contents_gs - 88;

    // figure out how big the file *really* is.  this is important to make sure
    // we don't read (or, try to read) past end of the file -- remember
    // heartbleed?
    fseek(wrap->fp, 0L, SEEK_END);
    size_t actual_file_size = ftell(wrap->fp);
    if (size_meta + 88 > actual_file_size) {
        return EFAULT;
    }

    // jump to start of the metadata section, 88 bytes in, and read the
    // metadata into *buf
    fseek(wrap->fp, 88, SEEK_SET);
    size_t nread = fread(
            buf,
            sizeof(char),
            size_meta,
            wrap->fp);
    if (nread != size_meta) {
        fprintf(stderr, "fread() meta failed: %zu\n", nread);
        return 1;
    }

    return 0;
}

int read_contents(WRAPPER_FILE *wrap, char *buf) {
    // make sure we don't read past end of the file
    fseek(wrap->fp, 0L, SEEK_END);
    size_t actual_file_size = ftell(wrap->fp);
    // wrongness may be *negative* here, so throw it to a signed int.  negative
    // wrongness is actually ok; it means we'll read *less* of the file than
    // probably intended, but it is a *safe* operation to perform, so full
    // steam ahead
    int64_t wrongness = (wrap->header.start_of_contents_gs + 1 +
            wrap->header.len_of_contents) - actual_file_size;
    if (wrongness > 0) {
        fprintf(stderr, 
                "Refusing to read %s contents; header shows more data than "
                    "actual file size (wrongness=%zu)\n",
                wrap->filename,
                wrongness);
        return EFAULT;
    }

    // jump to start of contents
    fseek(wrap->fp, wrap->header.start_of_contents_gs + 1, SEEK_SET);
    size_t nread = fread(
            buf,
            sizeof(char),
            wrap->header.len_of_contents,
            wrap->fp);
    if (nread != wrap->header.len_of_contents) {
        fprintf(stderr, "fread() contents failed: %zu\n", nread);
        return 1;
    }

    return 0;
}


