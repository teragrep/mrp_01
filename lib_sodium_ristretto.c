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
*/
#include <sodium.h>
#include <string.h>
#include <mysql/mysql.h>
#include "lib_sodium_ristretto.h"

#ifndef my_bool
typedef char my_bool;
#endif

// Encoded element validation

my_bool ristrettoisvalidpoint_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_BYTES ) {
        strcpy( message, "First input argument is not a 32 byte binary string" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    return 0;
}

long long ristrettoisvalidpoint( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_BYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    if( crypto_core_ristretto255_is_valid_point( r ) == 0 ) {
        return 0;
    }
    return 1;
}

// Random group element

my_bool ristrettorandom_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettorandom_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettorandom( const UDF_INIT* initid, UDF_ARGS* args, char* result,
                       unsigned long* length, char* is_null, char* error )
{
    unsigned char p[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_random( p );
    memcpy( initid->ptr, p, crypto_core_ristretto255_BYTES );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

// Hash-to-group

my_bool ristrettofromhash_init( UDF_INIT* initid, const UDF_ARGS* args,
                                char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_HASHBYTES ) {
        strcpy( message, "First input argument is not a 64 byte binary string" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }

    return 0;
}

void ristrettofromhash_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettofromhash( const UDF_INIT* initid, const UDF_ARGS* args,
                         char* result,
                         unsigned long* length, char* is_null, char* error )
{
    unsigned char x[crypto_core_ristretto255_HASHBYTES];
    memcpy( x, args->args[0], args->lengths[0] );
    unsigned char yy[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash( yy, x );
    memcpy( initid->ptr, yy, crypto_core_ristretto255_BYTES );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

// Scalar multiplication

my_bool scalarmultristretto_init( UDF_INIT* initid, const UDF_ARGS* args,
                                  char* message )
{
    if( args->arg_count != 2 ||  args->arg_type[0] != STRING_RESULT  ||
            args->arg_type[1] != STRING_RESULT ) {
        strcpy( message, "requires 2 binary string arguments" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input argument is not a 32 byte scalar in binary string format" );
        return 1;
    }
    if( args->lengths[1] != crypto_core_ristretto255_BYTES ) {
        strcpy( message, "Second input argument is not a 32 byte binary string" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    unsigned char arg1[crypto_core_ristretto255_BYTES];
    memcpy( arg1, args->args[1], args->lengths[1] );
    if( crypto_core_ristretto255_is_valid_point( arg1 ) == 0 ) {
        strcpy( message, "Second input argument is not a valid ristretto point" );
        return 1;
    }

    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void scalarmultristretto_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* scalarmultristretto( const UDF_INIT* initid, const UDF_ARGS* args,
                           char* result,
                           unsigned long* length, char* is_null, char* error )
{
    unsigned char n[crypto_core_ristretto255_SCALARBYTES];
    memcpy( n, args->args[0], args->lengths[0] );
    unsigned char p[crypto_core_ristretto255_BYTES];
    memcpy( p, args->args[1], args->lengths[1] );
    unsigned char q[crypto_core_ristretto255_BYTES];
    if( crypto_scalarmult_ristretto255( q, n, p ) != 0 ) {
        *error = 1;
        return 0;
    }
    memcpy( initid->ptr, q, crypto_core_ristretto255_BYTES );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

my_bool scalarmultristrettobase_init( UDF_INIT* initid, const UDF_ARGS* args,
                                      char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT )  {
        strcpy( message, "requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input argument is not a 32 byte scalar in binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void scalarmultristrettobase_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* scalarmultristrettobase( const UDF_INIT* initid, const UDF_ARGS* args,
                               char* result,
                               unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    unsigned char gr[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255_base( gr, r );
    memcpy( initid->ptr, gr, crypto_core_ristretto255_BYTES );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

// Element addition/subtraction

my_bool ristrettoadd_init( UDF_INIT* initid, const UDF_ARGS* args,
                           char* message )
{
    if( args->arg_count != 2 ||  args->arg_type[0] != STRING_RESULT  ||
            args->arg_type[1] != STRING_RESULT ) {
        strcpy( message, "requires 2 binary string arguments" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_BYTES ) {
        strcpy( message, "First input argument is not a 32 byte binary string" );
        return 1;
    }
    if( args->lengths[1] != crypto_core_ristretto255_BYTES ) {
        strcpy( message, "Second input argument is not a 32 byte binary string" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    unsigned char arg0[crypto_core_ristretto255_BYTES];
    memcpy( arg0, args->args[0], args->lengths[0] );
    if( crypto_core_ristretto255_is_valid_point( arg0 ) == 0 ) {
        strcpy( message, "First input is not a valid ristretto point" );
        return 1;
    }
    unsigned char arg1[crypto_core_ristretto255_BYTES];
    memcpy( arg1, args->args[1], args->lengths[1] );
    if( crypto_core_ristretto255_is_valid_point( arg1 ) == 0 ) {
        strcpy( message, "Second input is not a valid ristretto point" );
        return 1;
    }


    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoadd_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoadd( const UDF_INIT* initid, const UDF_ARGS* args, char* result,
                    unsigned long* length, char* is_null, char* error )
{
    unsigned char gr[crypto_core_ristretto255_BYTES];
    memcpy( gr, args->args[0], args->lengths[0] );
    unsigned char px[crypto_core_ristretto255_BYTES];
    memcpy( px, args->args[1], args->lengths[1] );
    unsigned char a[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_add( a, px, gr );
    memcpy( initid->ptr, a, crypto_core_ristretto255_BYTES );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

my_bool ristrettosub_init( UDF_INIT* initid, const UDF_ARGS* args,
                           char* message )
{
    if( args->arg_count != 2 ||  args->arg_type[0] != STRING_RESULT  ||
            args->arg_type[1] != STRING_RESULT ) {
        strcpy( message, "requires 2 binary string arguments" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_BYTES ) {
        strcpy( message, "First input argument is not a 32 byte binary string" );
        return 1;
    }
    if( args->lengths[1] != crypto_core_ristretto255_BYTES ) {
        strcpy( message, "Second input argument is not a 32 byte binary string" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    unsigned char arg0[crypto_core_ristretto255_BYTES];
    memcpy( arg0, args->args[0], args->lengths[0] );
    if( crypto_core_ristretto255_is_valid_point( arg0 ) == 0 ) {
        strcpy( message, "First input is not a valid ristretto point" );
        return 1;
    }
    unsigned char arg1[crypto_core_ristretto255_BYTES];
    memcpy( arg1, args->args[1], args->lengths[1] );
    if( crypto_core_ristretto255_is_valid_point( arg1 ) == 0 ) {
        strcpy( message, "Second input is not a valid ristretto point" );
        return 1;
    }


    initid->ptr = malloc( crypto_core_ristretto255_BYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettosub_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettosub( const UDF_INIT* initid, const UDF_ARGS* args, char* result,
                    unsigned long* length, char* is_null, char* error )
{
    unsigned char gr[crypto_core_ristretto255_BYTES];
    memcpy( gr, args->args[0], args->lengths[0] );
    unsigned char px[crypto_core_ristretto255_BYTES];
    memcpy( px, args->args[1], args->lengths[1] );
    unsigned char a[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_sub( a, px, gr );
    memcpy( initid->ptr, a, crypto_core_ristretto255_BYTES );
    *length = crypto_core_ristretto255_BYTES;
    return initid->ptr;
}

// Scalar arithmetic over L

my_bool ristrettoscalarrandom_init( UDF_INIT* initid, UDF_ARGS* args,
                                    char* message )
{
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarrandom_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarrandom( const UDF_INIT* initid, UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random( r );
    memcpy( initid->ptr, r, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarreduce_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_NONREDUCEDSCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 64 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarreduce_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarreduce( const UDF_INIT* initid, const UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null, char* error )
{
    unsigned char s[crypto_core_ristretto255_NONREDUCEDSCALARBYTES];
    memcpy( s, args->args[0], args->lengths[0] );
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_reduce( r, s );
    memcpy( initid->ptr, r, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarinvert_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarinvert_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarinvert( const UDF_INIT* initid, const UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null, char* error )
{
    unsigned char s[crypto_core_ristretto255_SCALARBYTES];
    memcpy( s, args->args[0], args->lengths[0] );
    unsigned char recip[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_invert( recip, s );
    memcpy( initid->ptr, recip, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarnegate_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "Requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarnegate_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarnegate( const UDF_INIT* initid, const UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    unsigned char ir[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_negate( ir, r );
    memcpy( initid->ptr, ir, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarcomplement_init( UDF_INIT* initid, const UDF_ARGS* args,
                                        char* message )
{
    if( args->arg_count != 1 ||  args->arg_type[0] != STRING_RESULT ) {
        strcpy( message, "requires 1 binary string argument" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarcomplement_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarcomplement( const UDF_INIT* initid, const UDF_ARGS* args,
                                 char* result,
                                 unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    unsigned char ir[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_complement( ir, r );
    memcpy( initid->ptr, ir, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalaradd_init( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* message )
{
    if( args->arg_count != 2 || args->arg_type[0] != STRING_RESULT ||
            args->arg_type[1] != STRING_RESULT ) {
        strcpy( message, "requires 2 binary string arguments" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( args->lengths[1] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "Second input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalaradd_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalaradd( const UDF_INIT* initid, const UDF_ARGS* args,
                          char* result,
                          unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    memcpy( b, args->args[1], args->lengths[1] );
    unsigned char ir[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_add( ir, r, b );
    memcpy( initid->ptr, ir, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarsub_init( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* message )
{
    if( args->arg_count != 2 || args->arg_type[0] != STRING_RESULT ||
            args->arg_type[1] != STRING_RESULT ) {
        strcpy( message, "requires 2 binary string arguments" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( args->lengths[1] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "Second input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarsub_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarsub( const UDF_INIT* initid, const UDF_ARGS* args,
                          char* result,
                          unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    memcpy( b, args->args[1], args->lengths[1] );
    unsigned char ir[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_sub( ir, r, b );
    memcpy( initid->ptr, ir, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}

my_bool ristrettoscalarmul_init( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* message )
{
    if( args->arg_count != 2 || args->arg_type[0] != STRING_RESULT ||
            args->arg_type[1] != STRING_RESULT ) {
        strcpy( message, "requires 2 binary string arguments" );
        return 1;
    }
    if( args->lengths[0] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "First input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( args->lengths[1] != crypto_core_ristretto255_SCALARBYTES ) {
        strcpy( message,
                "Second input is not a scalar in 32 byte binary string format" );
        return 1;
    }
    if( sodium_init() == -1 ) {
        strcpy( message, "sodium failed to initialize" );
        return 1;
    }
    initid->ptr = malloc( crypto_core_ristretto255_SCALARBYTES );
    if( initid->ptr == 0 ) {
        strcpy( message, "not enough memory for buffer" );
        return 1;
    }
    return 0;
}

void ristrettoscalarmul_deinit( UDF_INIT* initid )
{
    if( initid->ptr != 0 ) {
        free( initid->ptr );
        initid->ptr = 0;
    }
}

char* ristrettoscalarmul( const UDF_INIT* initid, const UDF_ARGS* args,
                          char* result,
                          unsigned long* length, char* is_null, char* error )
{
    unsigned char r[crypto_core_ristretto255_SCALARBYTES];
    memcpy( r, args->args[0], args->lengths[0] );
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    memcpy( b, args->args[1], args->lengths[1] );
    unsigned char ir[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_mul( ir, r, b );
    memcpy( initid->ptr, ir, crypto_core_ristretto255_SCALARBYTES );
    *length = crypto_core_ristretto255_SCALARBYTES;
    return initid->ptr;
}