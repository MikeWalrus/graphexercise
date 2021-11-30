ACLOCAL_AMFLAGS = -I m4 --install

lib_LTLIBRARIES = libgraphexercise.la
libgraphexercise_la_SOURCES = \
							  src/adjacency_matrix.c src/adjacency_matrix.h \
							  src/adjacency_list.c src/adjacency_list.h \
							  src/vec.c src/vec.h \
							  src/utilities.h

pkginclude_HEADERS = \
					 src/adjacency_matrix.h \
					 src/adjacency_list.h

TESTS = check_graphexercise check_vec
check_PROGRAMS = check_graphexercise check_vec

check_graphexercise_SOURCES = tests/check_graphexercise.c
check_graphexercise_CFLAGS = @CHECK_CFLAGS@
check_graphexercise_LDADD = libgraphexercise.la @CHECK_LIBS@

check_vec_SOURCES = tests/check_vec.c
check_vec_CFLAGS = @CHECK_CFLAGS@
check_vec_LDADD = libgraphexercise.la @CHECK_LIBS@
