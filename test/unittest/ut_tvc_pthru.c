/* tvcompression passthru unit tests
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-26
 *
 * Unit tests for tvcompression passthru functions.
 */

#include "tvcompression/tvcompression.h"
#include "config.h"

// the section we need for either or
#if defined(CONFIG_TVCALG_NOCOMPRESSION) || defined(CONFIG_TVCALG_TABLETEST)

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "tvcompression/comp_passthru.h"

struct tvctestparams {
    size_t realinsize;
    char *realinput;
    size_t expoutsize;
    char *expoutput;
    int expreturn;
};

char *sd(const char *str) {
    char *out = cr_malloc(strlen(str) + 1);
    strcpy(out, str);
    return out;
}

#endif // CONFIG_TVCALG_NOCOMPRESSION || CONFIG_TVCALG_TABLETEST

Test(tvc_algos, nocompression) {
#ifdef CONFIG_TVCALG_NOCOMPRESSION
    struct COMPRESSION_ALGO *plain = lookup_algo_by_id(0);

    cr_expect(plain->id == 0,
            "expected plain ID 0, got %d -- wrong algo!!", plain->id);
    cr_expect(plain->compress == &(passthru_func),
            "nocomp has wrong compression function!");
    cr_expect(plain->decompress == &(passthru_func),
            "nocomp has wrong decompression function!");
    cr_expect_str_eq(plain->human_name, "No compression",
            "wrong human-readable name for nocompression algo");
#else
    // fail builds that don't allow uncompressed data
    cr_assert(0,
            "no-compression passthru has been disabled!!");
#endif
}

#ifdef CONFIG_TVCALG_TABLETEST
// necessary header file is tvcompression/comp_passthru.c -- already included
// unconditionally at the top (because it also contains the nocompression
// functions)
Test(tvc_algos, tabletest) {
    struct COMPRESSION_ALGO *algo = lookup_algo_by_id(1);

    cr_expect(algo->id == 1,
            "table test algorithm has wrong ID %d", algo->id);
    cr_expect(algo->compress == &(effectivity_test_comp),
            "tabletest has wrong compression function!");
    cr_expect(algo->decompress == &(effectivity_test_decomp),
            "tabletest has wrong decompression function!");
    cr_expect_str_eq(algo->human_name, "Compression table test",
            "tabletest has wrong human name!");
}

Test(tvc_funcs, compress) {
    size_t insize = 5;
    char *input = "Hello";
    size_t outsize;
    char *output;
    size_t expoutsize = 5;
    char *expout = "Ifmmp";

    int err = compress(1, &insize, &input, &outsize, &output);

    cr_expect(err == 0,
            "generic compress() returned err %d", err);
    cr_expect(outsize == expoutsize,
            "generic compress() output wrong size (%zu != %zu)",
            outsize, expoutsize);
    cr_expect_str_eq(output, expout,
            "generic compress() output wrong (%s VS %s)", input, output);
}

Test(tvc_funcs, decompress) {
    size_t insize = 5;
    char *input = "Ifmmp";
    size_t outsize;
    char *output;
    size_t expoutsize = 5;
    char *expout = "Hello";

    int err = decompress(1, &insize, &input, &outsize, &output);

    cr_expect(err == 0,
            "generic compress() returned err %d", err);
    cr_expect(outsize == expoutsize,
            "generic compress() output wrong size (%zu != %zu)",
            outsize, expoutsize);
    cr_expect_str_eq(output, expout,
            "generic compress() output wrong (%s VS %s)", input, output);
}
#endif

#ifdef CONFIG_TVCALG_NOCOMPRESSION

ParameterizedTestParameters(tvcpthru, plain) {
    static struct tvctestparams params[] = {
        {5, "Hello", 5, "Hello", 0},
        {5, "World", 5, "World", 0},
        {13, "Hello, world!", 13, "Hello, world!", 0},
    };

    size_t nb_params = sizeof(params) / sizeof(struct tvctestparams);

    for (size_t i=0; i < nb_params; i++) {
        params[i].realinput = sd(params[i].realinput);
        params[i].expoutput = sd(params[i].expoutput);
    }

    return cr_make_param_array(struct tvctestparams, params, nb_params);
}

