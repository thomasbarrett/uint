#include <uint.h>
#include <string.h>
#include <assert.h>

int big_uint_cmp(const big_uint_t *a, const big_uint_t *b) {
    int res = 0;
    for (int i = 0; i < N; i++) {
        int j = N - 1 - i;
        d_int_t ai = (d_int_t) a->limbs[j];
        d_int_t bi = (d_int_t) b->limbs[j];
        d_int_t di = ai - bi;
        int si = (int) ((d_int_t) 0 < di) - (int) (di < (d_int_t) 0);
        res += (res == 0) * si;
    }
    return res;
}

big_uint_t big_uint_add(const big_uint_t *a, const big_uint_t *b) {
    big_uint_t res;
    uint_t carry = 0;
    for (int i = 0; i < N; i++) {
        d_uint_t ai = (d_uint_t) a->limbs[i];
        d_uint_t bi = (d_uint_t) b->limbs[i];
        d_uint_t ci = ai + bi + carry;
        res.limbs[i] = (uint_t) ci;
        carry = (uint_t) (ci >> LIMB_BITS > 0);
    }
    return res;
}

big_uint_t big_uint_sub(const big_uint_t *a, const big_uint_t *b) {
    big_uint_t res;
    uint_t borrow = 0;
    for (int i = 0; i < N; i++) {
        d_uint_t ai = (d_uint_t) a->limbs[i];
        d_uint_t bi = (d_uint_t) b->limbs[i];
        d_uint_t ci = ai - bi - borrow;
        res.limbs[i] = (uint_t) ci;
        borrow = (ci >> LIMB_BITS > 0);
    }
    return res;
}

static int parse_digit(const char *str, uint8_t *digit) {
    if (strlen(str) < 1) return -2;
    char c = str[0];
    switch (c) {
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
        *digit = c - '0';
        break;
    case 'a': case 'b': case 'c': case 'd':
    case 'e': case 'f':
        *digit = c - 'a' + 0x10;
        break;
    case 'A': case 'B': case 'C': case 'D':
    case 'E': case 'F':
        *digit = c - 'A' + 0x10;
        break;
    }
    return 1;
}

static int parse_byte(const char *str, uint8_t *byte) {
    int res;
    uint8_t hi, lo;
    if (strlen(str) < 2) return -2;
    res = parse_digit(str + 0, &hi);
    if (res < 1) return res;
    res = parse_digit(str + 1, &lo);
    if (res < 1) return res;
    *byte = (hi << 4) | lo;
    return 2;
}

static int parse_limb(const char *str, uint_t *limb) {
    const char *iter = str;
    *limb = 0;
    if (strlen(iter) < 2 * sizeof(uint_t)) return -2;
    for (int i = 0; i < sizeof(uint_t); i++) {
        uint8_t byte;
        int res = parse_byte(iter, &byte);
        if (res < 0) return res;
        *limb = (*limb << 8) | byte;
        iter += 2;
    }
    return iter - str;
}

int parse_uint(const char *str, big_uint_t *uint) {
    const char *iter = str;
    if (strlen(iter) != 2 + N * 2 * sizeof(uint_t)) return -2;
    if (iter[0] != '0') return -1;
    if (iter[1] != 'x' && iter[1] != 'X') return -1;
    iter += 2;
    for (int i = 0; i < N; i++) {
        int j = N - 1 - i;
        uint_t limb;
        int res = parse_limb(iter, &limb);
        if (res < 0) return res;
        uint->limbs[j] = limb;
        iter += 2 * sizeof(uint_t);
    }
    return iter - str;
}

big_uint_t must_parse_uint(const char *str) {
    big_uint_t res;
    int err = parse_uint(str, &res);
    assert(err > 0);
    return res;
}