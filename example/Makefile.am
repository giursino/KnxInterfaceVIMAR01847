#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#bin_PROGRAMS=exampleProgram

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=01847-test

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4

# Sources
01847_test_SOURCES= \
	01847-test.c \
	01847-test.h

# Libraries
01847_test_LDADD = $(top_builddir)/libknxusb/libknxusb.la

# Linker options
01847_test_LDFLAGS = -rpath `cd $(top_builddir);pwd`/libknxusb/.libs

# Compiler options
01847_test_CPPFLAGS = -I$(top_srcdir)/include

# Data files to be included in distro but not to be installed.
EXTRA_DIST = 