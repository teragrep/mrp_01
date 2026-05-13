#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettoScalarRandom_init() {
    char * testArgs[] = {0};
    unsigned long testLengths[1] = {0};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 0, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalarrandom_init(&initid, &args, message);
    assert(result == 0 && "Result is not 0, _init failed when it should have passed.");
    assert(initid.ptr != 0 && "Memory was not succesfully allocated");
    printf("testRistrettoScalarRandom_init() passed assertions!\n");
    free(initid.ptr);
}

void testRistrettoScalarRandom_deinit() {
    char *scalar = malloc (crypto_core_ristretto255_SCALARBYTES);
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = scalar, .const_item = 0};
    assert(initid.ptr != 0);
    ristrettoscalarrandom_deinit(&initid);
    assert(initid.ptr == 0 && "_deinit failed to free the allocated memory.");
    printf("testRistrettoScalarRandom_deinit() passed assertions!\n");
}

void testRistrettoScalarRandom() {
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 0, .arg_type = itemValue, .args = 0, .lengths = 0, .maybe_null=0};
    char *scalar = calloc (crypto_core_ristretto255_SCALARBYTES, sizeof(char));
    assert(scalar != 0);
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = scalar, .const_item = 0};
    ristrettoscalarrandom(&initid, &args, result, length, is_null, error);
    char *zeroArray = calloc (crypto_core_ristretto255_SCALARBYTES, sizeof(char));
    assert(memcmp(zeroArray, scalar, crypto_core_ristretto255_SCALARBYTES) < 0 && "ristrettoscalarrandom() did not populate the output with a random 32-byte scalar");
    printf("testRistrettoScalarRandom() passed assertions!\n");
    free(scalar);
    free(zeroArray);
}

int main() {
    testRistrettoScalarRandom_init();
    testRistrettoScalarRandom_deinit();
    testRistrettoScalarRandom();
    return 0;
}