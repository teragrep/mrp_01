# Build a library, lib_sodium_ristretto.la.
lib_LTLIBRARIES = lib_sodium_ristretto.la
lib_sodium_ristretto_la_SOURCES = lib_sodium_ristretto.c
lib_sodium_ristretto_la_CFLAGS = -DSTANDARD -DMYSQL_SERVER @MYSQL_CFLAGS@
lib_sodium_ristretto_la_CXXFLAGS = -DSTANDARD -DMYSQL_SERVER @MYSQL_CFLAGS@
lib_sodium_ristretto_la_LDFLAGS = -module -avoid-version -no-undefined -rpath @MYSQL_PLUGINDIR@
