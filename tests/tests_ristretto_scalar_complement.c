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

void testRistrettoScalarComplement_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* ) scalar};
    unsigned long testLengths[1] = {crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristrettoscalarcomplement_init( &initid, &args,
                           message );
    assert( result == 0 &&
            "Result is not 0, _init failed when it should have passed." );
    assert( initid.ptr != 0 && "Memory was not succesfully allocated" );
    printf( "testRistrettoScalarComplement_init() passed assertions!\n" );
    free( initid.ptr );
}

void testInvalidArgsAmountRistrettoScalarComplement_init()
{
    unsigned char scalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* )scalar, ( char* )scalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES, crypto_core_ristretto255_SCALARBYTES};
    enum Item_result itemValue[] = {STRING_RESULT, STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristrettoscalarcomplement_init( &initid, &args,
                           message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message, "requires 1 binary string argument" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidArgsAmountRistrettoScalarComplement_init() passed assertions!\n" );
}

void testInvalidFirstArgSizeRistrettoScalarComplement_init()
{
    unsigned char scalar[16];
    crypto_core_ristretto255_scalar_random( scalar );
    char* testArgs[] = {( char* ) scalar};
    unsigned long testLengths[] = {16};
    enum Item_result itemValue[] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristrettoscalarcomplement_init( &initid, &args,
                           message );
    assert( result == 1 &&
            "Result is not 1, _init passed when it should have failed." );
    assert( strcmp( message,
                    "First input is not a scalar in 32 byte binary string format" ) == 0 &&
            "Error message is incorrect" );
    assert( initid.ptr == 0 && "Memory was allocated when it shouldn't" );
    printf( "testInvalidFirstArgSizeRistrettoScalarComplement_init() passed assertions!\n" );
}

void testRistrettoScalarComplement_deinit()
{
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = 0, .const_item = 0};
    initid.ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( initid.ptr != 0 );
    ristrettoscalarcomplement_deinit( &initid );
    assert( initid.ptr == 0 && "_deinit failed to free the allocated memory." );
    printf( "testRistrettoScalarComplement_deinit() passed assertions!\n" );
}

void testRistrettoScalarComplement()
{
    unsigned char inputScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( inputScalar );
    char* testArgs[] = {( char* ) inputScalar};
    unsigned long testLengths[] = {crypto_core_ristretto255_SCALARBYTES};
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[1] = {STRING_RESULT};
    const UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    char* scalar = malloc( crypto_core_ristretto255_SCALARBYTES );
    assert( scalar != 0 );
    const UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = scalar, .const_item = 0};
    const char* returnedPtr = ristrettoscalarcomplement( &initid, &args, result,
                              length,
                              is_null, error );
    assert( returnedPtr == initid.ptr &&
            "Returned pointer does not originate from the UDF_INIT struct" );
    char expectedScalar[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_complement( ( unsigned char* ) expectedScalar,
            inputScalar );
    assert( memcmp( expectedScalar, scalar,
                    crypto_core_ristretto255_SCALARBYTES ) == 0 &&
            "Output of the ristrettoscalarcomplement() is not as expected" );
    printf( "testRistrettoScalarComplement() passed assertions!\n" );
    free( scalar );
}

int main()
{
    testRistrettoScalarComplement_init();
    testInvalidArgsAmountRistrettoScalarComplement_init();
    testInvalidFirstArgSizeRistrettoScalarComplement_init();
    testRistrettoScalarComplement_deinit();
    testRistrettoScalarComplement();
    return 0;
}