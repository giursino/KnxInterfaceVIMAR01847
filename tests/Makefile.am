check_PROGRAMS = test_libknxusb
TESTS = test_libknxusb

test_libknxusb_SOURCES = \
	test_libknxusb.c 

test_libknxusb_CFLAGS = @CMOCKA_CFLAGS@ -I$(top_srcdir)/include -I$(top_srcdir)/libknxusb
test_libknxusb_LDADD = @CMOCKA_LIBS@
#test_libknxusb_LDADD = @CMOCKA_LIBS@ $(top_builddir)/libknxusb/libknxusb.la

MAINTAINERCLEANFILES = Makefile.in