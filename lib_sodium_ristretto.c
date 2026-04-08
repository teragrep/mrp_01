#include <sodium.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

#ifndef my_bool
typedef char my_bool;
#endif

my_bool ristretto_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    /* make sure user has provided exactly one string argument */
    if (args->arg_count != 1 || (args->arg_type[0] != STRING_RESULT)){
        strcpy(message, "ristretto hash requires 1 string argument");
        return 1;
                             }

    args->maybe_null[0] = 1;

    initid->ptr = malloc(crypto_core_ristretto255_BYTES);
    if (initid->ptr == 0)
    {
        strcpy(message, "ristretto not enough memory for buffer");
        return 1;
    }

    return 0;
}

// Take unsigned char x as input arguments and return the ristretto'ed hash.
char* ristretto(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    if (sodium_init() == -1) {
        //return 1;
    }
    char* x=args->args[0];
    unsigned char xx, yy;
    xx = (unsigned char)*x;
    yy = (unsigned char)*initid->ptr;

    crypto_core_ristretto255_from_hash(&yy, &xx);

    return initid->ptr;
}

void ristretto_deinit(UDF_INIT *initid) {
    if (initid->ptr != 0)
    {
        free( initid->ptr);
    }
}

