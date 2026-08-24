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
USE mysql;

DROP FUNCTION IF EXISTS ristrettoisvalidpoint;
CREATE FUNCTION ristrettoisvalidpoint RETURNS INTEGER SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettorandom;
CREATE FUNCTION ristrettorandom RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettofromhash;
CREATE FUNCTION ristrettofromhash RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS scalarmultristretto;
CREATE FUNCTION scalarmultristretto RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS scalarmultristrettobase;
CREATE FUNCTION scalarmultristrettobase RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoadd;
CREATE FUNCTION ristrettoadd RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettosub;
CREATE FUNCTION ristrettosub RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarrandom;
CREATE FUNCTION ristrettoscalarrandom RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarreduce;
CREATE FUNCTION ristrettoscalarreduce RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarinvert;
CREATE FUNCTION ristrettoscalarinvert RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarnegate;
CREATE FUNCTION ristrettoscalarnegate RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarcomplement;
CREATE FUNCTION ristrettoscalarcomplement RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalaradd;
CREATE FUNCTION ristrettoscalaradd RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarsub;
CREATE FUNCTION ristrettoscalarsub RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarmul;
CREATE FUNCTION ristrettoscalarmul RETURNS STRING SONAME 'lib_sodium_ristretto.so';