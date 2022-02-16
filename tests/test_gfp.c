#include <assert.h>
#include <gfp.h>
#include <stdio.h>
#include <stdlib.h>

char* file_read(char *path) {
    FILE *file = fopen(path, "r");
    assert(file != NULL);
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char *str = (char *) calloc(size + 1, 1);
    size_t n_read = fread(str, size, 1, file);
    assert(n_read == 1);
    return str;
}

void test_gfp_add() {
    char *str = file_read("tests/testdata/add_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[2 * N] = {0};
        uint_t a[2 * N] = {0};
        uint_t b[2 * N] = {0};
        uint_t expected[2 * N] = {0};
        uint_t actual[2 * N] = {0};
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        gfp_add(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}


void test_gfp_sub() {
    char *str = file_read("tests/testdata/sub_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[2 * N] = {0};
        uint_t a[2 * N] = {0};
        uint_t b[2 * N] = {0};
        uint_t expected[2 * N] = {0};
        uint_t actual[2 * N] = {0};
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        gfp_sub(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}


void test_gfp_mul() {
    char *str = file_read("tests/testdata/mul_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[2 * N] = {0};
        uint_t a[2 * N] = {0};
        uint_t b[2 * N] = {0};
        uint_t expected[2 * N] = {0};
        uint_t actual[2 * N] = {0};
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        gfp_mul(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_gfp_pow() {
    char *str = file_read("tests/testdata/pow_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[2 * N] = {0};
        uint_t a[2 * N] = {0};
        uint_t b[2 * N] = {0};
        uint_t expected[2 * N] = {0};
        uint_t actual[2 * N] = {0};
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        gfp_pow(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_gfp_inv() {
    char *str = file_read("tests/testdata/pow_inv.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[2 * N] = {0};
        uint_t a[2 * N] = {0};
        uint_t expected[2 * N] = {0};
        uint_t actual[2 * N] = {0};
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        gfp_inv(p, a, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_barrett_r() {
    char *str = file_read("tests/testdata/barrett.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[N] = {0};
        uint_t expected[N + 1] = {0};
        uint_t actual[N + 1] = {0};
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N + 1);
        assert(res > 0);
        iter += res + 1; 

        barrett_r(p, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

int main(void) {
    test_gfp_add();
    test_gfp_sub();
    // test_gfp_mul();
    // test_gfp_pow();
    // test_gfp_inv();
    test_barrett_r();
}
