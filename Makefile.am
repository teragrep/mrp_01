# Build a library, lib_sodium_ristretto.la.
lib_sodium_ristretto_ladir = $(pkgincludedir)
lib_LTLIBRARIES = lib_sodium_ristretto.la
lib_sodium_ristretto_la_SOURCES = lib_sodium_ristretto.c
lib_sodium_ristretto_la_HEADERS = lib_sodium_ristretto.h
lib_sodium_ristretto_la_CFLAGS = -DSTANDARD -DMYSQL_SERVER @MYSQL_CFLAGS@
lib_sodium_ristretto_la_CXXFLAGS = -DSTANDARD -DMYSQL_SERVER @MYSQL_CFLAGS@
lib_sodium_ristretto_la_LDFLAGS = -module -avoid-version -no-undefined -rpath @MYSQL_PLUGINDIR@
lib_sodium_ristretto_la_LIBADD = -lsodium

RISTRETTO_BUILT_TESTS = tests/tests_ristretto_isvalidpoint tests/tests_ristretto_random tests/tests_ristretto_from_hash tests/tests_scalarmult_ristretto tests/tests_scalarmult_ristrettobase tests/tests_ristretto_add tests/tests_ristretto_sub tests/tests_ristretto_scalar_random tests/tests_ristretto_scalar_reduce tests/tests_ristretto_scalar_invert
TESTS = $(RISTRETTO_BUILT_TESTS)
check_PROGRAMS = $(RISTRETTO_BUILT_TESTS)

tests_tests_ristretto_isvalidpoint_SOURCES = tests/tests_ristretto_isvalidpoint.c
tests_tests_ristretto_isvalidpoint_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_random_SOURCES = tests/tests_ristretto_random.c
tests_tests_ristretto_random_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_from_hash_SOURCES = tests/tests_ristretto_from_hash.c
tests_tests_ristretto_from_hash_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_scalarmult_ristretto_SOURCES = tests/tests_scalarmult_ristretto.c
tests_tests_scalarmult_ristretto_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_scalarmult_ristrettobase_SOURCES = tests/tests_scalarmult_ristrettobase.c
tests_tests_scalarmult_ristrettobase_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_add_SOURCES = tests/tests_ristretto_add.c
tests_tests_ristretto_add_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_sub_SOURCES = tests/tests_ristretto_sub.c
tests_tests_ristretto_sub_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_scalar_random_SOURCES = tests/tests_ristretto_scalar_random.c
tests_tests_ristretto_scalar_random_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_scalar_reduce_SOURCES = tests/tests_ristretto_scalar_reduce.c
tests_tests_ristretto_scalar_reduce_LDADD = -lsodium lib_sodium_ristretto.la

tests_tests_ristretto_scalar_invert_SOURCES = tests/tests_ristretto_scalar_invert.c
tests_tests_ristretto_scalar_invert_LDADD = -lsodium lib_sodium_ristretto.la

EXTRA_DIST = *.h *.sql

mrproper:
	make clean
	make maintainer-clean
	rm -rf config.guess config.h.* config.status configure config.sub ltmain.sh depcomp aclocal.m4 install.sh config.log compile missing Makefile.in doc/html *.lo mkinstalldirs install-sh *~ config/ltmain.sh config/libtool.m4 config/ltoptions.m4 config/ltversion.m4 config/lt~obsolete.m4

installdb:
	$(MYSQL) < ./install_ristretto.sql

uninstalldb:
	$(MYSQL) < ./uninstall_ristretto.sql

dist-hook:
	find $(distdir) -name .svn -o -name .git* -o -name .c9* -o -name .DS_Store -o -name .cdtproject -o -name .project -o -name .settings -o *~ | xargs --no-run-if-empty rm -rf

maintainer-dist:
	make dist
	git tag "$(VERSION)"

maintainer-diff:
	git diff "$(DIFFVERSION)" . | gitx

.PHONY : mrproper