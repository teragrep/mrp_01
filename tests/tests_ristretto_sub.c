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
#include <stdbool.h>

#include "../lib_sodium_ristretto_subtraction.h"

void testRistrettoSub_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = ristrettosub_init( &initid, &args, message );
    assert( result == false &&
            "Result is not false (0), _init failed when it should have passed." );
    printf( "testRistrettoSub_init() passed assertions!\n" );
}

void testInvalidArgsAmountRistrettoSub_init()
{
    unsigned char point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point );
    char* testArgs[] = {( char* )point};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = ristrettosub_init( &initid, &args, message );
    assert( result == true &&
            "Result is not true (1), _init passed when it should have failed." );
    assert( strcmp( message, "requires 2 binary string arguments" ) == 0 &&
            "Error message is incorrect" );
    printf( "testInvalidArgsAmountRistrettoSub_init() passed assertions!\n" );
}

void testInvalidFirstArgPointRistrettoSub_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point1[i] = i;
    }
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = ristrettosub_init( &initid, &args, message );
    assert( result == false &&
            "Result is not false (0), _init failed when it should have passed." );
    printf( "testInvalidFirstArgPointRistrettoSub_init() passed assertions!\n" );
}

void testInvalidSecondArgPointRistrettoSub_init()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point2[i] = i;
    }
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    const my_bool result = ristrettosub_init( &initid, &args, message );
    assert( result == false &&
            "Result is not false (0), _init failed when it should have passed." );
    printf( "testInvalidSecondArgPointRistrettoSub_init() passed assertions!\n" );
}

void testRistrettoSub()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = ristrettosub( &initid, &args, result, length, is_null,
                                            error );
    assert( returnedPtr == result &&
            "Returned pointer does not originate from the result argument" );
    assert( crypto_core_ristretto255_is_valid_point( ( unsigned char* )
            result ) == 1 &&
            "Output of the ristrettosub() is not a valid ristretto point" );
    char expecteRistrettoPoint[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_sub( ( unsigned char* )expecteRistrettoPoint, point1,
                                  point2 );
    assert( memcmp( expecteRistrettoPoint, result,
                    crypto_core_ristretto255_BYTES ) == 0 &&
            "Output of the ristrettosub() is not as expected" );
    printf( "testRistrettoSub() passed assertions!\n" );
}

void testInvalidFirstArgPointRistrettoSub()
{
    sodium_init();
    unsigned char point1[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point1[i] = i;
    }
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = ristrettosub( &initid, &args, result, length, is_null,
                                            error );
    assert( returnedPtr == NULL &&
            "ristrettosub() did not return NULL on invalid first input argument" );
    printf( "testInvalidFirstArgPointRistrettoSub() passed assertions!\n" );
}

void testInvalidSecondArgPointRistrettoSub()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[crypto_core_ristretto255_BYTES];
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        point2[i] = i;
    }
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = ristrettosub( &initid, &args, result, length, is_null,
                                            error );
    assert( returnedPtr == NULL &&
            "ristrettosub() did not return NULL on invalid second input argument" );
    printf( "testInvalidSecondArgPointRistrettoSub() passed assertions!\n" );
}

void testInvalidFirstArgSizeRistrettoSub()
{
    unsigned char point1[16];
    for( size_t i = 0; i < 16; i++ ) {
        point1[i] = i;
    }
    unsigned char point2[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point2 );
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {16, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = ristrettosub( &initid, &args, result, length, is_null,
                                            error );
    assert( returnedPtr == NULL &&
            "ristrettosub() did not return NULL on invalid first input argument" );
    printf( "testInvalidFirstArgSizeRistrettoSub() passed assertions!\n" );
}

void testInvalidSecondArgSizeRistrettoSub()
{
    unsigned char point1[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( point1 );
    unsigned char point2[16];
    for( size_t i = 0; i < 16; i++ ) {
        point2[i] = i;
    }
    char* testArgs[] = {( char* )point1, ( char* )point2};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, 16};
    enum Item_result itemValue[2] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = {.arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    const UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0
    };
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = ristrettosub( &initid, &args, result, length, is_null,
                                            error );
    assert( returnedPtr == NULL &&
            "ristrettosub() did not return NULL on invalid second input argument" );
    printf( "testInvalidSecondArgSizeRistrettoSub() passed assertions!\n" );
}

int main()
{
    testRistrettoSub_init();
    testInvalidArgsAmountRistrettoSub_init();
    testInvalidFirstArgPointRistrettoSub_init();
    testInvalidSecondArgPointRistrettoSub_init();
    testRistrettoSub();
    testInvalidFirstArgPointRistrettoSub();
    testInvalidSecondArgPointRistrettoSub();
    testInvalidFirstArgSizeRistrettoSub();
    testInvalidSecondArgSizeRistrettoSub();
    return 0;
}