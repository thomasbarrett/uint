#ifndef MOD_H
#define MOD_H

/**
 * @brief Implementation for a Galois field of prime order GF(p)
 * @author Thomas Barrett
 */

#include <uint.h>

/**
 * @brief Computes `a` + `b` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the first element
 * @param b the second element
 * @param c the sum `a` + `b` mod `p`
 * @param n the number of limbs
 */
void mod_add(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes `a` - `b` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the first element
 * @param b the second element
 * @param c the sum `a` - `b` mod `p`
 * @param n the number of limbs
 */
void mod_sub(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes `a` * `b` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the first element
 * @param b the second element
 * @param c the sum `a` * `b` mod `p`
 * @param n the number of limbs
 */
void mod_mul(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

void mod_pow(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n);

/**
 * @brief Computes the inverse of `a` mod `p`.
 * 
 * @param p the prime modulo
 * @param a the element
 * @param c the inverse of `a`
 * @param n the number of limbs
 */
void mod_inv(const uint_t *p, const uint_t *a, uint_t *b, size_t n);

#endif /* MOD_H */