#include <time.h>
#include <uint.h>
#include <mod.h>
#include <stdio.h>

#define STRINGIFY(x) #x
#define BENCHMARK(func, batch) benchmark(STRINGIFY(func), func, batch)

uint_t q[N], a1[N], a2[N], a3[2 * N], a4[2 * N];

void benchmark(const char *name, void (*func)(void), size_t batch) {
    time_t start = clock();
    size_t count = 0;
    while (1) {
        for (size_t i = 0; i < batch; i++) func();
        count += batch;
        time_t end = clock();
        if (end - start > CLOCKS_PER_SEC) {
            size_t ops = count * CLOCKS_PER_SEC / (end - start);
            printf("%s: %lu\n", name, ops);
            break;
        }
    }
}

void run_uint_cmp(void) {
    uint_cmp(a1, a2, N);
}

void run_uint_add(void) {
    uint_add(a1, a2, a3, N);
}

void run_uint_sub(void) {
    uint_sub(a1, a2, a3, N);
}

void run_uint_mul(void) {
    uint_mul(a1, a2, a3, N);
}

void run_uint_div(void) {
    uint_div(a1, a2, a3, a4, N);
}

void run_mod_add(void) {
    mod_add(q, a1, a2, a3, N);
}

void run_mod_sub(void) {
    mod_sub(q, a1, a2, a3, N);
}

void run_mod_mul(void) {
    mod_mul(q, a1, a2, a3, N);
}

void run_mod_pow(void) {
    mod_pow(q, a1, a2, a3, N);
}

void run_mod_inv(void) {
    mod_inv(q, a1, a2, N);
}

int main(void) {
    must_parse_uint("0x8c307598fd51fb732b33dddb02ddc885d3b78759d6b1c165b39bba8df4a5a691", q, N);
    must_parse_uint("0x6409b613c5e7c7e927f9d2c41b56af5ea49ec28277c71eb12223a2cff01135d7", a1, N);
    must_parse_uint("0x1a74c20e80b9aa122b748f723ad25ba06018dd1d7933c61d3df4ec16860c5a0f", a2, N);
    BENCHMARK(run_uint_cmp, 100);
    BENCHMARK(run_uint_add, 100);
    BENCHMARK(run_uint_sub, 100);
    BENCHMARK(run_uint_mul, 100);
    BENCHMARK(run_uint_div, 100);
    BENCHMARK(run_mod_add, 100);
    BENCHMARK(run_mod_sub, 100);
    BENCHMARK(run_mod_mul, 100);
    BENCHMARK(run_mod_pow, 1);
    BENCHMARK(run_mod_inv, 1);
}