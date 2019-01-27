# KNX Interface VIMAR 01847

This project enables the access on Linux box to the KNX bus using the [USB KNX interface "01847"](https://www.vimar.com/it/it/catalog/product/index/code/01993) by [VIMAR](www.vimar.com). 

Now you can send and receive messages on Linux box with the VIMAR USB KNX interface "01847"!

## Build
```
sudo apt-get install build-essential pkgconf autoconf automake libtool autoconf-archive
sudo apt-get install libcmocka-dev libhidapi-dev libncurses5-dev

./autogen.sh  (or autogen-cross-rpi.sh to cross compile to raspberry-pi)
make
```

## Install
```
make install
```

### Install 01847 device on Linux system
1. Copy udev rules: `sudo cp udev/99-vimar-01847.rules /etc/udev/rules.d`
2. Unplug 01847 device
3. Restart udev: `sudo /etc/init.d/udev restart`
4. Plug 01847 device 
5. Check if `/dev/vimar-01847` exists when 01847 is plugged.
6. If interface was not found, restart the system.

## Usage
```
./example/01847-test
```
Commands:
- `q` key to quit
- `s` key to send message in raw format (hex)

