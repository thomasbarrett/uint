#ifndef UINT_H
#define UINT_H

#include <stdint.h>

#define N 8 // the number of integer limbs in a big_uint
#define uint_t uint32_t // single precision unsigned integer
#define d_int_t int64_t // double precision signed integer
#define d_uint_t uint64_t // double precision unsigned integer

#define LIMB_BITS (8 * sizeof(uint_t))

/**
 * @brief An arbitrary precision unsigned integer.
 * 
 */
typedef struct big_uint {
    uint_t limbs[N];
} big_uint_t;

/**
 * @brief Parse a big_uint_t into the `res` parameter. Return a value 
 *        indicating success or failed.
 * 
 * @param str the string to parse
 * @return -2 if incomplete
 *         -1 in invalid
 *          n if successful (number of characters consumed)
 */
int parse_uint(const char *str, big_uint_t *res);

/**
 * @brief Parse and return a big_uint_t from the given string or exit.
 * 
 * @param str the string to parse
 * @return the parsed big_uint_t 
 */
big_uint_t must_parse_uint(const char *str);

/**
 * @brief Print a big_uint_t to stdout.
 * 
 * @param x the big_uint_t
 */
void big_uint_print(big_uint_t x);

/**
 * @brief Compares two uint_t values and returns an integer value indicating
 *        their relative ordering.
 * 
 * @param a the first uint
 * @param b the second uint
 * @return n > 0 if a > b
 *         n = 0 if a == b
 *         n < 0 if a < b.
 */
int big_uint_cmp(const big_uint_t *a, const big_uint_t *b);

/**
 * @brief Return the sum of two bit_uint_t values.
 * 
 * @param a the first uint
 * @param b the second uint
 * @return a + b.
 */
big_uint_t big_uint_add(const big_uint_t *a, const big_uint_t *b);

/**
 * @brief Return the difference of two bit_uint_t values.
 * 
 * @param a the first uint
 * @param b the second uint
 * @return a - b.
 */
big_uint_t big_uint_sub(const big_uint_t *a, const big_uint_t *b);

#endif /* UINT_H */