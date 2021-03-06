# KNX Interface VIMAR 01847

This project enables the access on Linux box to the KNX bus using the [USB KNX interface "01847"](https://www.vimar.com/it/it/catalog/product/index/code/01993) by [VIMAR](www.vimar.com).

Now with the "C" library `libknxusb` you can send and receive messages on Linux box with the VIMAR USB KNX interface "01847".

This project includes many code examples to use the library.

## Requirements
```
sudo apt-get install build-essential autotools-dev autoconf automake autoconf-archive gnu-standards autoconf-doc libtool
sudo apt-get install libcmocka0 libcmocka-dev
sudo apt-get install libhidapi-hidraw0 libhidapi-dev libhidapi-hidraw0-dbg udev
sudo apt-get install libncurses5-dev
```

## Build
```
./autogen.sh  (or autogen-cross-rpi.sh to cross compile to raspberry-pi)
make
```

## Install
This install the library on the system and many example programs.
```
make install
```

## Setup the Vimar 01847 device on Linux system
1. Copy udev rules: `sudo cp udev/99-vimar-01847.rules /etc/udev/rules.d`
2. Unplug Vimar 01847 device
3. Restart udev: `sudo /etc/init.d/udev restart`
4. Plug Vimar 01847 device
5. Check if `/dev/vimar-01847` exists when Vimar 01847 is plugged.
6. If interface was not found, restart the system.

## Usage

### KNX bus monitor
Run `01847-busmonitor` program to show the knx bus messages.
This program permits also to send messages on the bus.

Commands:
- `q` key to quit
- `s` key to send message in raw format (hex)

### KNX bus monitor (nolib)
Run `01847-busmonitor-nolib` program to show the knx bus messages.
This program does not use the `libknxusb` library.

### KNX send message
Run `01847-sendmsg "<MSG>"` program to send a KNX raw message from command line.
The `<MSG>` must be format in hex value, example:
* `01847-sendmsg "BC 1001 AABB E1 00 81"`: send a A_GroupValueWrite to group `0xAABB` with value `1`
* `01847-sendmsg "BC 1001 1002 60 03D7 16 FF 10 01 FF"`: send a A_PropertyValueWrite message

## KNX Tiny Interface 810 library
In this project I added the library to use another KNX interface: the "KNX Tiny Interface 810" by Weinzierl Engineering GmbH.

**But it is only experimental.**

Please check my other repo: [knxtinyserial](https://github.com/giursino/knxtinyserial).

# Development

## Debug

To debug, in the root of project, create a file `setup-environment.local` like this:
```
export CPPFLAGS="-DDEBUG -DNO_DAEMON"
export CFLAGS="-g -O0"
```

This file is sourced inside the `autogen*.sh` scripts.

To check if this file works correctly, run:
```
grep CPPFLAGS config.status
grep CFLAGS  config.status
```

## Deploy

To deploy and test on TARGET `raspberry pi`, run script `deploy.sh` that copy all ARM binary inside `tmp` folder of TARGET.
```
HOST> ./deploy.sh
TARGET> cd /tmp
TARGET> LD_LIBRARY_PATH=/tmp 01847-busmonitor
```

## Unit-test

To run the unit-test (CMOCKA required):
```
make check
```

## Release

To release a new version:
* bump version on `AC_INIT` inside `configure.ac`
* run `make distcheck`
* copy `.tar.gz` on GitHub release and create a TAG
