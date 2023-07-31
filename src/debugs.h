/* Debugging printf-style macros.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-22
 *
 * Defines a series of macros that can be used based on the Kconfig-set
 * debugging level.
 *
 * The macros in this file are defined such that the compiler always sees the
 * code, even if we know at preprocessor time that we don't want a given
 * statement.  The reasoning for this is from this excellent S/O answer:
 * https://stackoverflow.com/a/1644898
 */


#pragma once

/**
 * Log a message to specified buffer when ``LOG_LEVEL`` (from menuconfig) is equal to or greater than the specified level.
 *
 * Not really intended for general use; see the TV_LOGE/W/I/D/V macros below.
 *
 * :param lvl: Log level to activate at (int).
 * :param buf: buffer to write to (stdout, stderr, etc)
 */
#define TV_LOG_LEVEL(lvl, buf, ...) \
    do { \
        if (CONFIG_LOG_LEVEL >= lvl) { \
            fprintf(buf, __VA_ARGS__); \
        } \
    } while (0)

//#define TV_LOGN(...) TV_LOG_LEVEL(0, __VA_ARGS__)

/**
 * Log an error message to stderr.  Accepts printf-like arguments, but only does anything when TV_LOG_ERR or greater is set via menuconfig.
 */
#define TV_LOGE(...) TV_LOG_LEVEL(1, stderr, __VA_ARGS__)

/**
 * Log a warning message to stderr.  Accepts printf-like arguments, but only does anything when TV_LOG_WARN or greater is set via menuconfig.
 */
#define TV_LOGW(...) TV_LOG_LEVEL(2, stderr, __VA_ARGS__)

/**
 * Log an information message to stdout.  Accepts printf-like arguments, but only does anything when TV_LOG_INFO or greater is set via menuconfig.
 */
#define TV_LOGI(...) TV_LOG_LEVEL(3, stdout, __VA_ARGS__)

/**
 * Log a debug message to stdout.  Accepts printf-like arguments, but only does anything when TV_LOG_DEBUG or greater is set via menuconfig.
 */
#define TV_LOGD(...) TV_LOG_LEVEL(4, stdout, __VA_ARGS__)

/** Log a verbose debug message to stdout.  Accepts printf-like arguments, but only does anything when TV_LOG_VERbOSE is set via menuconfig.
 */
#define TV_LOGV(...) TV_LOG_LEVEL(5, stdout, __VA_ARGS__)

