/* TagVFS wrapper file format writer.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-21
 *
 * Implements functions for writing TagVFS wrapper format files.
 */

#include "tvw_o.h"

int tvw_write(WRAPPER_FILE *wrap) {
    // first, update the header
    tvw_upd_hdr_from_conts(wrap);

    // and out the pipe it goes!
    FILE *fp = fopen(wrap->filename, "wb");
    if (fp == NULL) {
        int errsv = errno;
        fprintf(stderr, "Couldn't open %s: error=%d -> %s\n",
                wrap->filename, errsv, strerror(errsv));
        errno = errsv;
        return TV_CHK_ERRNO;
    }
    wrap->fp = fp;

    int err = tvw_write_hdr(wrap);
    if (err != 0) {
        // ???
        return err;
    }

    if (ftell(wrap->fp) != 88) {
        fseek(wrap->fp, 88, SEEK_SET);
    }

    size_t wrote = fwrite(
            wrap->metadata,
            sizeof(char),
            wrap->sizeof_meta,
            wrap->fp);
    if (wrote != wrap->sizeof_meta) {
        TV_LOGE("Failed to write metadata; expected %zu, wrote %zu\n",
                wrap->sizeof_meta, wrote);
        return TVW_ERR_INV_META;
    }

    char split = 0x1d;
    wrote = fwrite(
            &split,
            sizeof(split),
            1,
            wrap->fp);
    if (wrote != 1) {
        TV_LOGE("Failed to write soc GS; expected 1, got %zu\n", wrote);
        return TVW_ERR_INV_SOCGS;
    }

    wrote = fwrite(
            wrap->contents,
            sizeof(char),
            wrap->sizeof_cont,
            wrap->fp);
    if (wrote != wrap->sizeof_cont) {
        TV_LOGE("Failed to write contents; expected %zu, wrote %zu\n",
                wrap->sizeof_cont, wrote);
        return TVW_ERR_INV_CONT;
    }

    fclose(wrap->fp);
    wrap->fp = NULL;
    return 0;
}

int tvw_upd_hdr_from_conts(WRAPPER_FILE *wrap) {
    wrap->header.format_version = CONFIG_FORMAT_VERSION;
    wrap->header.start_of_contents_gs = 88 + wrap->sizeof_meta;
    wrap->header.len_of_contents = wrap->sizeof_cont;

    if (wrap->contents != NULL) {
        uint8_t *shabuf = hash_of((uint8_t*)(wrap->contents), wrap->sizeof_cont);
        memcpy(&(wrap->header.sha512), shabuf, SHA512_DIGEST_LENGTH);
    }

    return 0;
}

int tvw_write_hdr(WRAPPER_FILE *wrap) {
    uint32_t fv = htobe32(wrap->header.format_version);
    size_t wrote = fwrite(
            &fv,
            sizeof(fv),
            1,
            wrap->fp);
    if (wrote != 1) {
        TV_LOGE("fwrite format_version failed: %zu\n", wrote);
        return TV_CHK_ERRNO;
    }

    uint64_t socgs = htobe64(wrap->header.start_of_contents_gs);
    wrote = fwrite(
            &socgs,
            sizeof(socgs),
            1,
            wrap->fp);
    if (wrote != 1) {
        TV_LOGE("fwrite soc gs failed: %zu\n", wrote);
        return TV_CHK_ERRNO;
    }

    uint64_t loc = htobe64(wrap->header.len_of_contents);
    wrote = fwrite(
            &loc,
            sizeof(loc),
            1,
            wrap->fp);
    if (wrote != 1) {
        TV_LOGE("fwrite loc failed: %zu\n", wrote);
        return TV_CHK_ERRNO;
    }

    uint16_t cam = htobe16(wrap->header.comp_algo_meta);
    wrote = fwrite(
            &cam,
            sizeof(cam),
            1,
            wrap->fp);
    if (wrote != 1) {
        TV_LOGE("fwrite cam failed: %zu\n", wrote);
        return TV_CHK_ERRNO;
    }

    uint16_t caf = htobe16(wrap->header.comp_algo_file);
    wrote = fwrite(
            &caf,
            sizeof(caf),
            1,
            wrap->fp);
    if (wrote != 1) {
        TV_LOGE("fwrite caf failed: %zu\n", wrote);
        return TV_CHK_ERRNO;
    }

    wrote = fwrite(
            &(wrap->header.sha512),
            sizeof(wrap->header.sha512[0]),
            sizeof(wrap->header.sha512),
            wrap->fp);
    if (wrote != SHA512_DIGEST_LENGTH) {
        TV_LOGE("fwrite sha failed: %zu\n", wrote);
        return TV_CHK_ERRNO;
    }

    return 0;

}

