/*
    MariaDB Ristretto255 Plugin (mrp_01)
    Copyright (C) 2021-2026 Suomen Kanuuna Oy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


    Additional permission under GNU Affero General Public License version 3
    section 7

    If you modify this Program, or any covered work, by linking or combining it
    with other code, such other code is not for that reason alone subject to any
    of the requirements of the GNU Affero GPL version 3 as long as this Program
    is the same Program as licensed from Suomen Kanuuna Oy without any additional
    modifications.

    Supplemented terms under GNU Affero General Public License version 3
    section 7

    Origin of the software must be attributed to Suomen Kanuuna Oy. Any modified
    versions must be marked as "Modified version of" The Program.

    Names of the licensors and authors may not be used for publicity purposes.

    No rights are granted for use of trade names, trademarks, or service marks
    which are in The Program if any.

    Licensee must indemnify licensors and authors for any liability that these
    contractual assumptions impose on licensors and authors.

    To the extent this program is licensed as part of the Commercial versions of
    Teragrep, the applicable Commercial License may apply to this file if you as
    a licensee so wish it.
*/
#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto_scalarmult.h"

void testScalarmultristretto_init()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point );
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )point};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = scalarmultristretto_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != NULL && "Memory was not succesfully allocated" );
    free( initid.ptr );
    printf( "testScalarmultristretto_init() passed assertions!\n" );
}

void testInvalidArgsAmountScalarmultristretto_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = scalarmultristretto_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 2 binary string arguments" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == NULL && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgsAmountScalarmultristretto_init() passed assertions!\n" );
}

void testInvalidFirstArgSizeScalarmultristretto_init()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point );
    unsigned char scalar[16];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )point};
    unsigned long testLengths[2] = {16, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = scalarmultristretto_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input argument is not a 32 byte scalar in binary string format" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == NULL && "Memory was allocated when it shouldn't" );
    printf( "testInvalidFirstArgSizeScalarmultristretto_init() passed assertions!\n" );
}

void testInvalidSecondArgSizeScalarmultristretto_init()
{
    unsigned char point[16];
    for( size_t i = 0; i < 16; i++ ) {
        point[i] = rand();
    }
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )point};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, 16};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = scalarmultristretto_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "Second input argument is not a 32 byte binary string" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == NULL && "Memory was allocated when it shouldn't" );
    printf( "testInvalidSecondArgSizeScalarmultristretto_init() passed assertions!\n" );
}

void testInvalidSecondArgPointScalarmultristretto_init()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point[i] = i;
    }
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )point};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = scalarmultristretto_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != NULL && "Memory was not succesfully allocated" );
    free( initid.ptr );
    printf( "testInvalidSecondArgPointScalarmultristretto_init() passed assertions!\n" );
}

void testScalarmultristretto_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( initid.ptr != NULL );
    scalarmultristretto_deinit( &initid );
    assert( initid.ptr == NULL && "_deinit failed to free the allocated memory." );
    printf( "testScalarmultristretto_deinit() passed assertions!\n" );
}

void testScalarmultristretto()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point );
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )point};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = scalarmultristretto( &initid, &args, result, length,
                              is_null, error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    assert( crypto_core_ristretto255_is_valid_point( ( unsigned char* )
            ristrettoPoint ) == 1 &&
            "Output of the scalarmultristretto() is not a valid ristretto point" );
    char expecteRistrettoPoint[crypto_core_ristretto255_BYTES];
    const int success = crypto_scalarmult_ristretto255( ( unsigned char* )
                        expecteRistrettoPoint, scalar,
                        point );
    assert( success == 0 && "crypto_scalarmult_ristretto255() was not successful" );
    assert( memcmp( expecteRistrettoPoint, ristrettoPoint,
                    crypto_core_ristretto255_BYTES ) == 0 &&
            "Output of the scalarmultristretto() is not as expected" );
    printf( "testScalarmultristretto() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidSecondArgPointScalarmultristretto()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point[i] = i;
    }
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )point};
    unsigned long testLengths[2] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = scalarmultristretto( &initid, &args, result, length,
                              is_null, error );
    assert( returnedPtr == NULL &&
            "scalarmultristretto() did not return NULL on invalid second input argument" );
    printf( "testInvalidSecondArgPointRistrettoSub() passed assertions!\n" );
    free( ristrettoPoint );
}

int main()
{
    testScalarmultristretto_init();
    testInvalidArgsAmountScalarmultristretto_init();
    testInvalidFirstArgSizeScalarmultristretto_init();
    testInvalidSecondArgSizeScalarmultristretto_init();
    testInvalidSecondArgPointScalarmultristretto_init();
    testScalarmultristretto_deinit();
    testScalarmultristretto();
    testInvalidSecondArgPointScalarmultristretto();
    return 0;
}