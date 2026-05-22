#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettofromhash_init()
{
    char* hash = malloc( crypto_core_ristretto255_HASHBYTES );
    assert( hash != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_HASHBYTES; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {hash};
    unsigned long testLengths[1] = {crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0
    };
    my_bool result = ristrettofromhash_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != nullptr && "Memory was not succesfully allocated" );
    free( hash );
    free( initid.ptr );
    printf( "testRistrettofromhash_init() passed assertions!\n" );
}

void testInvalidArgSizeRistrettofromhash_init()
{
    char* hash = malloc( 32 );
    assert( hash != nullptr );
    size_t i;
    for( i = 0; i < 32; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {hash};
    unsigned long testLengths[1] = {32};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    my_bool result = ristrettofromhash_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input argument is not a 64 byte binary string" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == nullptr && "Memory was allocated when it shouldn't" );
    free( hash );
    printf( "testInvalidArgSizeRistrettofromhash_init() passed assertions!\n" );
}

void testInvalidArgAmountRistrettofromhash_init()
{
    char* hash = malloc( crypto_core_ristretto255_HASHBYTES );
    assert( hash != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_HASHBYTES; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {hash, hash};
    unsigned long testLengths[2] = {crypto_core_ristretto255_HASHBYTES, crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    my_bool result = ristrettofromhash_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == nullptr && "Memory was allocated when it shouldn't" );
    free( hash );
    printf( "testInvalidArgAmountRistrettofromhash_init() passed assertions!\n" );
}

void testRistrettofromhash_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = nullptr, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_BYTES );
    assert( initid.ptr != nullptr );
    ristrettofromhash_deinit( &initid );
    assert( initid.ptr == nullptr && "_deinit failed to free the allocated memory." );
    printf( "testRistrettofromhash_deinit() passed assertions!\n" );
}

void testRistrettofromhash()
{
    char* hash = malloc( crypto_core_ristretto255_HASHBYTES );
    assert( hash != nullptr );
    size_t i;
    for( i = 0; i < crypto_core_ristretto255_HASHBYTES; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {hash};
    unsigned long testLengths[1] = {crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != nullptr );
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = nullptr};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    char* returnedPtr = ristrettofromhash( &initid, &args, result, length, is_null,
                                           error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    assert( crypto_core_ristretto255_is_valid_point( ristrettoPoint ) == 1 &&
            "Result is not a valid ristretto point" );
    printf( "testRistrettofromhash() passed assertions!\n" );
    free( hash );
    free( ristrettoPoint );
}

int main()
{
    testRistrettofromhash_init();
    testInvalidArgSizeRistrettofromhash_init();
    testInvalidArgAmountRistrettofromhash_init();
    testRistrettofromhash_deinit();
    testRistrettofromhash();
    return 0;
}