ParameterizedTest(struct tvctestparams *param, tvcpthru, plain) {
    size_t realoutsize;
    char *realoutput;
    int realreturn;

    cr_assert_arr_eq(param->realinput, param->expoutput, param->realinsize,
            "asking wrong thing of function!");

    realreturn = passthru_func(
            &(param->realinsize),
            &(param->realinput),
            &realoutsize,
            &realoutput);

    cr_expect(realreturn == param->expreturn,
            "expected return val %d, got %d!", param->expreturn, realreturn);
    cr_expect(realoutsize == param->expoutsize,
            "expected %zu out size, got %zu!", param->expoutsize, realoutsize);
    cr_expect_arr_eq(realoutput, param->expoutput, realoutsize,
            "got wrong expected output!");
}

#endif  // ifdef CONFIG_TVCALG_NOCOMPRESSION

#ifdef CONFIG_TVCALG_TABLETEST

ParameterizedTestParameters(tvcpthru, ttcomp) {
    static struct tvctestparams params[] = {
        {5, "Hello", 5, "Ifmmp", 0},
        {5, "World", 5, "Xpsme", 0},
        {13, "Hello, World!", 13, "Ifmmp-!Xpsme\"", 0},
    };

    size_t nb_params = sizeof(params) / sizeof(struct tvctestparams);

    for (size_t i=0; i < nb_params; i++) {
        params[i].realinput = sd(params[i].realinput);
        params[i].expoutput = sd(params[i].expoutput);
    }

    return cr_make_param_array(struct tvctestparams, params, nb_params);
}

ParameterizedTest(struct tvctestparams *param, tvcpthru, ttcomp) {
    size_t realoutsize;
    char *realoutput;
    int realreturn;

    cr_assert_arr_neq(param->realinput, param->expoutput, param->realinsize,
            "asking wrong thing of function!");

    realreturn = effectivity_test_comp(
            &(param->realinsize),
            &(param->realinput),
            &realoutsize,
            &realoutput);

    cr_expect(realreturn == param->expreturn,
            "expected return val %d, got %d!", param->expreturn, realreturn);
    cr_expect(realoutsize == param->expoutsize,
            "expected %zu out size, got %zu!", param->expoutsize, realoutsize);
    cr_expect_arr_eq(realoutput, param->expoutput, realoutsize,
            "got wrong expected output!");
}

ParameterizedTestParameters(tvcpthru, ttdecomp) {
    static struct tvctestparams params[] = {
        {5, "Ifmmp", 5, "Hello", 0},
        {5, "Xpsme", 5, "World", 0},
        {13, "Ifmmp-!Xpsme\"", 13, "Hello, World!", 0},
    };

    size_t nb_params = sizeof(params) / sizeof(struct tvctestparams);

    for (size_t i=0; i < nb_params; i++) {
        params[i].realinput = sd(params[i].realinput);
        params[i].expoutput = sd(params[i].expoutput);
    }

    return cr_make_param_array(struct tvctestparams, params, nb_params);
}

ParameterizedTest(struct tvctestparams *param, tvcpthru, ttdecomp) {
    size_t realoutsize;
    char *realoutput;
    int realreturn;

    cr_assert_arr_neq(param->realinput, param->expoutput, param->realinsize,
            "asking wrong thing of function!");

    realreturn = effectivity_test_decomp(
            &(param->realinsize),
            &(param->realinput),
            &realoutsize,
            &realoutput);

    cr_expect(realreturn == param->expreturn,
            "expected return val %d, got %d!", param->expreturn, realreturn);
    cr_expect(realoutsize == param->expoutsize,
            "expected %zu out size, got %zu!", param->expoutsize, realoutsize);
    cr_expect_arr_eq(realoutput, param->expoutput, realoutsize,
            "got wrong expected output!");
}

#endif  // ifdef CONFIG_TVCALG_NOCOMPRESSION

