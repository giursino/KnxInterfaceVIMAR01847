#! /bin/sh

TARGET=ggrasp
SYSROOT=/mnt/ggrasp


echo -n "*** checking cross gcc..."
if `arm-linux-gnueabihf-gcc -v > /dev/null 2>&1`; then
	echo "ok"
else
	echo "fail"
	echo "Please install or check cross-compiler path"
	echo "HINT: "
	echo " mkdir -p /opt/rpi-sdk"
	echo " cd /opt/rpi-sdk"
	echo " git clone https://github.com/raspberrypi/tools"
	echo ' echo "PATH=$PATH:/opt/rpi-sdk/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin" > ~/.profile'
	echo "FATAL ERROR"
	exit 1
fi


echo -n "*** checking sysroot..."
if [ ! -d ${SYSROOT} ]; then
	echo "created ${SYSROOT}"
	mkdir -p ${SYSROOT}
else
	echo "ok"
fi

echo -n "*** mounting ${SYSROOT}..."
if [ ! -e ${SYSROOT}/usr/bin ]; then
	sudo mount $TARGET:/ $SYSROOT
	echo "ok"
else
	echo "already mounted"
fi

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

export PKG_CONFIG_DIR=
export PKG_CONFIG_LIBDIR=${SYSROOT}/usr/lib/pkgconfig:${SYSROOT}/usr/local/lib/pkgconfig:${SYSROOT}/usr/share/pkgconfig:${SYSROOT}/usr/local/share/pkgconfig:${SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${SYSROOT}/usr/local/lib/arm-linux-gnueabihf/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${SYSROOT}
export PKG_CONFIG_PATH=${PKG_CONFIG_LIBDIR}

echo "*** configuring..."
./configure \
  --host=arm-linux-gnueabihf \
  --with-sysroot=${SYSROOT} \
  CFLAGS="--sysroot=${SYSROOT} ${CFLAGS}" \
  $@

echo ""
echo "done."
exit
