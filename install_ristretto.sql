USE mysql;

DROP FUNCTION IF EXISTS ristrettofromhash;
CREATE FUNCTION ristrettofromhash RETURNS STRING SONAME 'lib_sodium_ristretto.so';

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

DROP FUNCTION IF EXISTS ristrettoscalarmult;
CREATE FUNCTION ristrettoscalarmult RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarmultbase;
CREATE FUNCTION ristrettoscalarmultbase RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoadd;
CREATE FUNCTION ristrettoadd RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettosub;
CREATE FUNCTION ristrettosub RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettovalidpoint;
CREATE FUNCTION ristrettovalidpoint RETURNS INTEGER SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettorandom;
CREATE FUNCTION ristrettorandom RETURNS STRING SONAME 'lib_sodium_ristretto.so';