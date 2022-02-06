#include <uint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N_MAX 32

int uint_cmp(const uint_t *a, const uint_t *b, size_t n) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        int j = n - 1 - i;
        d_int_t ai = (d_int_t) a[j];
        d_int_t bi = (d_int_t) b[j];
        d_int_t di = ai - bi;
        int si = (int) ((d_int_t) 0 < di) - (int) (di < (d_int_t) 0);
        res += (res == 0) * si;
    }
    return res;
}

void uint_add(const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    uint_t carry = 0;
    for (int i = 0; i < n; i++) {
        d_uint_t ai = (d_uint_t) a[i];
        d_uint_t bi = (d_uint_t) b[i];
        d_uint_t ci = ai + bi + carry;
        c[i] = (uint_t) ci;
        carry = (uint_t) (ci >> LIMB_BITS);
    }
}

void uint_sub(const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    uint_t borrow = 0;
    for (int i = 0; i < n; i++) {
        d_uint_t ai = (d_uint_t) a[i];
        d_uint_t bi = (d_uint_t) b[i];
        d_uint_t ci = ai - bi - borrow;
        c[i] = (uint_t) ci;
        borrow = (ci >> LIMB_BITS > 0);
    }
}

static void _uint_mul(const uint_t *a, uint_t b, uint_t *c, size_t n) {
    uint_t carry = 0;
    for (int i = 0; i < n; i++) {
        d_uint_t ai = (d_uint_t) a[i];
        d_uint_t ci = ai * b + carry;
        c[i] = (uint_t) ci;
        carry = (uint_t) (ci >> LIMB_BITS);
    }
    c[n] = carry;
}

void uint_mul(const uint_t *a, const uint_t *b, uint_t *c, size_t n) {
    assert(n <= N_MAX);
    uint_t res[2 * N_MAX + 2] = {0};
    uint_t tmp[N_MAX + 2] = {0};
    for (int i = 0; i < n; i++) {
        uint_t bi = b[i];
        memset(tmp, 0, sizeof(tmp));
        _uint_mul(a, bi, tmp, n);
        uint_add(res + i, tmp, res + i, n + 2);
    }
    memcpy(c, res, 2 * n * sizeof(uint_t));
}

void uint_shl_word(const uint_t *a, uint_t b, uint_t *c, size_t n) {
    assert(b <= n);
    memmove(c + b, a, (n - b) * sizeof(uint_t));
    memset(c, 0, b * sizeof(uint_t));
}

static void uint_shl_one(const uint_t *a, uint_t *b, size_t n) {
    uint_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint_t ai = a[i];
        b[i] = (ai << 1) | carry;
        carry = ai >> (LIMB_BITS - 1U);
    }
}

static void uint_shr_word(const uint_t *a, uint_t b, uint_t *c, size_t n) {
    assert(b <= n);
    memmove(c, a - b, (n - b) * sizeof(uint_t));
    memset(c + n - b, 0, b * sizeof(uint_t));
}

static void uint_shr_one(const uint_t *a, uint_t *b, size_t n) {
    uint_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        size_t j = N - 1 - i;
        uint_t aj = a[j];
        b[j] = (aj >> 1) | carry;
        carry = aj << (LIMB_BITS - 1U);
    }  
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
        *digit = c - 'a' + 10;
        break;
    case 'A': case 'B': case 'C': case 'D':
    case 'E': case 'F':
        *digit = c - 'A' + 10;
        break;
    default:
        return -1;
    }
    return 1;
}

static int parse_byte(const char *str, uint8_t *byte) {
    int res;
    uint8_t hi, lo;
    const size_t len = 2;
    if (strnlen(str, len) < len) return -2;
    res = parse_digit(str + 0, &hi);
    if (res <= 0) return res;
    res = parse_digit(str + 1, &lo);
    if (res <= 0) return res;
    *byte = (hi << 4U) | lo;
    return 2;
}

static int parse_limb(const char *str, uint_t *limb) {
    const char *iter = str;
    *limb = 0;
    const size_t len = 2 * sizeof(uint_t);
    if (strnlen(iter, len) < len) return -2;
    for (int i = 0; i < sizeof(uint_t); i++) {
        uint8_t byte;
        int res = parse_byte(iter, &byte);
        if (res <= 0) return res;
        *limb = (*limb << 8U) | byte;
        iter += 2;
    }
    return iter - str;
}

int parse_uint(const char *str, uint_t *x, size_t n) {
    const char *iter = str;
    const size_t len = 2 + n * 2 * sizeof(uint_t);

    if (strnlen(iter, len) < len) return -2;
    if (iter[0] != '0') return -1;
    if (iter[1] != 'x' && iter[1] != 'X') return -1;
    iter += 2;
    for (int i = 0; i < n; i++) {
        int j = n - 1 - i;
        uint_t limb;
        int res = parse_limb(iter, &limb);
        if (res <= 0) return res;
        x[j] = limb;
        iter += 2 * sizeof(uint_t);
    }
    return iter - str;
}

void must_parse_uint(const char *str, uint_t *x, size_t n) {
    int err = parse_uint(str, x, n);
    assert(err > 0);
}

void uint_print(const uint_t *x, size_t n) {
    printf("0x");
    for (int i = 0; i < n; i++) {
        int j = n - 1 - i;
        printf("%08x", x[j]);
    }
}