#include <sodium.h>

// gcc -Wall -Werror lib_sodium_ristretto.c -lsodium -o lib_sodium_ristretto
// ./lib_sodium_ristretto
int main(void)
{
    if (sodium_init() == -1) {
        return 1;
    }

    unsigned char x[crypto_core_ristretto255_HASHBYTES];
    randombytes_buf(x, sizeof x);
    unsigned char px[crypto_core_ristretto255_BYTES];

    crypto_core_ristretto255_from_hash(px, x);

    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    unsigned char gr[crypto_core_ristretto255_BYTES];
    unsigned char a[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_scalar_random(r);
    crypto_scalarmult_ristretto255_base(gr, r);
    crypto_core_ristretto255_add(a, px, gr);

    unsigned char rr[crypto_core_ristretto255_SCALARBYTES];
    if (crypto_core_ristretto255_sub(rr, a, px) == -1) {
        return 1;
    }

    const size_t arrlen = sizeof(rr);
    const size_t hexlen = 2; // hex representation of byte with leading zero
    const size_t outstrlen = arrlen * hexlen;

    char * outstr = malloc(outstrlen + 1);
    if (!outstr) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    // Create a string containing a hex representation of `bytearr`
    char * p = outstr;
    for (size_t i = 0; i < arrlen; i++) {
        p += sprintf(p, "%.2x", rr[i]);
    }

    printf("String variable contains:\n%s\n", outstr);
    free(outstr);
}

