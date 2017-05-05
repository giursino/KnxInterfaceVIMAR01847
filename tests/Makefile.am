check_PROGRAMS = test_libknxusb
TESTS = test_libknxusb

test_libknxusb_SOURCES = \
	test_libknxusb.c 
#	../libknxusb/libknxusb.c \
#	../libknxusb/libknxusb_priv.h

test_libknxusb_CFLAGS = @CMOCKA_CFLAGS@ -I$(top_srcdir)/include
#test_libknxusb_LDADD = @CMOCKA_LIBS@
test_libknxusb_LDADD = $(top_builddir)/libknxusb/libknxusb.la @CMOCKA_LIBS@

MAINTAINERCLEANFILES = Makefile.in