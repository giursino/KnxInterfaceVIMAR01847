#! /bin/sh
echo "*** maintainer cleaning..."
make maintainer-clean

echo -n "*** other cleaning..."
rm -rf Makefile.in autom4te.cache config.* compile configure depcomp install-sh ltmain missing test-driver aclocal.m4
echo "ok"

echo "*** setting environment for debugging..."
export CPPFLAGS="-DDEBUG"
export CFLAGS="-g -O0"

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

echo "*** configuring..."
./configure $@

echo ""
echo "done."
exit
