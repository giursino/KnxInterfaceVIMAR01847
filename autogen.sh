#! /bin/sh
[ -e config.cache ] && rm -f config.cache
 
echo "*** libtolizing..."
libtoolize --automake

echo "*** aclocaling..."
aclocal ${OECORE_ACLOCAL_OPTS}

echo "*** autoconfing..."
autoconf

echo "*** autoheadering..."
autoheader

echo "*** automaking..."
automake -a

echo "*** setting DEBUG build..."
export CPPFLAGS="-DDEBUG"
export CFLAGS="-g -O0"

echo "*** configuring..."
./configure $@
echo ""
echo "done."
exit
