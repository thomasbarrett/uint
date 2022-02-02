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
    big_uint_t a = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    big_uint_t b = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    assert(big_uint_cmp(&a, &b) == 0);
    assert(big_uint_cmp(&b, &a) == 0);

    a = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000001");
    b = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    assert(big_uint_cmp(&a, &b) > 0);
    assert(big_uint_cmp(&b, &a) < 0);

    a = must_parse_uint("0x1000000000000000000000000000000000000000000000000000000000000000");
    b = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    assert(big_uint_cmp(&a, &b) > 0);
    assert(big_uint_cmp(&b, &a) < 0);
}

void test_uint_add() {
    char *str = file_read("tests/testdata/add.txt");
    char *iter = str;
    int res;
    while (1) {
        big_uint_t a, b, expected, actual;
        res = parse_uint(iter, &a);
        assert(res > 0);
        iter += res + 1; 
        res = parse_uint(iter, &b);
        assert(res > 0);
        iter += res + 1; 
        res = parse_uint(iter, &expected);
        actual = big_uint_add(&a, &b);
        assert(big_uint_cmp(&expected, &actual) == 0);
        printf(".\n");
    }
}

void test_uint_sub() {
    big_uint_t a, b, expected, actual;
    a = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    b = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    expected = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000000");
    actual = big_uint_sub(&a, &b);
    assert(big_uint_cmp(&expected, &actual) == 0);

    a = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000003");
    b = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000002");
    expected = must_parse_uint("0x0000000000000000000000000000000000000000000000000000000000000001");
    actual = big_uint_sub(&a, &b);
    assert(big_uint_cmp(&expected, &actual) == 0);
}

int main(void) {
    //test_uint_cmp();
    test_uint_add();
    test_uint_sub();
}