/* TagVFS error codes definitions.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-22
 *
 * Defines error codes used in TagVFS (in addition to errno).
 */

#pragma once

/** No errors to report.  All is well!
 *
 * This value is equivalent to :kconfig:option:`CONFIG_TV_ERR_START`.
 */
#define TV_NO_ERR           CONFIG_TV_ERR_START

/**
 * An error has occurred, but the recipient of this code should further check
 * the system ``errno`` for more information.
 *
 * Equal to :kconfig:option:`CONFIG_TV_ERR_START` plus 1.
 */
#define TV_CHK_ERRNO        CONFIG_TV_ERR_START + 1

// tvwio read errors

/** tvwio invalid header */
#define TVW_ERR_INV_HDR     CONFIG_TV_ERR_START + 2

/** tvwio invalid metadata */
#define TVW_ERR_INV_META    CONFIG_TV_ERR_START + 3 

/** tvwio invalid file contents */
#define TVW_ERR_INV_CONT    CONFIG_TV_ERR_START + 4 

/** tvwio reports problems in multiple places */
#define TVW_ERR_INV_MULT    CONFIG_TV_ERR_START + 5

/** tvwio start-of-contents GS error */
#define TVW_ERR_INV_SOCGS   CONFIG_TV_ERR_START + 6

