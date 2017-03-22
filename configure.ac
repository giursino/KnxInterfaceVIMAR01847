dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(01847-test, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_CHECK_HEADERS([hidapi/hidapi.h])
AC_CHECK_LIB(hidapi-hidraw, hid_write)

AC_PROG_CC

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

