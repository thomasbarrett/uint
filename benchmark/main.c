#include <time.h>
#include <uint.h>
#include <gfp.h>
#include <stdio.h>

#define STRINGIFY(x) #x
#define BENCHMARK(func, aux, batch) benchmark(STRINGIFY(func), func, aux, batch)

uint_t q[2 * N], a1[2 * N], a2[2 * N], a3[2 * N], a4[2 * N];

void benchmark(const char *name, void (*func)(void*), void *aux, size_t batch) {
    time_t start = clock();
    size_t count = 0;
    while (1) {
        for (size_t i = 0; i < batch; i++) func(aux);
        count += batch;
        time_t end = clock();
        if (end - start > CLOCKS_PER_SEC) {
            size_t ops = count * CLOCKS_PER_SEC / (end - start);
            printf("%s: %lu\n", name, ops);
            break;
        }
    }
}

void run_uint_cmp(void *aux) {
    uint_cmp(a1, a2, N);
}

void run_uint_add(void *aux) {
    uint_add(a1, a2, a3, N);
}

void run_uint_sub(void *aux) {
    uint_sub(a1, a2, a3, N);
}

void run_uint_mul(void *aux) {
    uint_mul(a1, a2, a3, N);
}

void run_uint_div(void *aux) {
    uint_div(a1, a2, a3, a4, N);
}

void run_gfp_add(void *aux) {
    gfp_add((gfp_t*) aux, a1, a2, a3, N);
}

void run_gfp_sub(void *aux) {
    gfp_sub((gfp_t*) aux, a1, a2, a3, N);
}

void run_gfp_mul(void *aux) {
    gfp_mul((gfp_t*) aux, a1, a2, a3, N);
}

void run_gfp_pow(void *aux) {
    gfp_pow((gfp_t*) aux, a1, a2, a3, N);
}

void run_gfp_inv(void *aux) {
    gfp_inv((gfp_t*) aux, a1, a2, N);
}

int main(void) {
    must_parse_uint("0x8c307598fd51fb732b33dddb02ddc885d3b78759d6b1c165b39bba8df4a5a691", q, N);
    must_parse_uint("0x6409b613c5e7c7e927f9d2c41b56af5ea49ec28277c71eb12223a2cff01135d7", a1, N);
    must_parse_uint("0x1a74c20e80b9aa122b748f723ad25ba06018dd1d7933c61d3df4ec16860c5a0f", a2, N);
    BENCHMARK(run_uint_cmp, NULL, 100);
    BENCHMARK(run_uint_add, NULL, 100);
    BENCHMARK(run_uint_sub, NULL, 100);
    BENCHMARK(run_uint_mul, NULL, 100);
    BENCHMARK(run_uint_div, NULL, 100);
    gfp_t gfp = gfp_init(q, N);
    BENCHMARK(run_gfp_add, &gfp, 100);
    BENCHMARK(run_gfp_sub, &gfp, 100);
    BENCHMARK(run_gfp_mul, &gfp, 100);
    BENCHMARK(run_gfp_pow, &gfp, 10);
    BENCHMARK(run_gfp_inv, &gfp, 10);
}
