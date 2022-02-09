#include <mod.h>
#include <assert.h>
#include <string.h>

static const uint_t ZERO[2 * N_MAX] = {0};

void mod_add(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    assert(uint_cmp(p, a, n) > 0);
    assert(uint_cmp(p, b, n) > 0);
    uint_t a1[N_MAX + 1] = {0};
    uint_t b1[N_MAX + 1] = {0};
    uint_t c1[N_MAX + 1] = {0};
    uint_t p1[N_MAX + 1] = {0};
    memcpy(a1, a, n * sizeof(uint_t));
    memcpy(b1, b, n * sizeof(uint_t));
    memcpy(p1, p, n * sizeof(uint_t));
    uint_add(a1, b1, c1, n + 1);
    int d = uint_cmp(p1, c1, n + 1) <= 0;
    uint_sub(c1, d ? p1: ZERO, c1, n + 1);
    memcpy(c, c1, n * sizeof(uint_t));
}

void mod_sub(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    assert(uint_cmp(p, a, n) > 0);
    assert(uint_cmp(p, b, n) > 0);
    uint_t a1[N_MAX + 1] = {0};
    uint_t b1[N_MAX + 1] = {0};
    uint_t c1[N_MAX + 1] = {0};
    uint_t p1[N_MAX + 1] = {0};
    memcpy(a1, a, n * sizeof(uint_t));
    memcpy(b1, b, n * sizeof(uint_t));
    memcpy(p1, p, n * sizeof(uint_t));
    int d = uint_cmp(a1, b1, n + 1) < 0;
    uint_add(a1, d ? p1: ZERO, a1, n + 1);
    uint_sub(a1, b1, c1, n + 1);
    memcpy(c, c1, n * sizeof(uint_t));
}

void mod_mul(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    assert(uint_cmp(p, a, n) > 0);
    assert(uint_cmp(p, b, n) > 0);
    uint_t c1[2 * N_MAX] = {0};
    uint_t p1[2 * N_MAX] = {0};
    memcpy(p1, p, n * sizeof(uint32_t));
    uint_mul(a, b, c1, n);
    uint_t q[2 * N_MAX] = {0};
    uint_t r[2 * N_MAX] = {0};
    uint_div(c1, p1, q, r, 2 * n);
    memcpy(c, r, n * sizeof(uint_t));
}

void mod_pow(const uint_t *p, const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    size_t bits = n * LIMB_BITS;
    uint_t a_2b[N_MAX] = {0};
    memcpy(a_2b, a, n * sizeof(uint_t));
    uint_t res[N_MAX] = {0};
    res[0] = 1;
    uint_t one[N_MAX] = {0};
    one[0] = 1;
    for (size_t i = 0; i < bits; i++) {
        uint_t bit = uint_get_bit(b, i, n);
        mod_mul(p, res, bit ? a_2b: one, res, n);
        mod_mul(p, a_2b, a_2b, a_2b, n);
    }
    memcpy(c, res, n * sizeof(uint_t));
}