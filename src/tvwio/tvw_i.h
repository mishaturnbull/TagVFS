/* TagVFS wrapper file format reader.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-13
 *
 * Defines structs & functions for reading TagVFS wrapper format files.
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <endian.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "debugs.h"
#include "errors.h"
#include "tvwrapper.h"
#include "hashing.h"


extern int TVWI_XML_RD_FLAGS;

/**
 * :c:func:`read_wrapper` accepts a filename argument, a pre-allocated
 * buffer object to write data into, and reads the TVW wrapper file at the
 * specified filename.  It returns an error code or zero on success.
 *
 * :param filename: the filename of the desired file to read
 * :param out: the output WRAPPER_FILE parameter
 * :return: 0 on success, otherwise, an error code
 */
int read_wrapper(char *filename, WRAPPER_FILE *out);

/**
 * Performs the file IO access operation to open a file pointer and attach it
 * to the given wrapper object.
 *
 * **Intended for internal use only!**
 *
 * :param filename: Name of the file to access
 * :param out: the wrapper file struct to attach the file pointer to
 * :return: 0 on success, otherwise, an error code
 */
int read_wrap_fp(char *filename, WRAPPER_FILE *out);

/**
 * Reads header information from a wrapper file.  Assumes that
 * :c:func:`read_wrap_fp` has been called (and returned a success code) and
 * attached a read-mode file pointer to the given wrapper file struct.
 *
 * **Intended for internal use only!**
 *
 * :param wrap: wrapper file structure to read header info from
 * :return: 0 on success, otherwise, an error code
 */
int read_wrap_hdr(WRAPPER_FILE *wrap);

/**
 * Reads metadata from the given wrapper file struct's on-disk file.  Assumes
 * that :c:func:`read_wrap_fp` and :c:func:`read_wrap_hdr` have both already
 * been called (and returned a success code).  This function then reads the
 * characters from the metadata section and stores it in the wrapper's metadata
 * buffer.
 *
 * :param wrap: wrapper file structure to operate on
 * :return: 0 on success, otherwise, an error code
 */
int read_metadata(WRAPPER_FILE *wrap);

/**
 * Reads contents from the given wrapper file struct's on-disk file.  Assumes
 * that :c:func:`read_wrap_fp` and :c:func:`read_wrap_hdr` have both already
 * been called (and returned a success code).  Does not require metadata to be
 * read first, though recommended.  This function then reads the characters
 * from the contents section and stores it in the wrapper's contents buffer.
 * It then maeks a call to the XML parser and stores the resulting document
 * pointer in the wrapper structure as well.
 *
 * .. todo::
 *    verify this kconfig link working?  VV
 *
 * The Kconfig option :kconfig:option:`CONFIG_TVWI_INVALID_XML_IS_FATAL`
 * decides the behavior of this function when XML parsing fails.  It can either
 * emit a warning or error-level message to the console, and either return an
 * error code or not.  If execution continues, :c:var:`WRAPPER_FILE.xmlerr` is
 * set on the ``wrap`` argument with the appropriate pointer.
 *
 * :param wrap: wrapper file structure to operate on
 * :return: 0 on success, otherwise, an error code
 */
int read_contents(WRAPPER_FILE *wrap);

