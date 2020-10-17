#! /bin/sh
[ -e config.cache ] && rm -f config.cache
 
libtoolize --automake
aclocal ${OECORE_ACLOCAL_OPTS}
autoconf
autoheader
automake -a
./configure --prefix=/dbg CPPFLAGS=-DDEBUG CFLAGS="-g -O0" $@
exit
