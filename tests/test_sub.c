#include <assert.h>
#include <sub.h>

void test_sub() {
    assert(sub(1, 2) == -1);
}

int main(void) {
    test_sub();
}