#include <assert.h>
#include <mod.h>
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

void test_mod_add() {
    char *str = file_read("tests/testdata/add_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[N], a[N], b[N], expected[N], actual[N];
        
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

        mod_add(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}


void test_mod_sub() {
    char *str = file_read("tests/testdata/sub_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[N], a[N], b[N], expected[N], actual[N];
        
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

        mod_sub(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}


void test_mod_mul() {
    char *str = file_read("tests/testdata/mul_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[N], a[N], b[N], expected[N], actual[N];
        
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

        mod_mul(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_mod_pow() {
    char *str = file_read("tests/testdata/pow_mod.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[N], a[N], b[N], expected[N], actual[N];
        
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

        mod_pow(p, a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_mod_inv() {
    char *str = file_read("tests/testdata/pow_inv.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t p[N], a[N], expected[N], actual[N];
        
        res = parse_uint(iter, p, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        mod_inv(p, a, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

int main(void) {
    test_mod_add();
    test_mod_sub();
    test_mod_mul();
    test_mod_pow();
    test_mod_inv();
}