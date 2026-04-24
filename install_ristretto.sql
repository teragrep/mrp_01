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