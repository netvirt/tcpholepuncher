AC_INIT([libthp], [1.0], [franck.rupin@gmail.com])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AC_DEFINE(UHP_DEBUG_MODE, 1, [Define to  enable debug mode.])
CFLAGS="$CFLAGS -g3 -O0 -std=gnu99"
AM_INIT_AUTOMAKE([1.13 subdir-objects -Wall -Werror foreign])
AC_PROG_CC

AM_PROG_AR([act-if-fail])
LT_INIT

PKG_CHECK_MODULES([libevent], [libevent])
PKG_CHECK_MODULES([jansson], [jansson])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([crypto], [libcrypto])
PKG_CHECK_MODULES([ssl], [libssl])

AC_CHECK_FILE(/dev/random, 
	      [AC_DEFINE(HAVE_DEVRAND, 1, [can use dev random])],[])

	
AC_CHECK_FILE(/dev/urandom,
	      [AC_DEFINE(HAVE_DEVURAND, 1, [can use dev urandom])],[])
	
AC_CONFIG_SRCDIR([punchctl.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
