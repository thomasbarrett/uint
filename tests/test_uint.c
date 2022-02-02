#include <assert.h>
#include <uint.h>
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

void test_uint_cmp() {
    uint_t a[N], b[N];
    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000", a, N);
    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000", b, N);
    assert(big_uint_cmp(a, b, N) == 0);
    assert(big_uint_cmp(b, a, N) == 0);

    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000001", a, N);
    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000", b, N);
    assert(big_uint_cmp(a, b, N) > 0);
    assert(big_uint_cmp(b, a, N) < 0);


    must_parse_uint("0x1000000000000000000000000000000000000000000000000000000000000000", a, N);
    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000", b, N);
    assert(big_uint_cmp(a, b, N) > 0);
    assert(big_uint_cmp(b, a, N) < 0);
}

void test_uint_add() {
    char *str = file_read("tests/testdata/add.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], b[N], expected[N], actual[N];

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        big_uint_add(a, b, actual, N);
        assert(big_uint_cmp(expected, actual, N) == 0);
    }
}

void test_uint_sub() {
    char *str = file_read("tests/testdata/sub.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], b[N], expected[N], actual[N];

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        big_uint_sub(a, b, actual, N);
        assert(big_uint_cmp(expected, actual, N) == 0);
    }
}

int main(void) {
    test_uint_cmp();
    test_uint_add();
    test_uint_sub();
}