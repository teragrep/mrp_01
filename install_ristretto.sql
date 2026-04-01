USE mysql;

DROP FUNCTION IF EXISTS ristretto;
CREATE FUNCTION ristretto RETURNS STRING SONAME 'lib_sodium_ristretto.so';