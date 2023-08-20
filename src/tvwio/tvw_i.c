/* TagVFS wrapper file format reader.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-13
 *
 * Implements functions for reading TagVFS wrapper format files.
 */

#include "tvw_i.h"

int TVWI_XML_RD_FLAGS = 0
#ifdef CONFIG_TVWI_XF_RECOVER
    || XML_PARSE_RECOVER
#endif
#ifdef CONFIG_TVWI_XF_NOERROR
    || XML_PARSE_NOERROR
#endif
#ifdef CONFIG_TVWI_XF_NOWARNING
    || XML_PARSE_NOWARNING
#endif
#ifdef CONFIG_TVWI_XF_PEDANTIC
    || XML_PARSE_PEDANTIC
#endif
#ifdef CONFIG_TVWI_XF_NONET
    || XML_PARSE_NONET
#endif
#ifdef TVWI_XF_IGNORE_ENC
    || XML_PARSE_IGNORE_ENC
#endif
    || 0;

int read_wrapper(char *filename, WRAPPER_FILE *out) {
    int err = read_wrap_fp(filename, out);
    if (err != 0) {
        TV_LOGE("Unable to open file for reading: %d\n", err);
        // if the file couldn't be opened, can't really proceed.  abort.
        return TV_CHK_ERRNO;
    }

    int ret = 0;
    err = read_wrap_hdr(out);
    if (err != 0) {
        TV_LOGE("Failed to read header!  Orig err: %d\n", err);
        ret = TVW_ERR_INV_HDR;
    }

    out->metadata = calloc(sizeof(char), out->header.start_of_contents_gs - 88);
    err = read_metadata(out);
    if (err != 0) {
        TV_LOGE("Failed to read metadata!  Orig err: %d\n", err);
        if (ret == 0) {
            ret = TVW_ERR_INV_META;
        } else {
            ret = TVW_ERR_INV_MULT;
        }

    }

    out->contents = calloc(sizeof(char), out->header.len_of_contents);
    err = read_contents(out);
    if (err != 0) {
        TV_LOGE("Failed to read contents!  Orig err: %d\n", err);
        if (ret == 0) {
            ret = TVW_ERR_INV_CONT;
        } else {
            ret = TVW_ERR_INV_MULT;
        }
    }

    fclose(out->fp);
    out->fp = NULL;

    return ret;
}

int read_wrap_fp(char* filename, WRAPPER_FILE *out) {
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

    return 0;
}

int read_wrap_hdr(WRAPPER_FILE *wrap) {
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
    TV_LOGV("after FV ftell = %zd\n", ftell(wrap->fp));

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
    TV_LOGV("after SOCGS ftell = %zd\n", ftell(wrap->fp));

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
    TV_LOGV("after LOC ftell = %zd\n", ftell(wrap->fp));

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
    TV_LOGV("after CAM ftell = %zd\n", ftell(wrap->fp));

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
    TV_LOGV("after CAC ftell = %zd\n", ftell(wrap->fp));

    nread = fread(
            &(wrap->header.sha512),
            sizeof(wrap->header.sha512[0]),
            sizeof(wrap->header.sha512),
            wrap->fp);
    if (nread != (sizeof(wrap->header.sha512) / sizeof(wrap->header.sha512[0]))) {
        fprintf(stderr, "fread() sha512 failed: %zu\n", nread);
        return 1;
    }
    TV_LOGV("after SHA ftell = %zd\n", ftell(wrap->fp));

    return 0;
}

int read_metadata(WRAPPER_FILE *wrap) {
    // figure out how big the metadata is, reportedly
    size_t size_meta = wrap->header.start_of_contents_gs - 88;

#ifdef CONFIG_TVWIO_READ_SECURITY
    // figure out how big the file *really* is.  this is important to make sure
    // we don't read (or, try to read) past end of the file -- remember
    // heartbleed?
    fseek(wrap->fp, 0L, SEEK_END);
    size_t actual_file_size = ftell(wrap->fp);
    if (size_meta + 88 > actual_file_size) {
        return EFAULT;
    }
#endif

    // jump to start of the metadata section, 88 bytes in, and read the
    // metadata into *buf
    fseek(wrap->fp, 88, SEEK_SET);
    size_t nread = fread(
            wrap->metadata,
            sizeof(char),
            size_meta,
            wrap->fp);
    if (nread != size_meta) {
        TV_LOGD("read_metadata: fread() meta failed: %zu\n", nread);
        return 1;
    }
    wrap->sizeof_meta = nread;

    xmlResetLastError();
    wrap->xmlroot = xmlReadMemory(
            wrap->metadata,
            wrap->sizeof_meta,
            "",
            NULL,
            TVWI_XML_RD_FLAGS);
    xmlErrorPtr xmlerr = xmlGetLastError();
    if (xmlerr != NULL) {
        TV_LOGD("detected GetLastError != NULL: %p\n", (void*)xmlerr);
        xmlFreeDoc(wrap->xmlroot);
        wrap->xmlroot = NULL;

        // i can't decide whether this issue should be a showstopper or not.
        // so, kconfig it is!
#ifdef CONFIG_TVWI_INVALID_XML_IS_FATAL
        TV_LOGE("ERROR: invalid XML in metadata!  aborting!\n");
        return TVW_ERR_INV_META;
#else
        TV_LOGW("WARNING: invalid XML in metadata!  Continuing anyways!\n");
        wrap->xmlerr = xmlerr;
#endif
    }

    return 0;
}

int read_contents(WRAPPER_FILE *wrap) {
#ifdef CONFIG_TVWIO_READ_SECURITY
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
#endif

    // jump to start of contents
    fseek(wrap->fp, wrap->header.start_of_contents_gs + 1, SEEK_SET);
    size_t nread = fread(
            wrap->contents,
            sizeof(char),
            wrap->header.len_of_contents,
            wrap->fp);
    if (nread != wrap->header.len_of_contents) {
        TV_LOGD("read_contents: fread() contents failed: %zu\n", nread);
        return 1;
    }
    wrap->sizeof_cont = nread;

    return 0;
}


