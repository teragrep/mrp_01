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

#include "../lib_sodium_ristretto_scalar_random.h"

void testRistrettoScalarRandom_init()
{
    char* testArgs[] = {0};
    unsigned long testLengths[1] = {0};
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 0, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null = 0};
    UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = NULL, .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    const my_bool result = ristrettoscalarrandom_init( &initid, &args, message );
    assert( result == false &&
            "Result is not false (0), _init failed when it should have passed." );
    printf( "testRistrettoScalarRandom_init() passed assertions!\n" );
}

void testRistrettoScalarRandom()
{
    char result[255];
    unsigned long length[1];
    char error[1];
    char is_null[1];
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 0, .arg_type = itemValue, .args = 0, .lengths = 0, .maybe_null = 0};
    const UDF_INIT initid = {.maybe_null = 0, .decimals = 3, .max_length = crypto_core_ristretto255_SCALARBYTES, .ptr = NULL, .const_item = 0};
    const char* returnedPtr = ristrettoscalarrandom( &initid, &args, result, length,
                              is_null, error );
    assert( returnedPtr == result &&
            "Returned pointer does not originate from the result argument" );
    char* zeroArray = calloc( crypto_core_ristretto255_SCALARBYTES,
                              sizeof( char ) );
    assert( memcmp( zeroArray, result, crypto_core_ristretto255_SCALARBYTES ) < 0 &&
            "ristrettoscalarrandom() did not populate the output with a random 32-byte scalar" );
    printf( "testRistrettoScalarRandom() passed assertions!\n" );
    free( zeroArray );
}

int main()
{
    testRistrettoScalarRandom_init();
    testRistrettoScalarRandom();
    return 0;
}