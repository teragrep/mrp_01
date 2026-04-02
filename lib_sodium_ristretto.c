#include <sodium.h>
#include <stdio.h>
#include <string.h>

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

    if (memcmp(rr, gr, crypto_core_ristretto255_BYTES) != 0) {
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

my_bool ristretto_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    /* make sure user has provided exactly one string argument */
    if (args->arg_count != 1 || (args->arg_type[0] != STRING_RESULT)){
        strcpy(message, "ristretto hash requires 1 string argument");
        return 1;
                             }

    args->maybe_null[0] = 1;

    initid->ptr = malloc( initid->max_length);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristretto not enough memory for buffer");
        return 1;
    }

    return 0;
}


char* ristretto(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {

}

void ristretto_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

