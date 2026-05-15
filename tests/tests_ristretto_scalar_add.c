#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettoScalarAdd_init() {
    char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(scalar);
    char * testArgs[] = {scalar};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalaradd_init(&initid, &args, message);
    assert(result == 0 && "Result is not 0, _init failed when it should have passed.");
    assert(initid.ptr != 0 && "Memory was not succesfully allocated");
    printf("testRistrettoScalarAdd_init() passed assertions!\n");
    free(initid.ptr);
}

void testInvalidArgsAmountRistrettoScalarAdd_init() {
    char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(scalar);
    char * testArgs[] = {scalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalaradd_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message, "requires 2 binary string arguments") == 0 && "Error message is incorrect");
    assert(initid.ptr == 0 && "Memory was allocated when it shouldn't");
    printf("testInvalidArgsAmountRistrettoScalarAdd_init() passed assertions!\n");
}

void testInvalidFirstArgSizeRistrettoScalarAdd_init() {
    char secondScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(secondScalar);
    char firstScalar[16];
    size_t i;
    for (i = 0; i < 16; i++)
    {
        firstScalar[i] = rand ();
    }
    char * testArgs[] = {firstScalar, secondScalar};
    unsigned long testLengths[] = {16};
    enum Item_result itemValue[] = {STRING_RESULT, STRING_RESULT};
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalaradd_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message, "First input is not a scalar in 32 byte binary string format") == 0 && "Error message is incorrect");
    assert(initid.ptr == 0 && "Memory was allocated when it shouldn't");
    printf("testInvalidFirstArgSizeRistrettoScalarAdd_init() passed assertions!\n");
}

void testInvalidSecondArgSizeRistrettoScalarAdd_init() {
    char firstScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(firstScalar);
    char secondScalar[16];
    size_t i;
    for (i = 0; i < 16; i++)
    {
        secondScalar[i] = rand ();
    }
    char * testArgs[] = {firstScalar, secondScalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES, 16};
    enum Item_result itemValue[] = {STRING_RESULT, STRING_RESULT};
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalaradd_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message, "Second input is not a scalar in 32 byte binary string format") == 0 && "Error message is incorrect");
    assert(initid.ptr == 0 && "Memory was allocated when it shouldn't");
    printf("testInvalidFirstArgSizeRistrettoScalarAdd_init() passed assertions!\n");
}

void testRistrettoScalarAdd_deinit() {
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    initid.ptr = malloc(crypto_core_ristretto255_SCALARBYTES);
    assert(initid.ptr != 0);
    ristrettoscalaradd_deinit(&initid);
    assert(initid.ptr == 0 && "_deinit failed to free the allocated memory.");
    printf("testRistrettoScalarAdd_deinit() passed assertions!\n");
}

void testRistrettoScalarAdd() {
    char inputScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(inputScalar);
    char * testArgs[] = {inputScalar, inputScalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_SCALARBYTES};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[] = {STRING_RESULT, STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    char *scalar = malloc (crypto_core_ristretto255_SCALARBYTES);
    assert(scalar != 0);
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = scalar, .const_item = 0};
    char *returnedPtr = ristrettoscalaradd(&initid, &args, result, length, is_null, error);
    assert(returnedPtr == initid.ptr && "Returned pointer does not originate from the UDF_INIT struct");
    char expectedScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_add(expectedScalar, inputScalar, inputScalar);
    assert(memcmp(expectedScalar, scalar, crypto_core_ristretto255_SCALARBYTES) == 0 && "Output of the ristrettoscalaradd() is not as expected");
    printf("testRistrettoScalarAdd() passed assertions!\n");
    free(scalar);
}

int main() {
    testRistrettoScalarAdd_init();
    testInvalidArgsAmountRistrettoScalarAdd_init();
    testInvalidFirstArgSizeRistrettoScalarAdd_init();
    testInvalidSecondArgSizeRistrettoScalarAdd_init();
    testRistrettoScalarAdd_deinit();
    testRistrettoScalarAdd();
    return 0;
}