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
CPPFLAGS="-DDEBUG"
CFLAGS="-g -O0"

echo "*** configuring..."
./configure \
  CPPFLAGS="${CPPFLAGS}" \
  CFLAGS="${CFLAGS}" \
  $@
echo ""
echo "done."
exit
