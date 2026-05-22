#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testPassRistrettoisvalidpoint_init()
{
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = rand();
    }
    char* testArgs[1] = {ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    printf( "testPassRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidArgSizeRistrettoisvalidpoint_init()
{
    char* ristrettoPoint = malloc( 64 );
    assert( ristrettoPoint != nullptr );
    size_t i;
    for( i = 0; i < 64; i++ ) {
        ristrettoPoint[i] = rand();
    }
    char* testArgs[1] = {ristrettoPoint};
    unsigned long testLengths[1] = {64};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = 64, .ptr = nullptr, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input argument is not a 32 byte binary string" ) == 0 &&
            "Error message is incorrect" );
    printf( "testInvalidArgSizeRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidArgAmountRistrettoisvalidpoint_init()
{
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = rand();
    }
    char* testArgs[2] = {ristrettoPoint, ristrettoPoint};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    printf( "testInvalidArgAmountRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidArgTypeRistrettoisvalidpoint_init()
{
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = rand();
    }
    char* testArgs[1] = {ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {INT_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    printf( "testInvalidArgTypeRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testPassRistrettoisvalidpoint()
{
    int init = sodium_init();
    assert( init >= 0 );
    // Generate a valid ristretto point for validation using crypto_core_ristretto255_random()
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    unsigned char p[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( p );
    memcpy( ristrettoPoint, p, crypto_core_ristretto255_BYTES );

    char* testArgs[1] = {ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = 21, .ptr = nullptr, .const_item = 0};
    char error[1];
    char is_null[1];

    long result = ristrettoisvalidpoint( &initid, &args, is_null, error );
    assert( result == 1 &&
            "Result is not 1, Ristretto point validation failed when it should have passed." );
    printf( "testPassRistrettoisvalidpoint() passed assertions!\n" );
    free( ristrettoPoint );
}

void testFailRistrettoisvalidpoint()
{
    // Generate an invalid ristretto point for validation using rand()
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = rand();
    }
    char* testArgs[1] = {ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = 21, .ptr = nullptr, .const_item = 0};
    char error[1];
    char is_null[1];
    long result = ristrettoisvalidpoint( &initid, &args, is_null, error );
    assert( result == 0 &&
            "Result is not 0, Ristretto point validation passed when it should have failed." );
    printf( "testFailRistrettoisvalidpoint() passed assertions!\n" );
    free( ristrettoPoint );
}

int main()
{
    testPassRistrettoisvalidpoint_init();
    testInvalidArgSizeRistrettoisvalidpoint_init();
    testInvalidArgAmountRistrettoisvalidpoint_init();
    testInvalidArgTypeRistrettoisvalidpoint_init();
    testPassRistrettoisvalidpoint();
    testFailRistrettoisvalidpoint();
    return 0;
}