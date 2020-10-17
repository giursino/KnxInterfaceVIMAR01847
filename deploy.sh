#!/bin/sh
set -e

TARGET="pi@ggrasp"

echo "Copying files to the TARGET..."
scp \
	libknxusb/.libs/libknxusb.so.0.0.0 \
	example/.libs/* \
	example/01847-temp-reader \
	${TARGET}:/tmp
ssh ${TARGET} "cd /tmp; ln -sf libknxusb.so.0.0.0 libknxusb.so"
ssh ${TARGET} "cd /tmp; ln -sf libknxusb.so.0.0.0 libknxusb.so.0"
echo "Done."

echo ""
echo ""

echo "Connect to the TARGET and run:"
echo " cd /tmp"
echo " LD_LIBRARY_PATH=. ./01847-busmonitor"
echo " LD_LIBRARY_PATH=. ./01847-temp"
echo "Done."
