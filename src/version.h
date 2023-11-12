/* TagVFS version information.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-09-24
 *
 * Version info & strings.
 */

#pragma once

#include "version_autogen.h"

/**
 * Program version for use in ``argp``.  Precisely equivalent to
 * :py:const:`tv_version.VERSION_EXTRA`.
 */
const char *argp_program_version = VERSION_EXTRA;

/**
 * Bug submission address for use in ``argp``.  Precisely equivalent to
 * :py:const:`tv_version.VERSION_HOMEPAGE`.
 */
const char *argp_program_bug_address = VERSION_HOMEPAGE;


