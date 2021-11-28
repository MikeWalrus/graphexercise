ACLOCAL_AMFLAGS = -I m4 --install

lib_LTLIBRARIES = libgraphexercise.la
libgraphexercise_la_SOURCES = \
	src/adjacency_matrix.c

include_HEADERS = src/adjacency_matrix.h

TESTS = check_graphexercise
check_PROGRAMS = check_graphexercise

check_graphexercise_SOURCES = tests/check_graphexercise.c
check_graphexercise_CFLAGS = @CHECK_CFLAGS@
check_graphexercise_LDADD = libgraphexercise.la @CHECK_LIBS@
