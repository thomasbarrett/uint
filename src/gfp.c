#include <gfp.h>
#include <assert.h>
#include <string.h>


uint_t ZERO[N_MAX] = {0};

static size_t uint_ceil_log2(uint_t *a, size_t n) {
    size_t k = 0;
    uint_t b[N_MAX] = {0};
    memcpy(b, a, n * sizeof(uint_t));
    while(uint_cmp(b, ZERO, n) > 0) {
        uint_shr_one(b, b, n);
        k++;
    }
    return k;
}

void barrett_r(uint_t *p, uint_t *r, size_t n) {
    size_t k = uint_ceil_log2(p, n);
    uint_t tmp[2 * N_MAX + 1] = {0};
    uint_set_bit(tmp, 2 * k, 1, 2 * n + 1);
    uint_t p2[2 * N_MAX + 1] = {0};
    memcpy(p2, p, n * sizeof(uint_t));
    uint_t q[2 * N_MAX + 1];
    uint_t rem[2 * N_MAX + 1];
    uint_div(tmp, p2, q, rem, n);
    memcpy(r, q, (n + 1) * sizeof(uint_t));
}

void gfp_add(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    uint_t res[N_MAX + 1] = {0};
    uint_t tmp[N_MAX + 1] = {0};
    uint_add(a, b, res, n + 1);
    int d = uint_cmp(p, res, n + 1) <= 0;
    uint_select_zero(p, d, tmp, n + 1);
    uint_sub(res, tmp, res, n + 1);
    memcpy(c, res, n * sizeof(uint_t));
}

void gfp_sub(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    uint_t res[N_MAX + 1] = {0};
    uint_t tmp[N_MAX + 1] = {0};
    int d = uint_cmp(a, b, n + 1) < 0;
    uint_select_zero(p, d, tmp, n + 1);
    uint_add(a, tmp, res, n + 1);
    uint_sub(res, b, res, n + 1);
    memcpy(c, res, n * sizeof(uint_t));
}

void gfp_mul(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    size_t k = uint_ceil_log2(p, n) + 1;
    uint_t x[2 * N_MAX] = {0};
    uint_mul(a, b, x, n);
    uint_t r[2 * N_MAX] = {0};
    barrett_r(p, r, n);
    uint_t tmp[4 * N_MAX] = {0};
    uint_mul(x, r, tmp, 2 * n);
    for (int i = 0; i < 2 * k; i++) {
        uint_shr_one(tmp, tmp, 4 * n);
    }
    uint_mul(tmp, p, tmp, 2 * n);
    assert(uint_cmp(x, tmp, 2 * n) > 0);
    uint_sub(x, tmp, tmp, 2 * n);
    uint_t cmp = uint_cmp(p, tmp, n) > 0;
    uint_t tmp2[2 * N_MAX] = {0};
    uint_select_zero(p, cmp, tmp2, n);
    uint_sub(tmp, tmp2, c, 2 * n);
}

void gfp_pow(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    size_t bits = n * LIMB_BITS;
    uint_t pow[N_MAX];
    memcpy(pow, a, n * sizeof(uint_t));
    uint_t res[N_MAX] = {0};
    res[0] = 1;
    uint_t tmp[N_MAX];
    for (size_t i = 0; i < bits; i++) {
        uint_t bit = uint_get_bit(b, i, n);
        uint_select_one(pow, bit, tmp, n);
        gfp_mul(p, res, tmp, res, n);
        gfp_mul(p, pow, pow, pow, n);
    }
    memcpy(c, res, n * sizeof(uint_t));
}

void gfp_inv(const uint_t *p, const uint_t *a, uint_t *b, size_t n) {
    uint_t one[2 * N_MAX] = {0};
    one[0] = 1U;
    uint_t pow[2 * N_MAX];
    uint_sub(p, one, pow, n);
    gfp_pow(p, a, pow, b, n);
}
