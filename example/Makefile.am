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
	KNXTinySerial-t01847-sendmsg-nolibest \
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
# Programma di esempio per leggere e scrivere messagi su bus,
# usa grafica ncurses e la libreria per accedere all'interfaccia 01847
# ------------------------------------------
# Sources
01847_test_SOURCES= \
	01847-test.c \
	01847-test.h \
	common.c \
	common.h

# Libraries
01847_test_LDADD = \
	$(top_builddir)/libknxusb/libknxusb.la \
	$(PTHREAD_LIBS) \
	$(CURSES_LIBS)

# Linker options
01847_test_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknxusb/.libs

# Compiler options
01847_test_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(PTHREAD_CFLAGS) \
	$(CURSES_CPPFLAGS)



# ------------------------------------------
# Programma di esempio per inviare messaggi da riga di comando
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
# Programma di esempio per leggere i messagi su bus, non usa né grafica né la libreria
# ------------------------------------------
01847_busmonitor_nolib_SOURCES= \
	01847-busmonitor-nolib.c



# ------------------------------------------
# Programma di esempio per inviare messaggi da riga di comando che non usa la libreria
# ------------------------------------------
01847_sendmsg_nolib_SOURCES= \
	01847-sendmsg-nolib.c



# ------------------------------------------
# Programma di esempio per il monitoraggio della temperatura
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
# Test del programma di esempio per il monitoraggio della temperatura
# ------------------------------------------
01847_temp_reader_SOURCES = \
	01847-temp-reader.c



# ------------------------------------------
# Test dell'hw KNX Tiny Serial 810
# ------------------------------------------
KNXTinySerial_test_SOURCES = \
	KNXTinySerial-test.c



# ------------------------------------------
# Test della libreria libknx810
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
