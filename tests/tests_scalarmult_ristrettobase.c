#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testScalarmultristrettobase_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {scalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0
    };
    my_bool result = scalarmultristrettobase_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != nullptr && "Memory was not succesfully allocated" );
    free( initid.ptr );
    printf( "testScalarmultristrettobase_init() passed assertions!\n" );
}

void testInvalidArgsAmountScalarmultristrettobase_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {scalar, scalar};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0
    };
    my_bool result = scalarmultristrettobase_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == nullptr && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgsAmountScalarmultristrettobase_init() passed assertions!\n" );
}

void testInvalidArgSizeScalarmultristrettobase_init()
{
    unsigned char scalar[16];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {scalar};
    unsigned long testLengths[1] = {16};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0
    };
    my_bool result = scalarmultristrettobase_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input argument is not a 32 byte scalar in binary string format" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == nullptr && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgSizeScalarmultristrettobase_init() passed assertions!\n" );
}

void testScalarmultristrettobase_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_BYTES );
    assert( initid.ptr != nullptr );
    scalarmultristrettobase_deinit( &initid );
    assert( initid.ptr == nullptr &&
            "_deinit failed to free the allocated memory." );
    printf( "testScalarmultristrettobase_deinit() passed assertions!\n" );
}

void testScalarmultristrettobase()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {scalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    char* returnedPtr = scalarmultristrettobase( &initid, &args, result, length,
                        is_null, error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    assert( crypto_core_ristretto255_is_valid_point( ristrettoPoint ) == 1 &&
            "Output of the scalarmultristrettobase() is not a valid ristretto point" );
    printf( "testScalarmultristrettobase() passed assertions!\n" );
    free( ristrettoPoint );
}

int main()
{
    testScalarmultristrettobase_init();
    testInvalidArgsAmountScalarmultristrettobase_init();
    testInvalidArgSizeScalarmultristrettobase_init();
    testScalarmultristrettobase_deinit();
    testScalarmultristrettobase();
    return 0;
}