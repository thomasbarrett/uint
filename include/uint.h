#ifndef UINT_H
#define UINT_H

#include <stdint.h>
#include <stddef.h>

#define N 8 // the number of integer limbs in a big_uint
#define uint_t uint32_t // single precision unsigned integer
#define d_int_t int64_t // double precision signed integer
#define d_uint_t uint64_t // double precision unsigned integer

#define LIMB_BITS (8 * sizeof(uint_t))

/**
 * @brief Parse a big_uint_t into the `res` parameter. Return a value 
 *        indicating success or failed.
 * 
 * @param str the string to parse
 * @return -2 if incomplete
 *         -1 in invalid
 *          n if successful (number of characters consumed)
 */
int parse_uint(const char *str, uint_t *res, size_t n);

/**
 * @brief Parse and return a big_uint_t from the given string or exit.
 * 
 * @param str the string to parse
 * @return the parsed big_uint_t 
 */
void must_parse_uint(const char *str, uint_t *res, size_t n);

/**
 * @brief Print a big_uint_t to stdout.
 * 
 * @param x the big_uint_t
 */
void big_uint_print(const uint_t *x, size_t n);

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
int big_uint_cmp(const uint_t *a, const uint_t *b, size_t n);

/**
 * @brief Assign the param `c` to be the sum of `a` and `b`.
 * 
 * @param a the first uint
 * @param b the second uint
 * @param c the result `a` + `b`
 */
void big_uint_add(const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Assign the param `a` to be the difference of `a` and `b`.
 * 
 * @param a the first uint
 * @param b the second uint
 * @param c the result `a` - `b`
 */
void big_uint_sub(const uint_t *a, const uint_t *b, uint_t *c, size_t n);

#endif /* UINT_H */