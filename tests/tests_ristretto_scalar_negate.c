#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettoScalarNegate_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalarnegate_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != 0 && "Memory was not succesfully allocated" );
    printf( "testRistrettoScalarNegate_init() passed assertions!\n" );
    free( initid.ptr );
}

void testInvalidArgsAmountRistrettoScalarNegate_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )scalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[] = {STRING_RESULT, STRING_RESULT};
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalarnegate_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "Requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgsAmountRistrettoScalarNegate_init() passed assertions!\n" );
}

void testInvalidFirstArgSizeRistrettoScalarNegate_init()
{
    unsigned char scalar[16];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar};
    unsigned long testLengths[] = {16};
    enum Item_result itemValue[] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoscalarnegate_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input is not a scalar in 32 byte binary string format" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidFirstArgSizeRistrettoScalarNegate_init() passed assertions!\n" );
}

void testRistrettoScalarNegate_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( initid.ptr != 0 );
    ristrettoscalarnegate_deinit( &initid );
    assert( initid.ptr == 0 && "_deinit failed to free the allocated memory." );
    printf( "testRistrettoScalarNegate_deinit() passed assertions!\n" );
}

void testRistrettoScalarNegate()
{
    unsigned char inputScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( inputScalar );
    char* testArgs[] = {( char* )inputScalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    char* scalar = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( scalar != 0 );
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = scalar, .const_item = 0};
    char* returnedPtr = ristrettoscalarnegate( &initid, &args, result, length,
                        is_null, error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    char expectedScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_negate( ( unsigned char* )expectedScalar,
                                            inputScalar );
    assert( memcmp( expectedScalar, scalar,
                    crypto_core_ristretto255_SCALARBYTES ) == 0 &&
            "Output of the ristrettoscalarnegate() is not as expected" );
    printf( "testRistrettoScalarNegate() passed assertions!\n" );
    free( scalar );
}

int main()
{
    testRistrettoScalarNegate_init();
    testInvalidArgsAmountRistrettoScalarNegate_init();
    testInvalidFirstArgSizeRistrettoScalarNegate_init();
    testRistrettoScalarNegate_deinit();
    testRistrettoScalarNegate();
    return 0;
}