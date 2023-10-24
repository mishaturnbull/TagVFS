/* TagVFS compression library header
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 *
 * Headers & declarations for TagVFS compression entry.
 */

#include "compression_table.h"

/**
 * Look up the requested algorithm ID and return its structure.
 *
 * This function identifies the compression algorithm structure with the
 * specified ID, then returns a pointer to it.
 *
 * If no such algorithm is available, a warning message is emitted and the
 * *first* entry in the table is returned.  You may detect this condition as
 * the caller by assessing the returned structure's
 * :c:var:`COMPRESSION_ALGO.id` member is equal to the ``algo`` parameter
 * provided; if not, the requested algorithm was unavailable.
 *
 * :param algo: algorithm ID to look up
 * :return: the compression algo struct with the given ID
 */
struct COMPRESSION_ALGO * lookup_algo_by_id(uint16_t algo);

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
 *    (compressed).  This will be ALLOCATED AND WRITTEN by the function (output
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
 *    (decompressed).  This will be ALLOCATED AND WRITTEN by the function
 *    (output parameter).  Its length is returned via the ``outsize`` parameter.
 *    The caller is responsible for freeing this buffer when no longer needed.
 * :return: Error code, or 0 on success.
 */
int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

