#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#bin_PROGRAMS=exampleProgram
bin_PROGRAMS=01847-temp

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=01847-test 01847-test-nolib

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4




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
	$(CURSES_LIB)

# Linker options
01847_test_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknxusb/.libs

# Compiler options
01847_test_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(PTHREAD_CFLAGS) \
	$(CURSES_CPPFLAGS) 





# Programma di test che non usa la libreria
01847_test_nolib_SOURCES= \
	01847-test-nolib.c 





# Programma di test della temperatura
01847_temp_SOURCES = \
	01847-temp.c \
	01847-temp.h \
	common.c \
	common.h

01847_temp_LDADD = \
	$(top_builddir)/libknxusb/libknxusb.la \
	$(PTHREAD_LIBS) 

# Linker options
01847_temp_LDFLAGS = \
	-rpath `cd $(top_builddir);pwd`/libknxusb/.libs

# Compiler options
01847_temp_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(PTHREAD_CFLAGS) 







# Data files to be included in distro but not to be installed.
EXTRA_DIST = 

	
MAINTAINERCLEANFILES = \
	Makefile.in \
	01847-test \
	01847-test-nolib \
	01847-temp 
