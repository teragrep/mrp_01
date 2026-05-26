/*
    MariaDB Ristretto255 Plugin (mrp_01)
    Copyright (C) 2021-2024 Suomen Kanuuna Oy

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
*/#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testRistrettofromhash_init()
{
    unsigned char* hash = malloc( crypto_core_ristretto255_HASHBYTES );
    assert( hash != 0 );
    for( size_t i = 0; i < crypto_core_ristretto255_HASHBYTES; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {( char* ) hash};
    unsigned long testLengths[1] = {crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {
        .maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0
    };
    const my_bool result = ristrettofromhash_init( &initid, &args, message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != 0 && "Memory was not succesfully allocated" );
    free( hash );
    free( initid.ptr );
    printf( "testRistrettofromhash_init() passed assertions!\n" );
}

void testInvalidArgSizeRistrettofromhash_init()
{
    unsigned char* hash = malloc( 32 );
    assert( hash != 0 );
    for( size_t i = 0; i < 32; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {( char* ) hash};
    unsigned long testLengths[1] = {32};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    const my_bool result = ristrettofromhash_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input argument is not a 64 byte binary string" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    free( hash );
    printf( "testInvalidArgSizeRistrettofromhash_init() passed assertions!\n" );
}

void testInvalidArgAmountRistrettofromhash_init()
{
    unsigned char* hash = malloc( crypto_core_ristretto255_HASHBYTES );
    assert( hash != 0 );
    for( size_t i = 0; i < crypto_core_ristretto255_HASHBYTES; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {( char* ) hash, ( char* ) hash};
    unsigned long testLengths[2] = {crypto_core_ristretto255_HASHBYTES, crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char message[MYSQL_ERRMSG_SIZE];
    const UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    const my_bool result = ristrettofromhash_init( &initid, &args, message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    free( hash );
    printf( "testInvalidArgAmountRistrettofromhash_init() passed assertions!\n" );
}

void testRistrettofromhash_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = 0, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_BYTES );
    assert( initid.ptr != 0 );
    ristrettofromhash_deinit( &initid );
    assert( initid.ptr == 0 && "_deinit failed to free the allocated memory." );
    printf( "testRistrettofromhash_deinit() passed assertions!\n" );
}

void testRistrettofromhash()
{
    unsigned char* hash = malloc( crypto_core_ristretto255_HASHBYTES );
    assert( hash != 0 );
    for( size_t i = 0; i < crypto_core_ristretto255_HASHBYTES; i++ ) {
        hash[i] = rand();
    }
    char* testArgs[] = {( char* ) hash};
    unsigned long testLengths[1] = {crypto_core_ristretto255_HASHBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    char* ristrettoPoint = malloc( crypto_core_ristretto255_BYTES );
    assert( ristrettoPoint != 0 );
    const UDF_ARGS args = {.arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    const UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_BYTES, .ptr = ristrettoPoint, .const_item = 0};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    const char* returnedPtr = ristrettofromhash( &initid, &args, result, length,
                              is_null,
                              error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    assert( crypto_core_ristretto255_is_valid_point( ( unsigned char* )
            ristrettoPoint ) == 1 &&
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