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
#ifdef CONFIG_TVWI_XF_IGNORE_ENC
    || XML_PARSE_IGNORE_ENC
#endif
#ifdef CONFIG_TVWI_XF_NOBLANKS
    || XML_PARSE_NOBLANKS
#endif
    || 0;

int read_wrapper(char *filename, struct WRAPPER_FILE *out) {
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

int read_wrap_fp(char* filename, struct WRAPPER_FILE *out) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        // something went wrong
        int errsv = errno;
        fprintf(stderr, "Couldn't open %s: error=%d -> %s\n", 
                filename, errsv, strerror(errsv));
        return errsv;
    }

    // we need to track the filename in the struct, because we're told to.
    // however...
    //
    // we need to allocate another string for it to ensure that this memory is
    // freeable by us later on (in tvwfree).  in many common usage patterns,
    // the string given to this function is *not freeable* by the tvwio
    // library, because it came from main's argv or something the like.  the
    // api doc includes a strict and clear warning that callers of
    // read_wrapper() (and therefore this function) are responsible for freeing
    // their own char* filename they passed as an argument.
    out->filename = calloc(sizeof(char), strlen(filename));
    memcpy(out->filename, filename, strlen(filename));
    out->fp = fp;

    return 0;
}

int read_wrap_hdr(struct WRAPPER_FILE *wrap) {
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

int read_metadata(struct WRAPPER_FILE *wrap) {
    // figure out how big the metadata is, reportedly
    size_t size_meta_ondisk = wrap->header.start_of_contents_gs - 88;

#ifdef CONFIG_TVWIO_READ_SECURITY
    // figure out how big the file *really* is.  this is important to make sure
    // we don't read (or, try to read) past end of the file -- remember
    // heartbleed?
    fseek(wrap->fp, 0L, SEEK_END);
    size_t actual_file_size = ftell(wrap->fp);
    if (size_meta_ondisk + 88 > actual_file_size) {
        return EFAULT;
    }
#endif

    // jump to start of the metadata section, 88 bytes in, and read the
    // metadata into *buf
    fseek(wrap->fp, 88, SEEK_SET);

    // allocate a buf to read into (before decompression)
    char *ondisk = calloc(size_meta_ondisk, sizeof(char));
    size_t nread = fread(
            ondisk,
            sizeof(char),
            size_meta_ondisk,
            wrap->fp);
    if (nread != size_meta_ondisk) {
        TV_LOGD("read_metadata: fread() meta failed: %zu\n", nread);
        return 1;
    }

    size_t size_meta_decomp = -1;
    char *decomp_meta;
    int err = decompress(wrap->header.comp_algo_meta,
            &size_meta_ondisk,
            &ondisk,
            &size_meta_decomp,
            &decomp_meta);

    if (err != 0) {
        free(ondisk);
        return err;
    }

    wrap->sizeof_meta = size_meta_decomp;
    wrap->metadata = decomp_meta;
    free(ondisk);

    // clear previous error so we don't detect that if there was nothing wrong
    // here
    xmlResetLastError();

    // by default, libxml2 prints parsing error/warning messages to stderr.  i
    // don't really want that -- would rather catch them programmatically and
    // handle them here than show the user this early on in the process.
    //
    // turns out the way to disable that is to set the error handlers to null,
    // and their associated context to a file pointer to log the messages to
    // instead.  to /dev/null it goes to!
    //
    /**
     * .. todo::
     *
     *    will this work on non-unix platforms?
     */
    FILE* devnull = fopen(CONFIG_SINKFILE, "w");
    xmlSetGenericErrorFunc(devnull, NULL);
    xmlSetStructuredErrorFunc(devnull, NULL);

    // do the actual parse
    wrap->xmlroot = xmlReadMemory(
            wrap->metadata,
            wrap->sizeof_meta,
            "",
            NULL,
            TVWI_XML_RD_FLAGS);

    // sinkfile is no longer needed
    fclose(devnull);

    // detect and store/show errors
    xmlErrorPtr xmlerr = xmlGetLastError();
    if (xmlerr != NULL) {
        TV_LOGD("detected GetLastError != NULL: %p\n", (void*)xmlerr);
        xmlFreeDoc(wrap->xmlroot);
        wrap->xmlroot = NULL;
        wrap->xmlerr = xmlerr;

        // i can't decide whether this issue should be a showstopper or not.
        // so, kconfig it is!
#ifdef CONFIG_TVWI_INVALID_XML_IS_FATAL
        TV_LOGE("ERROR: invalid XML in metadata!  aborting!\n");
        return TVW_ERR_INV_META;
#else
        TV_LOGW("WARNING: invalid XML in metadata!  Continuing anyways!\n");
#endif
    }

    return 0;
}

int read_contents(struct WRAPPER_FILE *wrap) {
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
    char *ondisk = calloc(wrap->header.len_of_contents, sizeof(char));
    size_t nread = fread(
            ondisk,
            sizeof(char),
            wrap->header.len_of_contents,
            wrap->fp);
    if (nread != wrap->header.len_of_contents) {
        TV_LOGD("read_contents: fread() contents failed: %zu\n", nread);
        return 1;
    }

    size_t sizeof_cont_ondisk = nread;
    size_t sizeof_cont_decomp = -1;
    char *decomp_cont;
    int err = decompress(wrap->header.comp_algo_file,
            &sizeof_cont_ondisk,
            &ondisk,
            &sizeof_cont_decomp,
            &decomp_cont);

    if (err != 0) {
        free(ondisk);
        return err;
    }
    
    wrap->sizeof_cont = sizeof_cont_decomp;
    wrap->contents = decomp_cont;
    free(ondisk);

    return 0;
}


