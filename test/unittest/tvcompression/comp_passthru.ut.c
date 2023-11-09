/* tvcompression comp_passthru unit tests
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-11-04
 *
 * Unit tests for compression table passthru functions.
 */

#include "../unittest.h"
#include "tvcompression/tvcompression.h"

// only need to include / define these things if we're going to be using them
// (but do so for either algo)
#if defined(CONFIG_TVCALG_NOCOMPRESSION) || defined(CONFIG_TVCALG_TABLETEST)

#include "tvcompression/comp_passthru.h"

/**
 * Struct to hold parameters of a test.
 *
 * Since only one parameter can be passed into a parameterized unit test, that
 * one parameter will be this struct, containing all the other necessary data.
 */
struct tvctestparams {
	size_t decpsize;
    char *decompress;
    size_t compsize;
    char *compress;
    int expreturn;
};

#endif // CONFIG_TVCALG_NOCOMPRESSION || CONFIG_TVCALG_TABLETEST

#ifdef CONFIG_TVCALG_NOCOMPRESSION

/**
 * Test parameters for the passthru (no compression) function.
 */
ParameterizedTestParameters(tvc_algo_passthru, plain) {
    static struct tvctestparams params[] = {
        // a few normal test cases
        {5, "Hello", 5, "Hello", 0},
        {5, "World", 5, "World", 0},
        {13, "Hello, world!", 13, "Hello, world!", 0},

        // make sure newlines and trailing spaces work
        {12, "Hello\nworld!", 12, "Hello\nworld!", 0},
        {15, "Hello, world!  ", 15, "Hello, world!  ", 0},

        // what if we lie to it and put null chars in the middle of strings?
        {15, "Hello, world!\0A", 15, "Hello, world!\0A", 0},
        {8, "\0\0\0\0\0\0\0\0", 8, "\0\0\0\0\0\0\0\0", 0},
        {8, "\0\0\0AA\0\0\0", 8, "\0\0\0AA\0\0\0", 0},
    };

    size_t nb_params = sizeof(params) / sizeof(struct tvctestparams);

    for (size_t i=0; i < nb_params; i++) {
        params[i].decompress = crs(params[i].decompress, params[i].decpsize);
        params[i].compress = crs(params[i].compress, params[i].compsize);
    }

    return cr_make_param_array(struct tvctestparams, params, nb_params);
}

/**
 * Test body for passthru (no compression) function.
 *
 * This is run once for each entry in the previous ParameterizedTestParameters
 * declared.  Params are provided in the `param` pointer.
 *
 * Because this algorithm is defined to be the same for both compression and
 * decompression (no change), we only need to test in one direction here.
 */
ParameterizedTest(struct tvctestparams *param, tvc_algo_passthru, plain) {
	size_t realoutsize;
    char *realoutput;
    int realreturn;

    realreturn = passthru_func(
            &(param->decpsize),
            &(param->decompress),
            &realoutsize,
            &realoutput);

    cr_expect(eq(int, realreturn, param->expreturn),
            "did not get expected return value from passthru_func!");
    cr_expect(eq(sz, realoutsize, param->compsize),
            "did not get expected output size from passthru_func!");
    cr_expect(eq(chr[realoutsize], realoutput, param->compress),
            "did not get expected output from passthru_func!");
}

#endif  // CONFIG_TVCALG_NOCOMPRESSION

#ifdef CONFIG_TVCALG_TABLETEST

/**
 * Test parameters for the table-test algorithm (1-shift caesar cipher).
 */
ParameterizedTestParameters(tvc_algo_tabletest, algo) {
    static struct tvctestparams params[] = {
        // a few normal test cases
        {5, "Hello", 5, "Ifmmp", 0},
        {5, "World", 5, "Xpsme", 0},
        {13, "Hello, world!", 13, "Ifmmp-!xpsme\"", 0},

        // make sure newlines and trailing spaces work
        {12, "Hello\nworld!", 12, "Ifmmp\vxpsme\"", 0},
        {15, "Hello, world!  ", 15, "Ifmmp-!xpsme\"!!", 0},

        // what if we lie to it and put null chars in the middle of strings?
        {15, "Hello, world!\0A", 15, "Ifmmp-!xpsme\"\1B", 0},
        {8, "\0\0\0\0\0\0\0\0", 8, "\1\1\1\1\1\1\1\1", 0},
        {8, "\0\0\0AA\0\0\0", 8, "\1\1\1BB\1\1\1", 0},
    };

    size_t nb_params = sizeof(params) / sizeof(struct tvctestparams);

    for (size_t i=0; i < nb_params; i++) {
        params[i].decompress = crs(params[i].decompress, params[i].decpsize);
        params[i].compress = crs(params[i].compress, params[i].compsize);
    }

    return cr_make_param_array(struct tvctestparams, params, nb_params);
}

/**
 * Test body for the table-test algorithm.
 *
 * This is run once for each entry in the ParameterizedTestParameters
 * previously declared.
 *
 * Because this operation is symmetric (that is, x == decompress(compress(x))),
 * we can use the same set of test parameters and run both the compression and
 * decompression algorithm on each test case.
 */
ParameterizedTest(struct tvctestparams *param, tvc_algo_tabletest, algo) {
	size_t realoutsize;
    char *realoutput;
    int realreturn;

    realreturn = effectivity_test_comp(
            &(param->decpsize),
            &(param->decompress),
            &realoutsize,
            &realoutput);

    cr_expect(eq(int, realreturn, param->expreturn),
            "did not get expected return value from effectivity_test_comp!");
    cr_expect(eq(sz, realoutsize, param->compsize),
            "did not get expected output size from effectivity_test_comp!");
    cr_expect(eq(chr[realoutsize], realoutput, param->compress),
            "did not get expected output from effectivity_test_comp!");

    char *realoutput2;

    realreturn = effectivity_test_decomp(
            &(param->compsize),
            &(param->compress),
            &realoutsize,
            &realoutput2);

    cr_expect(eq(int, realreturn, param->expreturn),
            "did not get expected return value from effectivity_test_decomp!");
    cr_expect(eq(sz, realoutsize, param->decpsize),
            "did not get expected output size from effectivity_test_decomp!");
    cr_expect(eq(chr[realoutsize], realoutput2, param->decompress),
            "did not get expected output from effectivity_test_decomp!");

}

#endif // CONFIG_TVCALG_TABLETEST

