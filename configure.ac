dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(01847-test, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([subdir-objects])

AM_CONFIG_HEADER(config.h)

PKG_CHECK_MODULES([CMOCKA], [cmocka >= 0.0.1])

AC_CHECK_LIB(hidapi-hidraw, hid_write, ,AC_MSG_ERROR([hidapi lib not found]))
AC_CHECK_HEADER(hidapi/hidapi.h)

ACX_PTHREAD
AC_CHECK_LIB(pthread, pthread_create, [PTHREAD_LIBS+=-lpthread])

AX_WITH_CURSES

AC_PROG_CC
AC_PROG_LIBTOOL

AC_CONFIG_FILES(
	Makefile 
	example/Makefile
	libknxusb/Makefile
	include/Makefile
	tests/Makefile
)
AC_OUTPUT

