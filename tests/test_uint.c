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
    assert(uint_cmp(a, b, N) == 0);
    assert(uint_cmp(b, a, N) == 0);

    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000001", a, N);
    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000", b, N);
    assert(uint_cmp(a, b, N) > 0);
    assert(uint_cmp(b, a, N) < 0);


    must_parse_uint("0x1000000000000000000000000000000000000000000000000000000000000000", a, N);
    must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000", b, N);
    assert(uint_cmp(a, b, N) > 0);
    assert(uint_cmp(b, a, N) < 0);
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

        uint_add(a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
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

        uint_sub(a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_uint_mul() {
    char *str = file_read("tests/testdata/mul.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], b[N], expected[2 * N], actual[2 * N];

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, 2 * N);
        assert(res > 0);
        iter += res + 1; 

        uint_mul(a, b, actual, N);
        assert(uint_cmp(expected, actual, 2 * N) == 0);
    }
}

void test_uint_div() {
    char *str = file_read("tests/testdata/div.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], b[N];
        uint_t expected_q[N], actual_q[N];
        uint_t expected_r[N], actual_r[N];

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, b, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected_q, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected_r, N);
        assert(res > 0);
        iter += res + 1; 

        uint_div(a, b, actual_q, actual_r, N);
        assert(uint_cmp(expected_q, actual_q, N) == 0);
        assert(uint_cmp(expected_r, actual_r, N) == 0);
    }
}

void test_uint_shl_one() {
    char *str = file_read("tests/testdata/shl_one.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], expected[N], actual[N];

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        uint_shl_one(a, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_uint_shr_one() {
    char *str = file_read("tests/testdata/shr_one.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], expected[N], actual[N];

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        uint_shr_one(a, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_uint_shl_limb() {
    char *str = file_read("tests/testdata/shl_limb.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], expected[N], actual[N];
        uint_t b;

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, &b, 1);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        uint_shl_limb(a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

void test_uint_shr_limb() {
    char *str = file_read("tests/testdata/shr_limb.txt");
    char *iter = str;
    int res;
    while (*iter != 0) {
        uint_t a[N], expected[N], actual[N];
        uint_t b;

        res = parse_uint(iter, a, N);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, &b, 1);
        assert(res > 0);
        iter += res + 1; 

        res = parse_uint(iter, expected, N);
        assert(res > 0);
        iter += res + 1; 

        uint_shr_limb(a, b, actual, N);
        assert(uint_cmp(expected, actual, N) == 0);
    }
}

int main(void) {
    test_uint_cmp();
    test_uint_add();
    test_uint_sub();
    test_uint_mul();
    test_uint_div();
    test_uint_shl_one();
    test_uint_shr_one();
    test_uint_shl_limb();
    test_uint_shr_limb();
}
