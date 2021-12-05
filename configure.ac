# Process this file with autoconf to produce a configure script.

# Prelude.
AC_PREREQ([2.71])
AC_INIT([graphexercise],[0.1],[liaojunxuan@whu.edu.cn])
AM_PROG_AR

AC_CONFIG_AUX_DIR([build_aux])
AC_CONFIG_MACRO_DIRS([m4])

# unique source file --- primitive safety check 
AC_CONFIG_SRCDIR([src/adjacency_matrix.c])

# fairly severe build strictness
# change foreign to gnu or gnits to comply with gnu standards
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign 1.11.2])

# Checks for programs.
AC_PROG_CC
LT_INIT

# Checks for libraries.

PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])
AM_PROG_CC_C_O

# Checks for header files.

AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

# Output files
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile])

# Doxygen
DX_INIT_DOXYGEN([graphexercise])

AC_OUTPUT
