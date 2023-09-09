/*
 * TagVFS wrapper file format writer.
 * AGPL-3.0 license.
 * Initial rev Misha Turnbull 2023-07-21
 *
 * Defines structs & functions for writing TagVFS wrapper format files.
 */

#pragma once

#include "tvwrapper.h"
#include "hashing.h"

#ifdef CONFIG_TVWIO_VALIDATE_XML
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#endif

extern int TVWI_XML_RD_FLAGS;

/**
 * Writes a wrapper file to disk as specified by data in the wrapper structure.
 * :c:var:`WRAPPER_FILE.filename` is used as the destination file; it must be
 * writeable, or this function will fail.  The current metadata and file
 * contents arrays are written into the file without alteration.
 *
 * If :kconfig:option:`CONFIG_TVWO_VALIDATE_XML` is enabled, this function will
 * refuse to write the file if the metadata is determined to be invalid,
 * instead returning :c:macro:`TVW_ERR_INV_META`.
 *
 * :param wrap: Wrapper file structure to save on disk.
 * :return: 0 on success, otherwise, an error code.
 */
int tvw_write(struct WRAPPER_FILE *wrap);

/**
 * Update the :c:var:`WRAPPER_FILE.header` element of the given
 * wrapper file from its contents and metadata.  Sizes are reset and the file
 * contents hash is recomputed.
 *
 * Internal use only.  This is called by :c:func:`tvw_write` before any data is
 * written to disk.
 *
 * :param wrap: Wrapper file structure to operate on.
 */
void tvw_upd_hdr_from_conts(struct WRAPPER_FILE *wrap);

/**
 * Write the header information to disk.  Does not do any sanity-checking on
 * header contents, and does not write metadata or file contents.
 *
 * Internal use only.  This is called by :c:func:`tvw_write` as a helper
 * function.
 *
 * :param wrap: Wrapper file structure to operate on.
 * :return: 0 on success, otherwise, an error code.
 */
int tvw_write_hdr(struct WRAPPER_FILE *wrap);

