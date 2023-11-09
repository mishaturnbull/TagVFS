/* Unit testing info printer
 * AGPL-3.0 license
 * Initial rev 2023-11-08 Misha Turnbull
 *
 * Ensures that unit test output snapshots version info.
 */

#include "unittest.h"
#include "version.h"

/**
 * Report hook to ensure we print version info.
 *
 * This serves no other purpose, nor tests anything.  The message is always
 * logged at error level in order to ensure it is unconditionally output.
 */
ReportHook(PRE_ALL)(struct criterion_test_set *cts) {
    cr_log_error("TagVFS version info: " VERSION_EXTRA);
    cr_log_info("all = %p", (void*)cts);
}

