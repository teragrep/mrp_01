#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"
extern int fail_malloc;

void testRistrettofromhash_init() {
    char *ristrettoPoint = malloc(crypto_core_ristretto255_HASHBYTES);
    size_t i;
    for (i = 0; i < crypto_core_ristretto255_HASHBYTES; i++) {
        ristrettoPoint[i] = rand();
    }
    char *testArgs[] = {ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettofromhash_init(&initid, &args, message);
    assert(result == 0 && "Result is not 0, _init failed when it should have passed.");
    assert(initid.ptr != 0 && "Memory was not succesfully allocated");
    free(ristrettoPoint);
    free(initid.ptr);
    printf("testRistrettofromhash_init() passed assertions!\n");
}

void testInvalidArgSizeRistrettofromhash_init() {
    char *ristrettoPoint = malloc (32);
    size_t i;
    for (i = 0; i < 32; i++)
    {
        ristrettoPoint[i] = rand ();
    }
    char * testArgs[] = {ristrettoPoint};
    unsigned long testLengths[1] = {32};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    my_bool result = ristrettofromhash_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message, "First input argument is not a 64 byte binary string") == 0 && "Error message is incorrect");
    assert(initid.ptr == 0 && "Memory was allocated when it shouldn't");
    free(ristrettoPoint);
    printf("testInvalidArgSizeRistrettofromhash_init() passed assertions!\n");
}

void testInvalidArgAmountRistrettofromhash_init() {
    char *testPoint = malloc(crypto_core_ristretto255_BYTES);
    assert(testPoint != 0);
    free(testPoint);
    char *ristrettoPoint = malloc (crypto_core_ristretto255_HASHBYTES);
    size_t i;
    for (i = 0; i < crypto_core_ristretto255_HASHBYTES; i++)
    {
        ristrettoPoint[i] = rand ();
    }
    char * testArgs[] = {ristrettoPoint, ristrettoPoint};
    unsigned long testLengths[2] = {crypto_core_ristretto255_HASHBYTES, crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    my_bool result = ristrettofromhash_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message, "requires 1 binary string argument") == 0 && "Error message is incorrect");
    assert(initid.ptr == 0 && "Memory was allocated when it shouldn't");
    free(ristrettoPoint);
    printf("testInvalidArgAmountRistrettofromhash_init() passed assertions!\n");
}

void testRistrettofromhash_deinit() {
    char *testPoint = malloc(crypto_core_ristretto255_BYTES);
    assert(testPoint != 0);
    free(testPoint);
    char *testPoint2 = malloc(crypto_core_ristretto255_BYTES);
    assert(testPoint2 != 0);
    free(testPoint2);
    char *ristrettoPoint = malloc(crypto_core_ristretto255_BYTES);
    assert(ristrettoPoint != 0);
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0};
    assert(initid.ptr != 0);
    ristrettofromhash_deinit(&initid);
    assert(initid.ptr == 0 && "_deinit failed to free the allocated memory.");
    printf("testRistrettofromhash_deinit() passed assertions!\n");
}

void testRistrettofromhash() {
    char *hash = malloc(crypto_core_ristretto255_HASHBYTES);
    size_t i;
    for (i = 0; i < crypto_core_ristretto255_HASHBYTES; i++) {
        hash[i] = rand();
    }
    char *testArgs[] = {hash};
    unsigned long testLengths[1] = {crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char *ristrettoPoint = malloc (crypto_core_ristretto255_BYTES);
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    ristrettofromhash(&initid, &args, result, length, is_null, error);
    assert(crypto_core_ristretto255_is_valid_point(ristrettoPoint) == 1 && "Result is not a valid ristretto point");
    printf("testRistrettofromhash() passed assertions!\n");
    free(hash);
    free(ristrettoPoint);
}

int main() {
    testRistrettofromhash_init();
    testInvalidArgSizeRistrettofromhash_init();
    testInvalidArgAmountRistrettofromhash_init();
    testRistrettofromhash_deinit();
    testRistrettofromhash();
    return 0;
}