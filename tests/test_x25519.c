#include <x25519.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

void test_x25519_double() {
    x25519_element_t e = {0};
    e.x[0] = 9;
    e.z[0] = 1;
    char *str = file_read("tests/testdata/x25519_double.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t expected_x[2 * N] = {0};
        uint_t expected_z[2 * N] = {0};

        res = parse_uint(iter, expected_x, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected_z, N);
        assert(res > 0);
        iter += res + 1; 

        assert(uint_cmp(e.x, expected_x, 2 * N) == 0);
        assert(uint_cmp(e.z, expected_z, 2 * N) == 0);
        x25519_double(&e, &e);
    }
}


void test_x25519_add() {
    x25519_element_t one = {0};
    one.x[0] = 9;
    one.z[0] = 1;
    char *str = file_read("tests/testdata/x25519_add.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        x25519_element_t n0 = {0};
        x25519_element_t n1 = {0};
        x25519_element_t n2 = {0};
        x25519_element_t expected_n2 = {0};


        res = parse_uint(iter, n0.x, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, n0.z, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, n1.x, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, n1.z, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected_n2.x, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected_n2.z, N);
        assert(res > 0);
        iter += res + 1; 

        x25519_add(&n0, &n1, &one, &n2);
        assert(uint_cmp(n2.x, expected_n2.x, 2 * N) == 0);
        assert(uint_cmp(n2.z, expected_n2.z, 2 * N) == 0);
    }
}

void test_x25519_mul() {
    char *str = file_read("tests/testdata/x25519_mul.txt");
    char *iter = str;
    int res;
    x25519_element_t base = {0};
    base.x[0] = 9;
    base.z[0] = 1;
    while (*iter != 0) {
        uint_t scalar[N] = {0};
        uint_t expected_x[2 * N] = {0};
        uint_t x[2 * N] = {0};
        res = parse_uint(iter, scalar, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected_x, N);
        assert(res > 0);
        iter += res + 1; 

        x25519_scalar_mult(scalar, &base, x);
        assert(uint_cmp(x, expected_x, 2 * N) == 0);
    }
}

int main() {
    x25519_init();
    test_x25519_add();
    test_x25519_double();
    test_x25519_mul();
    return 0;
}
