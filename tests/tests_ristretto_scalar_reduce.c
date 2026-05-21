#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettoScalarReduce_init()
{
    char* nonReducedScalar = malloc(
                                 crypto_core_ristretto255_NONREDUCEDSCALARBYTES );
    assert( nonReducedScalar != 0 );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_NONREDUCEDSCALARBYTES; i++ ) {
        nonReducedScalar[i] = rand();
    }
    char* testArgs[] = {nonReducedScalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_NONREDUCEDSCALARBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalarreduce_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != 0 && "Memory was not succesfully allocated" );
    printf( "testRistrettoScalarReduce_init() passed assertions!\n" );
    free( initid.ptr );
    free( nonReducedScalar );
}

void testInvalidArgsAmountRistrettoScalarReduce_init()
{
    char* nonReducedScalar = malloc(
                                 crypto_core_ristretto255_NONREDUCEDSCALARBYTES );
    assert( nonReducedScalar != 0 );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_NONREDUCEDSCALARBYTES; i++ ) {
        nonReducedScalar[i] = rand();
    }
    char* testArgs[] = {nonReducedScalar, nonReducedScalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_NONREDUCEDSCALARBYTES, crypto_core_ristretto255_NONREDUCEDSCALARBYTES};
    enum Item_result itemValue[] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoscalarreduce_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgsAmountRistrettoScalarReduce_init() passed assertions!\n" );
    free( nonReducedScalar );
}

void testInvalidFirstArgSizeRistrettoScalarReduce_init()
{
    char* scalar = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( scalar != 0 );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_SCALARBYTES; i++ ) {
        scalar[i] = rand();
    }
    char* testArgs[] = {scalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoscalarreduce_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input is not a scalar in 64 byte binary string format" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidFirstArgSizeRistrettoScalarReduce_init() passed assertions!\n" );
    free( scalar );
}

void testRistrettoScalarReduce_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( initid.ptr != 0 );
    ristrettoscalarreduce_deinit( &initid );
    assert( initid.ptr == 0 && "_deinit failed to free the allocated memory." );
    printf( "testRistrettoScalarReduce_deinit() passed assertions!\n" );
}

void testRistrettoScalarReduce()
{
    char* nonReducedScalar = malloc(
                                 crypto_core_ristretto255_NONREDUCEDSCALARBYTES );
    assert( nonReducedScalar != 0 );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_NONREDUCEDSCALARBYTES; i++ ) {
        nonReducedScalar[i] = rand();
    }
    char* testArgs[] = {nonReducedScalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_NONREDUCEDSCALARBYTES};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    char* scalar = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( scalar != 0 );
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = scalar, .const_item = 0};
    char* returnedPtr = ristrettoscalarreduce( &initid, &args, result, length,
                        is_null, error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    char expectedScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_reduce( expectedScalar, nonReducedScalar );
    assert( memcmp( expectedScalar, scalar,
                    crypto_core_ristretto255_SCALARBYTES ) == 0 &&
            "Output of the ristrettoscalarreduce() is not as expected" );
    printf( "testRistrettoScalarReduce() passed assertions!\n" );
    free( scalar );
    free( nonReducedScalar );
}

int main()
{
    testRistrettoScalarReduce_init();
    testInvalidArgsAmountRistrettoScalarReduce_init();
    testInvalidFirstArgSizeRistrettoScalarReduce_init();
    testRistrettoScalarReduce_deinit();
    testRistrettoScalarReduce();
    return 0;
}