#include <sodium.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

#ifndef my_bool
typedef char my_bool;
#endif

my_bool ristrettofromhash_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    /* make sure user has provided exactly one string argument */
    if (args->arg_count != 1 || (args->arg_type[0] != STRING_RESULT)){
        strcpy(message, "ristrettofromhash requires 1 string argument");
        return 1;
    }
    if (args->lengths[0] != crypto_core_ristretto255_HASHBYTES){
        strcpy(message, "ristrettofromhash requires 64 byte argument");
        return 1;
    }

    initid->ptr = malloc(crypto_core_ristretto255_BYTES);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristrettofromhash not enough memory for buffer");
        return 1;
    }

    return 0;
}

void ristrettofromhash_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

char* ristrettofromhash(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    if (sodium_init() == -1) {
        *error = 1;
        return 0;
    }
    unsigned char x[crypto_core_ristretto255_HASHBYTES];
    memcpy(x, args->args[0], args->lengths[0]);
    unsigned char yy[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash(yy, x);
    if (crypto_core_ristretto255_is_valid_point(yy) == 0) {
        *error = 1;
        return 0;
    }
    memcpy(initid->ptr, yy, crypto_core_ristretto255_BYTES);
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

my_bool ristrettoscalarrandom_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    initid->ptr = malloc(crypto_core_ristretto255_SCALARBYTES);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristrettoscalarrandom not enough memory for buffer");
        return 1;
    }
    return 0;
}

void ristrettoscalarrandom_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

char* ristrettoscalarrandom(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    if (sodium_init() == -1) {
        *error = 1;
        return 0;
    }
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(r);
    memcpy(initid->ptr, r, crypto_core_ristretto255_SCALARBYTES);
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarmultbase_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    /* make sure user has provided exactly one string argument */
    if (args->arg_count != 1 || (args->arg_type[0] != STRING_RESULT)){
        strcpy(message, "ristrettoscalarmultbase requires 1 string argument");
        return 1;
    }
    if (args->lengths[0] != crypto_core_ristretto255_SCALARBYTES){
        strcpy(message, "ristrettoscalarmultbase requires string scalar argument");
        return 1;
    }

    initid->ptr = malloc(crypto_core_ristretto255_BYTES);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristrettoscalarmultbase not enough memory for buffer");
        return 1;
    }
    return 0;
}

void ristrettoscalarmultbase_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

char* ristrettoscalarmultbase(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    if (sodium_init() == -1) {
        *error = 1;
        return 0;
    }
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy(r, args->args[0], args->lengths[0]);
    unsigned char gr[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255_base(gr, r);
    if (crypto_core_ristretto255_is_valid_point(gr) == 0) {
        *error = 1;
        return 0;
    }
    memcpy(initid->ptr, gr, crypto_core_ristretto255_BYTES);
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}
