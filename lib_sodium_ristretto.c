#include <sodium.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

#ifndef my_bool
typedef char my_bool;
#endif

my_bool ristrettofromhash_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
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
    memcpy(initid->ptr, gr, crypto_core_ristretto255_BYTES);
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

my_bool ristrettoadd_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if (args->arg_count != 2 || (args->arg_type[0] != STRING_RESULT) || (args->arg_type[1] != STRING_RESULT)){
        strcpy(message, "ristrettoadd requires 2 string argument");
        return 1;
    }
    if (args->lengths[0] != crypto_core_ristretto255_BYTES || args->lengths[1] != crypto_core_ristretto255_BYTES){
        strcpy(message, "ristrettoadd requires 32 byte arguments");
        return 1;
    }
    if (sodium_init() == -1) {
        strcpy(message, "sodium failed to initialize");
        return 1;
    }
    unsigned char arg0[crypto_core_ristretto255_BYTES];
    memcpy(arg0, args->args[0], args->lengths[0]);
    if (crypto_core_ristretto255_is_valid_point(arg0) == 0) {
        strcpy(message, "First input is not a valid ristretto point");
        return 1;
    }
    unsigned char arg1[crypto_core_ristretto255_BYTES];
    memcpy(arg1, args->args[1], args->lengths[1]);
    if (crypto_core_ristretto255_is_valid_point(arg1) == 0) {
        strcpy(message, "Second input is not a valid ristretto point");
        return 1;
    }


    initid->ptr = malloc(crypto_core_ristretto255_BYTES);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristrettoadd not enough memory for buffer");
        return 1;
    }
    return 0;
}

void ristrettoadd_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

char* ristrettoadd(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    if (sodium_init() == -1) {
        *error = 1;
        return 0;
    }
    unsigned char gr[crypto_core_ristretto255_BYTES];
    memcpy(gr, args->args[0], args->lengths[0]);
    unsigned char px[crypto_core_ristretto255_BYTES];
    memcpy(px, args->args[1], args->lengths[1]);
    unsigned char a[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_add(a, px, gr);
    memcpy(initid->ptr, a, crypto_core_ristretto255_BYTES);
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

my_bool ristrettoscalarnegate_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if (args->arg_count != 1 || (args->arg_type[0] != STRING_RESULT)){
        strcpy(message, "ristrettoscalarnegate requires 1 string argument");
        return 1;
    }
    if (args->lengths[0] != crypto_core_ristretto255_SCALARBYTES){
        strcpy(message, "ristrettoscalarnegate requires 32 byte argument");
        return 1;
    }

    initid->ptr = malloc(crypto_core_ristretto255_SCALARBYTES);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristrettoscalarnegate not enough memory for buffer");
        return 1;
    }
    return 0;
}

void ristrettoscalarnegate_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

char* ristrettoscalarnegate(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    if (sodium_init() == -1) {
        *error = 1;
        return 0;
    }
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy(r, args->args[0], args->lengths[0]);
    unsigned char ir[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_negate(ir, r);
    memcpy(initid->ptr, ir, crypto_core_ristretto255_SCALARBYTES);
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettovalidpoint_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if (args->arg_count != 1 || (args->arg_type[0] != STRING_RESULT)){
        strcpy(message, "ristrettovalidpoint requires 1 string argument");
        return 1;
    }
    if (args->lengths[0] != crypto_core_ristretto255_BYTES){
        strcpy(message, "ristrettovalidpoint requires 32 byte argument");
        return 1;
    }
    return 0;
}

long long ristrettovalidpoint(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    if (sodium_init() == -1) {
        *error = 1;
        return 0;
    }
    unsigned char r[crypto_core_ristretto255_BYTES];
    memcpy(r, args->args[0], args->lengths[0]);
    if (crypto_core_ristretto255_is_valid_point(r) == 0) {
        return 0;
    }
    return 1;
}