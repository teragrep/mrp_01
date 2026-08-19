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
#include <sodium.h>
#include <string.h>
#include <mysql/mysql.h>
#include "lib_sodium_ristretto_hash.h"
#include <stdbool.h>

// Hash-to-group

my_bool ristrettofromhash_init( UDF_INIT* initid, const UDF_ARGS* args,
                                char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "requires 1 binary string argument" );
        return true;
    }
    if( args->lengths[0] != crypto_core_ristretto255_HASHBYTES ) {
        strcpy( message, "First input argument is not a 64 byte binary string" );
        return true;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return true;
    }
    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == NULL ) {
        strcpy( message, "not enough memory for buffer" );
        return true;
    }

    return false;
}

void ristrettofromhash_deinit( UDF_INIT* initid )
{
    if( initid->ptr != NULL ) {
        free( initid->ptr );
        initid->ptr = NULL;
    }
}

char* ristrettofromhash( const UDF_INIT* initid, const UDF_ARGS* args,
                         char* result,
                         unsigned long* length, char* is_null, char* error )
{
    const unsigned char* hash1 = ( const unsigned char* )args->args[0];
    crypto_core_ristretto255_from_hash( ( unsigned char* )initid->ptr, hash1 );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}