#ifndef MOD_H
#define MOD_H

/**
 * @brief Implementation for a Galois field of prime order GF(p)
 * @author Thomas Barrett
 */

#include <uint.h>

typedef struct gfp {
    uint_t p[2 * N];
    uint_t r[2 * N];
} gfp_t;

/**
 * @brief 
 * 
 * @param p A prime uint of size n
 * @param r The result of size n + 1
 * @param n The number of limbs
 */
void barrett_r(uint_t *p, uint_t *r, size_t n);

/**
 * @brief Computes `a` + `b` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the first element
 * @param b the second element
 * @param c the sum `a` + `b` mod `p`
 * @param n the number of limbs
 */
void gfp_add(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes `a` - `b` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the first element
 * @param b the second element
 * @param c the sum `a` - `b` mod `p`
 * @param n the number of limbs
 */
void gfp_sub(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes `a` * `b` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the first element
 * @param b the second element
 * @param c the sum `a` * `b` mod `p`
 * @param n the number of limbs
 */
void gfp_mul(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

void gfp_pow(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes the inverse of `a` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the element
 * @param c the inverse of `a`
 * @param n the number of limbs
 */
void gfp_inv(const uint_t *p, const uint_t *a, uint_t *b, size_t n);

#endif /* MOD_H */
