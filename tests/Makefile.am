check_PROGRAMS = test_libknxusb
TESTS = test_libknxusb

test_libknxusb_SOURCES = \
	test_libknxusb.c 

test_libknxusb_CFLAGS = @CMOCKA_CFLAGS@
test_libknxusb_LDADD = $(top_builddir)/libknxusb/libknxusb.la @CMOCKA_LIBS@
