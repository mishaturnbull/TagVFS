/* Unittesting code for hashing.c functions.
 * AGPL-3.0 license
 * Initial rev 2023-11-07 Misha Turnbull
 *
 * This file contains unit tests for the hashing.c functions.
 */

#include "../unittest.h"
#include "tvwio/hashing.h"

/**
 * Struct to hold parameters of a test for hashing a string.
 *
 * Since only one parameter can be passed into a parameterized unit test, that
 * one parameter will be this struct, containing all the other necessary data.
 */
struct hashtestparam {
    char *input;
    size_t insize;
    uint8_t hash[64];
};

/**
 * Test parameters for the passthru (no compression) function.
 */
ParameterizedTestParameters(tvwio_hashing, hash_of) {
    static struct hashtestparam params[] = {
        {"Hello, world!", 13, {
            0xc1, 0x52, 0x7c, 0xd8, 0x93, 0xc1, 0x24, 0x77,
            0x3d, 0x81, 0x19, 0x11, 0x97, 0x0c, 0x8f, 0xe6,
            0xe8, 0x57, 0xd6, 0xdf, 0x5d, 0xc9, 0x22, 0x6b,
            0xd8, 0xa1, 0x60, 0x61, 0x4c, 0x0c, 0xd9, 0x63,
            0xa4, 0xdd, 0xea, 0x2b, 0x94, 0xbb, 0x7d, 0x36,
            0x02, 0x1e, 0xf9, 0xd8, 0x65, 0xd5, 0xce, 0xa2,
            0x94, 0xa8, 0x2d, 0xd4, 0x9a, 0x0b, 0xb2, 0x69,
            0xf5, 0x1f, 0x6e, 0x7a, 0x57, 0xf7, 0x94, 0x21}},

        {"goodbye, world! 12345", 21, {
			0x90, 0x6a, 0x08, 0x38, 0x19, 0xb4, 0x47, 0x51,
			0x75, 0x5d, 0xb8, 0x7a, 0x83, 0x9c, 0x1c, 0x4f,
			0xd4, 0xc5, 0x7e, 0x92, 0x3e, 0xb2, 0x56, 0x8e,
			0xce, 0xe0, 0xaf, 0xb0, 0x05, 0x39, 0xa7, 0xf6,
			0x84, 0xc1, 0x87, 0x5f, 0x45, 0x37, 0xd2, 0x51,
			0x91, 0xb6, 0x88, 0x6b, 0xb9, 0x12, 0x67, 0x5c,
			0x80, 0x8d, 0xd6, 0xc6, 0x6b, 0xad, 0x3a, 0xb0,
			0x43, 0x88, 0x3c, 0x4c, 0x8b, 0x0f, 0x33, 0xcf}},

		{"\0\0\0\0", 4, {
            0xec, 0x2d, 0x57, 0x69, 0x1d, 0x9b, 0x2d, 0x40,
            0x18, 0x2a, 0xc5, 0x65, 0x03, 0x20, 0x54, 0xb7,
            0xd7, 0x84, 0xba, 0x96, 0xb1, 0x8b, 0xcb, 0x5b,
            0xe0, 0xbb, 0x4e, 0x70, 0xe3, 0xfb, 0x04, 0x1e,
            0xff, 0x58, 0x2c, 0x8a, 0xf6, 0x6e, 0xe5, 0x02,
            0x56, 0x53, 0x9f, 0x21, 0x81, 0xd7, 0xf9, 0xe5,
            0x36, 0x27, 0xc0, 0x18, 0x9d, 0xa7, 0xe7, 0x5a,
            0x4d, 0x5e, 0xf1, 0x0e, 0xa9, 0x3b, 0x20, 0xb3}},

    };

    size_t nb_params = sizeof(params) / sizeof(struct hashtestparam);

    for (size_t i=0; i < nb_params; i++) {
        params[i].input = crs(params[i].input, params[i].insize);
    }

    return cr_make_param_array(struct hashtestparam, params, nb_params);
}

/**
 * Test body for the hash_of function that computes a given hash.
 *
 * This test performs a hash of the given string, and compares the result with an expected known-good value.
 */
ParameterizedTest(struct hashtestparam *param, tvwio_hashing, hash_of) {

	uint8_t *buf = hash_of((uint8_t*)(param->input), param->insize);

	cr_expect(eq(u8[64], buf, param->hash),
            "did not get expected hash value!");
}

/**
 * Struct to hold parameters of a test for hashing a string.
 *
 * Since only one parameter can be passed into a parameterized unit test, that
 * one parameter will be this struct, containing all the other necessary data.
 */
struct hasheqparam {
    char *a;
    size_t alen;
    char *b;
    size_t blen;
    int should_eq;
};

/**
 * Test parameters for hash comparisons.
 *
 * These parameters specify two strings, which are both hashed during the test
 * function.
 */
ParameterizedTestParameters(tvwio_hashing, cmp_hash) {
    static struct hasheqparam params[] = {
        {"Hello, world!", 13,
         "Hello, world!", 13,
         1},

        {"Goodbye, world! 12345", 21,
         "Goodbye, world! 12345", 21,
         1},

        {"\0\0\0\0", 4,
         "\0\0\0\0", 4,
         1},

        {"Hello, world!", 13,
         "Goodbye, world! 12345", 21,
         0},

        {"\0\0\0\0", 4,
         "\0\0\0\0\0\0\0\0", 8,
         0},

        {"\0\1\0\4", 4,
         "\0\1\0\1\0\1\0\4", 8,
         0},
    };

    size_t nb_params = sizeof(params) / sizeof(struct hasheqparam);

    for (size_t i=0; i < nb_params; i++) {
        params[i].a = crs(params[i].a, params[i].alen);
        params[i].b = crs(params[i].b, params[i].blen);
    }

    return cr_make_param_array(struct hasheqparam, params, nb_params);
}

/**
 * Test body for hash comparison.
 *
 * This test performs two hash operations on separate strings, then compares
 * them using the cmp_hashes function.  The result is compared to an expectant
 * result.
 */
ParameterizedTest(struct hasheqparam *param, tvwio_hashing, cmp_hash) {

    uint8_t *ahash = hash_of((uint8_t*)(param->a), param->alen);
    uint8_t *bhash = hash_of((uint8_t*)(param->b), param->blen);
    int act_eq = cmp_hashes(ahash, bhash);

    cr_expect(eq(int, act_eq, param->should_eq),
            "cmp_hashes differed from expected comparison result!");
}

