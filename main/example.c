
#include <x25519.h>
#include <stdio.h>
#include <string.h>

uint32_t swap_uint32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

void scan_hex(char *text, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        uint32_t byte;
        sscanf(text, "%02x", &byte);
        bytes[i] = (uint8_t) byte;
        text += 2;
    }
}

void print_hex(uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", (uint8_t) bytes[i]);
    }
}

void bytes_to_uint(uint8_t *bytes, uint_t *n) {
    for (int i = 0; i < N; i++) {
        n[i] = ntohl(swap_uint32(*(uint_t*)(bytes + i * sizeof(uint_t))));
    }
}

void uint_to_bytes(uint_t *n, uint8_t *bytes) {
    for (int i = 0; i < N; i++) {
        *(uint_t*)(bytes + i * sizeof(uint_t)) = swap_uint32(htonl(n[i]));
    }
}

void random_bytes(uint8_t *sk) {
    FILE *f = fopen("/dev/random", "r");
    fread(sk, 32, 1, f);
    fclose(f);
}

int main(void) {
    x25519_init();

    // x25519 base element
    x25519_element_t base = {0};
    base.x[0] = 9;
    base.z[0] = 1;

    // scalar multiple of base (secret key)
    uint8_t sk_bytes[33] = {0};
    random_bytes(sk_bytes);

    printf("sk1: ");
    print_hex(sk_bytes, 32);
    printf("\n");

    x25519_clamp(sk_bytes);

    uint_t sk[N] = {0};
    bytes_to_uint(sk_bytes, sk);

    uint_t pk[2 * N] = {0};
    x25519_scalar_mult(sk, &base, pk);
    uint8_t pk_bytes[33] = {0};
    uint_to_bytes(pk, pk_bytes);

    printf("pk1: ");
    print_hex(pk_bytes, 32);
    printf("\n");

     // scalar multiple of base (secret key)
    char peer_pk_text[65] = {0};
    printf("enter pk2: ");
    fread(peer_pk_text, 64, 1, stdin);
    uint8_t peer_pk_bytes[33] = {0};
    scan_hex(peer_pk_text, peer_pk_bytes, 32);

    x25519_element_t peer_pk = {0};
    peer_pk.z[0] = 1;
    bytes_to_uint(peer_pk_bytes, peer_pk.x);

    uint_t shared_secret[N] = {0};
    x25519_scalar_mult(sk, &peer_pk, shared_secret);
    uint8_t shared_secret_bytes[33] = {0};
    uint_to_bytes(shared_secret, shared_secret_bytes);

    printf("secret: ");
    print_hex(shared_secret_bytes, 32);
    printf("\n");
}
