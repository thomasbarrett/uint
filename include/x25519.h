#include <uint.h>

/**
 * @brief Given a scalar `s` in the range [0, 2^256) the clamp function
 * sets three least significant bits of `s` to 0 so that the new scalar is
 * a multiple of 8. This ensures that for any clamped scalar s and
 * group element P, s * P is an element in the Curve25519 subgroup of
 * large prime order. Note that the co-factor of the large prime-order
 * subgroup is 8, so for any element not in the subgroup, s * P = 0. * 
 * 
 * Additionally, this function sets the most significant bit to 0 and the
 * second most significant bit to 1. This ensures that the scalar value is
 * between 2^254 and 2^255 - 1 which is in the valid range [0, q). Since
 * the clamped scalar is never 0, for base point P in the large prime-order
 * subgroup, the value s * P will never equal 0.
 * 
 * @param s the scalar to clamp.
 */
void x25519_clamp(uint8_t s[32]);

typedef struct x25519_element {
    uint_t x[2 * N];
    uint_t z[2 * N];
} x25519_element_t;

void x25519_init();

/**
 * @brief Compute the sum of two x25519 group elements given their difference.
 * 
 * @param a first group element
 * @param b second group element
 * @param d the difference b - a
 * @param res the sum a + b
 */
void x25519_add(x25519_element_t *a, x25519_element_t *b, x25519_element_t *d, x25519_element_t *res);

/**
 * @brief Compute the doubling formula for the x25519 group element e
 * 
 * @param e 
 * @param res 
 */
void x25519_double(x25519_element_t *e, x25519_element_t *res);

void x25519_scalar_mult(uint_t *s, x25519_element_t *e, uint_t *res);
