#!/bin/sh
set -e

TARGET="pi@ggrasp"


echo "Checking binary files..."
BIN_EXAMPLE=$(find example -type f -not -name '*.*' -exec sh -c "file {} | grep 'ARM' > /dev/null" \; -print)
if [ -z "${BIN_EXAMPLE}" ]; then
  echo "ERROR: missing ARM binary files. Please cross-compile!"
  exit 1
fi

echo "Copying files to the TARGET..."
scp \
	libknxusb/.libs/libknxusb.so.0.0.0 \
	${BIN_EXAMPLE} \
	${TARGET}:/tmp
ssh ${TARGET} "cd /tmp; ln -sf libknxusb.so.0.0.0 libknxusb.so"
ssh ${TARGET} "cd /tmp; ln -sf libknxusb.so.0.0.0 libknxusb.so.0"
echo "Done."

echo ""
echo ""

echo "Connect to the TARGET and run:"
echo " cd /tmp"
echo " LD_LIBRARY_PATH=. ./01847-busmonitor"
echo "Done."
