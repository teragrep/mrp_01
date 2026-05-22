#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettoAdd_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* ) point1, ( char* ) point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoadd_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != 0 && "Memory was not succesfully allocated" );
    free( initid.ptr );
    printf( "testRistrettoAdd_init() passed assertions!\n" );
}

void testInvalidArgsAmountRistrettoAdd_init()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point );
    char* testArgs[] = {( char* ) point};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoadd_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 2 binary string arguments" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgsAmountRistrettoAdd_init() passed assertions!\n" );
}

void testInvalidFirstArgSizeRistrettoAdd_init()
{
    unsigned char point1[16];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* ) point1, ( char* ) point2};
    unsigned long testLengths[2] = {16, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoadd_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input argument is not a 32 byte binary string" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidFirstArgSizeRistrettoAdd_init() passed assertions!\n" );
}

void testInvalidFirstArgPointRistrettoAdd_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point1[i] = rand();
    }
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* ) point1, ( char* ) point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoadd_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "First input is not a valid ristretto point" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidFirstArgPointRistrettoAdd_init() passed assertions!\n" );
}

void testInvalidSecondArgSizeRistrettoAdd_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[16];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* ) point1, ( char* ) point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, 16};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoadd_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "Second input argument is not a 32 byte binary string" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidSecondArgSizeRistrettoAdd_init() passed assertions!\n" );
}

void testInvalidSecondArgPointRistrettoAdd_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point2[i] = rand();
    }
    char* testArgs[] = {( char* ) point1, ( char* ) point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    my_bool result = ristrettoadd_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "Second input is not a valid ristretto point" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidSecondArgPointRistrettoAdd_init() passed assertions!\n" );
}

void testRistrettoAdd_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_BYTES );
    assert( initid.ptr != 0 );
    ristrettoadd_deinit( &initid );
    assert( initid.ptr == 0 && "_deinit failed to free the allocated memory." );
    printf( "testRistrettoAdd_deinit() passed assertions!\n" );
}

void testRistrettoAdd()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* ) point1, ( char* ) point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != 0 );
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    char* returnedPtr = ristrettoadd( &initid, &args, result, length, is_null,
                                      error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    assert( crypto_core_ristretto255_is_valid_point( ( unsigned char* )
            ristrettoPoint ) == 1 &&
            "Output of the ristrettoadd() is not a valid ristretto point" );
    char expecteRistrettoPoint[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_add( ( unsigned char* ) expecteRistrettoPoint, point2,
                                  point1 );
    assert( memcmp( expecteRistrettoPoint, ristrettoPoint,
                    crypto_core_ristretto255_BYTES ) == 0 &&
            "Output of the ristrettoadd() is not as expected" );
    printf( "testRistrettoAdd() passed assertions!\n" );
    free( ristrettoPoint );
}

int main()
{
    testRistrettoAdd_init();
    testInvalidArgsAmountRistrettoAdd_init();
    testInvalidFirstArgSizeRistrettoAdd_init();
    testInvalidFirstArgPointRistrettoAdd_init();
    testInvalidSecondArgSizeRistrettoAdd_init();
    testInvalidSecondArgPointRistrettoAdd_init();
    testRistrettoAdd_deinit();
    testRistrettoAdd();
    return 0;
}