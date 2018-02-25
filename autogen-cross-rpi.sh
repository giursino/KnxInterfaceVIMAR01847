#! /bin/sh

TARGET=ggrasp
SYSROOT=/mnt/ggrasp

echo "checking sysroot..."
if [ ! -e ${SYSROOT}/usr/bin ]; then
	sudo mount $TARGET:/ $SYSROOT
fi

echo "maintainer cleaning..."
make maintainer-clean

echo "other cleaning..."
rm -rf Makefile.in autom4te.cache config.* compile configure depcomp install-sh ltmain missing test-driver aclocal.m4

echo "libtolizing..."
libtoolize --automake

echo "aclocaling..."
aclocal ${OECORE_ACLOCAL_OPTS}

echo "autoconfing..."
autoconf

echo "autoheadering..."
autoheader

echo "automaking..."
automake -a

export PKG_CONFIG_DIR=
export PKG_CONFIG_LIBDIR=${SYSROOT}/usr/lib/pkgconfig:${SYSROOT}/usr/local/lib/pkgconfig:${SYSROOT}/usr/share/pkgconfig:${SYSROOT}/usr/local/share/pkgconfig:${SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${SYSROOT}/usr/local/lib/arm-linux-gnueabihf/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${SYSROOT}
export PKG_CONFIG_PATH=${PKG_CONFIG_LIBDIR}

export CFLAGS="--sysroot=${SYSROOT}"

echo "configuring..."
./configure --host=arm-linux-gnueabihf --with-sysroot=${SYSROOT} $@

echo "done."
exit
