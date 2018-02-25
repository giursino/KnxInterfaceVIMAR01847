#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libknx810.la


#######################################
# Build information for each library

# Sources and headers nedded by library
# All file are included in the distro
libknx810_la_SOURCES = \
	libknx810.c \
	libknx810_priv.h 
 

# Linker options
libknx810_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libknx810_la_CPPFLAGS = -I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

