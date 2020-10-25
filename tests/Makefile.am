# Programs to build
check_PROGRAMS = \
	test_libknxusb \
	test_01847_temp

# Programs to run on "make check"
TESTS = \
	test_libknxusb \
	test_01847_temp

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
test_01847_temp_SOURCES = \
	test_01847_temp.c

test_01847_temp_CFLAGS = \
	@CMOCKA_CFLAGS@ \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/example

test_01847_temp_LDADD = \
	@CMOCKA_LIBS@

#############################

# Files to clean
MAINTAINERCLEANFILES = \
	Makefile.in