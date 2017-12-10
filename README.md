# 01847-test

Sniffer of 01847 device

## Install

sudo apt-get install libcmocka-dev libhidapi-dev libncurses5-dev

./configure
make
make install

cp udev/99-vimar-01847.rules /etc/udev/rules.d

unplug 01847 device

sudo /etc/init.d/udev restart

plug 01847 device and check if /dev/vimar-01847 exists when 01847 is plugged


## Build

sudo apt-get install build-essential pkgconf autoconf automake libtool autoconf-archive


## Usage

./example/01847-test

Commands:
- q: quit
- s: send message in raw format (hex)

