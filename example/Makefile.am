#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
bin_PROGRAMS= \
	01847-busmonitor \
	01847-busmonitor-nolib \
	01847-sendmsg \
	01847-sendmsg-nolib \
	01847-temp \
	01847-temp-reader \
	KNXTinySerial-test \
	KNXTinySerial-libtest

# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4




# ------------------------------------------
# Example application for VIMAR 01847 KNX interface.
# It uses ncurse to manage the graphics.
# ------------------------------------------
# Sources
01847_busmonitor_SOURCES= \
	01847-busmonitor.c \
	01847-busmonitor.h \
	common.c \
	common.h

# Libraries
01847_busmonitor_LDADD = \
	$(top_builddir)/libknxusb/libknxusb.la \
	$(PTHREAD_LIBS) \
	$(CURSES_LIBS) \
	$(CURSES_LIB)

# Linker options
01847_busmonitor_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknxusb/.libs

# Compiler options
01847_busmonitor_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(PTHREAD_CFLAGS) \
	$(CURSES_CFLAGS) \
	$(CURSES_CPPFLAGS)



# ------------------------------------------
# Application to send knx message from command line using 01847 interface
# ------------------------------------------
01847_sendmsg_SOURCES= \
	01847-sendmsg.c

01847_sendmsg_LDADD = \
	$(top_builddir)/libknxusb/libknxusb.la

01847_sendmsg_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknxusb/.libs

01847_sendmsg_CPPFLAGS = \
	-I$(top_srcdir)/include




# ------------------------------------------
# Example application to test the KNX Interface VIMAR 01847
# It do not use libknxusb
# It only uses libhid
# ------------------------------------------
01847_busmonitor_nolib_SOURCES= \
	01847-busmonitor-nolib.c



# ------------------------------------------
# Example application to send a message with KNX Interface VIMAR 01847
# It does not use libknxusb, it only use libhid
# ------------------------------------------
01847_sendmsg_nolib_SOURCES= \
	01847-sendmsg-nolib.c



# ------------------------------------------
# Example application to monitor temperature using KNX Interface VIMAR 01847
# ------------------------------------------
01847_temp_SOURCES = \
	01847-temp.c \
	01847-temp.h \
	common.c \
	common.h

01847_temp_LDADD = \
	$(top_builddir)/libknxusb/libknxusb.la \
	$(PTHREAD_LIBS)

01847_temp_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknxusb/.libs -lm

01847_temp_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(PTHREAD_CFLAGS)



# ------------------------------------------
# Example application to test 01847-temp application
# ------------------------------------------
01847_temp_reader_SOURCES = \
	01847-temp-reader.c



# ------------------------------------------
# Example application to connect to KNX Tiny Serial 810 hardware
# ------------------------------------------
KNXTinySerial_test_SOURCES = \
	KNXTinySerial-test.c



# ------------------------------------------
# Example application to connect to KNX Tiny Serial 810 hardware
# using the library libknx810
# ------------------------------------------
KNXTinySerial_libtest_SOURCES = \
	KNXTinySerial-libtest.c

KNXTinySerial_libtest_LDADD = \
	$(top_builddir)/libknx810/libknx810.la \
	$(PTHREAD_LIBS)

KNXTinySerial_libtest_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknx810/.libs -lm

KNXTinySerial_libtest_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(PTHREAD_CFLAGS)



# Data files to be included in distro but not to be installed.
EXTRA_DIST = \
	tplotd.py


MAINTAINERCLEANFILES = \
	Makefile.in \
	01847-busmonitor \
	01847-busmonitor-nolib \
	01847-sendmsg \
	01847-sendmsg-nolib \
	01847-temp \
	01847-temp-reader \
	KNXTinySerial-test \
	KNXTinySerial-libtest
