#include <assert.h>
#include <add.h>

void test_add() {
    assert(add(1, 2) == 3);
}

int main(void) {
    test_add();
}