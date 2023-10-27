/* tvcompression passthru unit tests
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-26
 *
 * Unit tests for tvcompression passthru functions.
 */

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
