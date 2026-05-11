#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testPassRistrettorandom_init() {
    char * testArgs[] = {0};
    unsigned long testLengths[1] = {0};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettorandom_init(&initid, &args, message);
    assert(result == 0 && "Result is not 0, _init failed when it should have passed.");
    assert(initid.ptr != 0 && "Memory was not succesfully allocated");
    printf("testPassRistrettorandom_init() passed assertions!\n");
    free(initid.ptr);
}

void testRistrettorandom() {
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 0, .arg_type = itemValue, .args = 0, .lengths = 0, .maybe_null=0};
    char *ristrettoPoint = malloc (crypto_core_ristretto255_BYTES);
    assert(ristrettoPoint != 0);
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0};
    ristrettorandom(&initid, &args, result, length, is_null, error);
    assert(crypto_core_ristretto255_is_valid_point(initid.ptr) == 1 && "Result is not 1, generated ristretto point was not valid.");
    printf("testRistrettorandom() passed assertions!\n");
    free(ristrettoPoint);
}

void testRistrettorandom_deinit() {
    char *ristrettoPoint = malloc (crypto_core_ristretto255_BYTES);
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0};
    assert(initid.ptr != 0);
    ristrettoadd_deinit(&initid);
    assert(initid.ptr == 0 && "_deinit failed to free the allocated memory.");
    printf("testRistrettorandom_deinit() passed assertions!\n");
}

int main()
{
    testPassRistrettorandom_init();
    testRistrettorandom();
    testRistrettorandom_deinit();
    return 0;
}