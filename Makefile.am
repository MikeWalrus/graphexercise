ACLOCAL_AMFLAGS = -I m4 --install

lib_LTLIBRARIES = libgraphexercise.la
libgraphexercise_la_SOURCES = \
							  src/adjacency_matrix.c src/adjacency_matrix.h \
							  src/adjacency_list.c src/adjacency_list.h \
							  src/vec.c src/vec.h \
							  src/queue.c src/queue.h \
							  src/utilities.h \
							  src/graph.c src/graph.h \
							  src/traverse.c src/traverse.h \
							  src/priority_queue.c src/priority_queue.h \
							  src/mst.c src/mst.h \
							  src/disjoint_set.c src/disjoint_set.h \
							  src/shortest_path.c src/shortest_path.h

pkginclude_HEADERS = \
					 src/adjacency_matrix.h \
					 src/adjacency_list.h \
					 src/traverse.h \
					 src/mst.h \
					 src/shortest_path.h

TESTS = check_graphexercise check_vec check_algorithms check_queue \
		check_priority_queue check_disjoint_set
check_PROGRAMS = check_graphexercise check_vec check_algorithms check_queue \
				 check_priority_queue check_disjoint_set

check_graphexercise_SOURCES = tests/check_graphexercise.c
check_graphexercise_CFLAGS = @CHECK_CFLAGS@
check_graphexercise_LDADD = libgraphexercise.la @CHECK_LIBS@

check_vec_SOURCES = tests/check_vec.c
check_vec_CFLAGS = @CHECK_CFLAGS@
check_vec_LDADD = libgraphexercise.la @CHECK_LIBS@

check_queue_SOURCES = tests/check_queue.c
check_queue_CFLAGS = @CHECK_CFLAGS@
check_queue_LDADD = libgraphexercise.la @CHECK_LIBS@

check_algorithms_SOURCES = tests/check_algorithms.c
check_algorithms_CFLAGS = @CHECK_CFLAGS@ -DDATASET_DIR="\"$(abs_top_srcdir)/tests/datasets/\""
check_algorithms_LDADD = libgraphexercise.la @CHECK_LIBS@

check_priority_queue_SOURCES = tests/check_priority_queue.c
check_priority_queue_CFLAGS = @CHECK_CFLAGS@
check_priority_queue_LDADD = libgraphexercise.la @CHECK_LIBS@

check_disjoint_set_SOURCES = tests/check_disjoint_set.c
check_disjoint_set_CFLAGS = @CHECK_CFLAGS@
check_disjoint_set_LDADD = libgraphexercise.la @CHECK_LIBS@

EXTRA_DIST = tests/datasets

@DX_RULES@
$(DX_DOCDIR)/$(PACKAGE).tag: $(libgraphexercise_la_SOURCES)

MOSTLYCLEANFILES = $(DX_CLEANFILES)
