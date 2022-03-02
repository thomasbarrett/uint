#include <x25519.h>
#include <gfp.h>
#include <string.h>
#include <uint.h>

gfp_t curve25519;

const uint_t A[2 * N] = {
    0x00076D06, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

const uint_t xp[2 * N] = {
    0x00000009, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

void x25519_clamp(uint8_t s[32]) {
    s[0] &= 0xf8;
    s[31] = (s[31] & 0x7f) | 0x40;
}

void x25519_init() {
    uint_t p[2 * N] = {0};
    must_parse_uint("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed", p, N);
    curve25519 = gfp_init(p, N);
}

void x25519_add(x25519_element_t *a, x25519_element_t *b, x25519_element_t *d, x25519_element_t *res) {
    uint_t xaxb[2 * N] = {0};
    uint_t zazb[2 * N] = {0};
    uint_t xazb[2 * N] = {0};
    uint_t xbza[2 * N] = {0};
    gfp_mul(&curve25519, a->x, b->x, xaxb, N);
    gfp_mul(&curve25519, a->z, b->z, zazb, N);
    gfp_mul(&curve25519, a->x, b->z, xazb, N);
    gfp_mul(&curve25519, b->x, a->z, xbza, N);

    uint_t tmp[2 * N] = {0};
    // x = 4 * (xa * xb - za * zb) ** 2 * zd
    gfp_sub(&curve25519, xaxb, zazb, tmp, N);
    gfp_mul(&curve25519, tmp, tmp, tmp, N);
    gfp_mul(&curve25519, d->z, tmp, tmp, N);
    gfp_add(&curve25519, tmp, tmp, tmp, N);
    gfp_add(&curve25519, tmp, tmp, res->x, N);

    // z = 4 * (xa * zb - za * xb) ** 2 * xd
    gfp_sub(&curve25519, xazb, xbza, tmp, N);
    gfp_mul(&curve25519, tmp, tmp, tmp, N);
    gfp_mul(&curve25519, d->x, tmp, tmp, N);
    gfp_add(&curve25519, tmp, tmp, tmp, N);
    gfp_add(&curve25519, tmp, tmp, res->z, N);
}

void x25519_double(x25519_element_t *e, x25519_element_t *res) {
    uint_t xx[2 * N] = {0};
    uint_t xz[2 * N] = {0};
    uint_t zz[2 * N] = {0};
    gfp_mul(&curve25519, e->x, e->x, xx, N);
    gfp_mul(&curve25519, e->x, e->z, xz, N);
    gfp_mul(&curve25519, e->z, e->z, zz, N);

    // x = (x^2 - z^2)^2
    uint_t tmp[2 * N] = {0};
    gfp_sub(&curve25519, xx, zz, tmp, N);
    gfp_mul(&curve25519, tmp, tmp, res->x, N);

    // z = 4xz(x^2 + Axz + z^2)
    gfp_mul(&curve25519, A, xz, tmp, N);
    gfp_add(&curve25519, tmp, xx, tmp, N);
    gfp_add(&curve25519, tmp, zz, tmp, N);
    gfp_mul(&curve25519, tmp, xz, tmp, N);
    gfp_add(&curve25519, tmp, tmp, tmp, N);
    gfp_add(&curve25519, tmp, tmp, res->z, N);
}

void x25519_scalar_mult(uint_t *s, x25519_element_t *e, uint_t *res) {
    x25519_element_t one = *e;
    x25519_element_t two = {0};
    x25519_double(&one, &two);
    x25519_element_t pm = one;
    x25519_element_t pm1 = two;
 
    for (int i = 253; i >= 0; i--) {
        uint_t b = uint_get_bit(s, i, N);
        x25519_element_t a = {0};
        x25519_element_t d = {0};
        x25519_add(&pm, &pm1, &one, &a);
        x25519_double(b ? &pm1: &pm, &d);
        pm = b ? a: d;
        pm1 = b ? d: a;
    }
    
    uint_t z_inv[2 * N] = {0};
    gfp_inv(&curve25519, pm.z, z_inv, N);
    gfp_mul(&curve25519, pm.x, z_inv, res, N);
}
