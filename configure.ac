dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(01847-test, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AM_CONFIG_HEADER(config.h)

AC_CHECK_LIB(hidapi-hidraw, hid_write, ,AC_MSG_ERROR([hidapi lib not found]))
AC_CHECK_HEADER(hidapi/hidapi.h)

AC_PROG_CC

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

