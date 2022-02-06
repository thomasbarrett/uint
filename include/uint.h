#ifndef UINT_H
#define UINT_H

#include <stdint.h>
#include <stddef.h>

#define N 8 // the number of integer limbs in a big_uint
#define uint_t uint32_t // single precision unsigned integer
#define d_int_t int64_t // double precision signed integer
#define d_uint_t uint64_t // double precision unsigned integer

#define LIMB_BITS (8 * sizeof(uint_t))
#define LIMB_BITS_LOG2 5

/**
 * @brief Parse a big_uint_t into the `res` parameter. Return a value 
 *        indicating success or failed.
 * 
 * @param str the string to parse
 * @param n the number of limbs in x
 * @return -2 if incomplete
 *         -1 in invalid
 *          n if successful (number of characters consumed)
 */
int parse_uint(const char *str, uint_t *res, size_t n);

/**
 * @brief Parse and return a big_uint_t from the given string or exit.
 * 
 * @param str the string to parse
 * @param n the number of limbs in x
 * @return the parsed big_uint_t 
 */
void must_parse_uint(const char *str, uint_t *res, size_t n);

/**
 * @brief Print a big_uint_t to stdout.
 * 
 * @param x the big_uint_t
 * @param n the number of limbs in x
 */
void uint_print(const uint_t *x, size_t n);

/**
 * @brief Compares two uint_t values and returns an integer value indicating
 *        their relative ordering.
 * 
 * @param a the first uint
 * @param b the second uint
 * @param n the number of limbs in a and b
 * @return n > 0 if a > b
 *         n = 0 if a == b
 *         n < 0 if a < b.
 */
int uint_cmp(const uint_t *a, const uint_t *b, size_t n);

/**
 * @brief Assign the param `c` to be the sum of `a` and `b`.
 * 
 * @param a the first uint
 * @param b the second uint
 * @param c the result `a` + `b`
 * @param n the number of limbs in a, b, and c
 */
void uint_add(const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Assign the param `a` to be the difference of `a` and `b`.
 * 
 * @param a the first uint
 * @param b the second uint
 * @param c the result `a` - `b`
 * @param n the number of limbs in a, b, and c
 */
void uint_sub(const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Assign the param `c` to be the product of `a` and `b`. Note
 *        that the number of limbs in `c` is assumed to be 2 * `n`
 * 
 * @param a the first uint
 * @param b the second uint
 * @param c the result `a` + `b`
 * @param n the number of limbs in a and b
 */
void uint_mul(const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes the integer division a / b and assigns the result to the
 * quotient `q` and remainder `r` parameters.
 * 
 * @param a numerator
 * @param b denominator
 * @param q quotient
 * @param r remainer
 * @param n number of limbs
 */
void uint_div(const uint_t *a, const uint_t *b, uint_t *q, uint_t *r, size_t n);

/**
 * @brief Shifts the uint `a` left by `b` limbs and stores the result
 * in `c`.
 * 
 * @param a input uint
 * @param b the number of limbs to shift by
 * @param c output uint
 * @param n the number of limbs in `a` and `c`.
 */
void uint_shl_limb(const uint_t *a, uint_t b, uint_t *c, size_t n);

/**
 * @brief Shifts the uint `a` left by 1 bit and stores the result
 * in `c`.
 * 
 * @param a input uint
 * @param b ouput uint
 * @param n the number of limbs in `a` and `b`.
 */
void uint_shl_one(const uint_t *a, uint_t *b, size_t n);

/**
 * @brief Shifts the uint `a` right by `b` limbs and stores the result
 * in `c`.
 * 
 * @param a input uint
 * @param b the number of limbs to shift by
 * @param c output uint
 * @param n the number of limbs in `a` and `c`.
 */
void uint_shr_limb(const uint_t *a, uint_t b, uint_t *c, size_t n);

/**
 * @brief Shifts the uint `a` right by 1 bit and stores the result
 * in `c`.
 * 
 * @param a input uint
 * @param b ouput uint
 * @param n the number of limbs in `a` and `b`.
 */
void uint_shr_one(const uint_t *a, uint_t *b, size_t n);

#endif /* UINT_H */