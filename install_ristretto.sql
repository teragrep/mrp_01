USE mysql;

DROP FUNCTION IF EXISTS ristrettofromhash;
CREATE FUNCTION ristrettofromhash RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarrandom;
CREATE FUNCTION ristrettoscalarrandom RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoscalarmultbase;
CREATE FUNCTION ristrettoscalarmultbase RETURNS STRING SONAME 'lib_sodium_ristretto.so';

DROP FUNCTION IF EXISTS ristrettoadd;
CREATE FUNCTION ristrettoadd RETURNS STRING SONAME 'lib_sodium_ristretto.so';