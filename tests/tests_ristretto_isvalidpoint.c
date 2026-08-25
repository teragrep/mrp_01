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

#include "../lib_sodium_ristretto_validation.h"

void testPassRistrettoisvalidpoint_init()
{
    unsigned char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = i;
    }
    char* testArgs[1] = {( char* ) ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristretto255_is_valid_point_init( &initid, &args,
                           message );
    assert( result == false &&
            "Result is not false (0), _init failed when it should have passed." );
    printf( "testPassRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidArgAmountRistrettoisvalidpoint_init()
{
    unsigned char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = i;
    }
    char* testArgs[2] = {( char* ) ristrettoPoint, ( char* ) ristrettoPoint};
    unsigned long testLengths[2] = {crypto_core_ristretto255_BYTES, crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristretto255_is_valid_point_init( &initid, &args,
                           message );
    assert( result == true &&
            "Result is not true (1), _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    printf( "testInvalidArgAmountRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidArgTypeRistrettoisvalidpoint_init()
{
    unsigned char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = i;
    }
    char* testArgs[1] = {( char* ) ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {INT_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = NULL, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristretto255_is_valid_point_init( &initid, &args,
                           message );
    assert( result == false &&
            "Result is not false (0), _init failed when it should have passed." );
    assert( args.arg_type[0] == STRING_RESULT &&
            "arg_type[0] was not set to STRING_RESULT by ristrettoisvalidpoint_init()" );
    printf( "testInvalidArgTypeRistrettoisvalidpoint_init() passed assertions!\n" );
    free( ristrettoPoint );
}

void testPassRistrettoisvalidpoint()
{
    const int init = sodium_init();
    assert( init >= 0 );
    // Generate a valid ristretto point for validation using crypto_core_ristretto255_random()
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    unsigned char p[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( p );
    memcpy( ristrettoPoint, p, crypto_core_ristretto255_BYTES );

    char* testArgs[1] = {ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = 21, .ptr = NULL, .const_item = 0};
    char error[1];
    char is_null[1];

    const long result = ristretto255_is_valid_point( &initid, &args, is_null,
                        error );
    assert( result == 1 &&
            "Result is not 1, Ristretto point validation failed when it should have passed." );
    printf( "testPassRistrettoisvalidpoint() passed assertions!\n" );
    free( ristrettoPoint );
}

void testInvalidArgSizeRistrettoisvalidpoint()
{
    // Generate an invalid sized ristretto point for validation
    unsigned char* ristrettoPoint = malloc( sizeof( char ) * 16 );
    assert( ristrettoPoint != NULL );
    for( size_t i = 0; i < 16; i++ ) {
        ristrettoPoint[i] = i;
    }
    char* testArgs[1] = {( char* ) ristrettoPoint};
    unsigned long testLengths[1] = {16};
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = 21, .ptr = NULL, .const_item = 0};
    char error[1];
    char is_null[1];
    const long result = ristretto255_is_valid_point( &initid, &args, is_null,
                        error );
    assert( result == 0 &&
            "Result is not 0, Ristretto point validation passed when it should have failed." );
    printf( "testInvalidArgSizeRistrettoisvalidpoint() passed assertions!\n" );
    free( ristrettoPoint );
}

void testFailRistrettoisvalidpoint()
{
    // Generate an invalid ristretto point for validation
    unsigned char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != NULL );
    for( size_t i = 0; i < crypto_core_ristretto255_BYTES; i++ ) {
        ristrettoPoint[i] = i;
    }
    char* testArgs[1] = {( char* ) ristrettoPoint};
    unsigned long testLengths[1] = {crypto_core_ristretto255_BYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = 21, .ptr = NULL, .const_item = 0};
    char error[1];
    char is_null[1];
    const long result = ristretto255_is_valid_point( &initid, &args, is_null,
                        error );
    assert( result == 0 &&
            "Result is not 0, Ristretto point validation passed when it should have failed." );
    printf( "testFailRistrettoisvalidpoint() passed assertions!\n" );
    free( ristrettoPoint );
}

int main()
{
    testPassRistrettoisvalidpoint_init();
    testInvalidArgAmountRistrettoisvalidpoint_init();
    testInvalidArgTypeRistrettoisvalidpoint_init();
    testPassRistrettoisvalidpoint();
    testInvalidArgSizeRistrettoisvalidpoint();
    testFailRistrettoisvalidpoint();
    return 0;
}