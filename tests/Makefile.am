# Programs to build
check_PROGRAMS = \
	test_libknxusb \
	test_knxutils

# Programs to run on "make check"
TESTS = \
	test_libknxusb \
	test_knxutils

#############################
test_libknxusb_SOURCES = \
	test_libknxusb.c 

test_libknxusb_CFLAGS = \
	@CMOCKA_CFLAGS@ \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libknxusb

test_libknxusb_LDADD = \
	@CMOCKA_LIBS@
#	$(top_builddir)/libknxusb/libknxusb.la

#############################
test_knxutils_SOURCES = \
	test_knxutils.c

test_knxutils_CFLAGS = \
	@CMOCKA_CFLAGS@ \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libknxusb

test_knxutils_LDADD = \
	@CMOCKA_LIBS@ \
	-lm

#############################

# Files to clean
MAINTAINERCLEANFILES = \
	Makefile.in