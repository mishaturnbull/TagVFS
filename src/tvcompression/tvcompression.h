/* TagVFS compression library header
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 *
 * Headers & declarations for TagVFS compression entry.
 */

#include "compression_table.h"

/**
 * Generic transparent compression function.
 *
 * Given an algorithm ID and necessary arguments, invokes the specified
 * algorithm's compression function.  This should be the main (and only!) call
 * to compress data for TagVFS wrapper files.
 *
 * :param algo: ID of the compression algorithm to utilize for this operation.
 * :param insize: Pointer to size of the input data (uncompressed).  This will
 *    not be altered by the function (input parameter).
 * :param inbuf: Pointer to the character array of the input data
 *    (uncompressed).  This will not be altered by the function (input 
 *    parameter).
 * :param outsize: Pointer to the size of the output data (compressed).  This
 *    will be SET by the function (output parameter).
 * :param outbuf: Pointer to the character array of the output data buffer
 *    (compressed).  This will be INITIALIZED AND WRITTEN by the function (output
 *    parameter).  Its length is returned via the ``outsize`` parameter.  The
 *    caller is responsible for freeing this buffer when no longer needed.
 * :return: Error code, or 0 on success.
 */
int compress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

/**
 * Generic transparent decompression function.
 *
 * Given an algorithm ID and necessary arguments, invokes the specified
 * algorithm's decompression function.  This should be the main (and only!)
 * call to decompress data for TagVFS wrapper files.
 *
 * :param algo: ID of the decompression algorithm to utilize for this operation.
 * :param insize: Pointer to size of the input data (compressed).  This will
 *    not be altered by the function (input parameter).
 * :param inbuf: Pointer to the character array of the input data
 *    (compressed).  This will not be altered by the function (input 
 *    parameter).
 * :param outsize: Pointer to the size of the output data (decompressed).  This
 *    will be SET by the function (output parameter).
 * :param outbuf: Pointer to the character array of the output data buffer
 *    (decompressed).  This will be INITIALIZED AND WRITTEN by the function
 *    (output parameter).  Its length is returned via the ``outsize`` parameter.
 *    The caller is responsible for freeing this buffer when no longer needed.
 * :return: Error code, or 0 on success.
 */
int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

