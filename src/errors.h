/* TagVFS error codes definitions.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-22
 *
 * Defines error codes used in TagVFS (in addition to errno).
 */

#pragma once

#define TV_NO_ERR           CONFIG_TV_ERR_START
#define TV_CHK_ERRNO        CONFIG_TV_ERR_START + 1

// tvwio read errors
#define TVW_ERR_INV_HDR     CONFIG_TV_ERR_START + 2
#define TVW_ERR_INV_META    CONFIG_TV_ERR_START + 3 
#define TVW_ERR_INV_CONT    CONFIG_TV_ERR_START + 4 
#define TVW_ERR_INV_MULT    CONFIG_TV_ERR_START + 5
#define TVW_ERR_INV_SOCGS   CONFIG_TV_ERR_START + 6

