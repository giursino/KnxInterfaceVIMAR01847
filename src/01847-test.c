/*
 * Hidraw Userspace Example
 *
 * Copyright (c) 2010 Alan Ott <alan@signal11.us>
 * Copyright (c) 2010 Signal 11 Software
 *
 * The code may be used by anyone for any purpose,
 * and can serve as a starting point for developing
 * applications using hidraw.
 */

/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 */
#ifndef HIDIOCSFEATURE
#warning Please have your distro update the userspace kernel headers
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


const char *bus_str(int bus);

int main(int argc, char **argv)
{
	int fd;
	int i, res, desc_size = 0;
	char buf[256];
	struct hidraw_report_descriptor rpt_desc;
	struct hidraw_devinfo info;
	char *device = "/dev/hidraw1";

	if (argc > 1)
		device = argv[1];

	/* Open the Device with non-blocking reads. In real life,
	   don't use a hard coded path; use libudev instead. */
	fd = open(device, O_RDWR);

	if (fd < 0) {
		perror("Unable to open device");
		return 1;
	}

	memset(&rpt_desc, 0x0, sizeof(rpt_desc));
	memset(&info, 0x0, sizeof(info));
	memset(buf, 0x0, sizeof(buf));

	/* Get Report Descriptor Size */
	res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
	if (res < 0)
		perror("HIDIOCGRDESCSIZE");
	else
		printf("Report Descriptor Size: %d\n", desc_size);

	/* Get Report Descriptor */
	rpt_desc.size = desc_size;
	res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
	if (res < 0) {
		perror("HIDIOCGRDESC");
	} else {
		printf("Report Descriptor:\n");
		for (i = 0; i < rpt_desc.size; i++)
			printf("%hhx ", rpt_desc.value[i]);
		puts("\n");
	}

	/* Get Raw Name */
	res = ioctl(fd, HIDIOCGRAWNAME(256), buf);
	if (res < 0)
		perror("HIDIOCGRAWNAME");
	else
		printf("Raw Name: %s\n", buf);

	/* Get Physical Location */
	res = ioctl(fd, HIDIOCGRAWPHYS(256), buf);
	if (res < 0)
		perror("HIDIOCGRAWPHYS");
	else
		printf("Raw Phys: %s\n", buf);

	/* Get Raw Info */
	res = ioctl(fd, HIDIOCGRAWINFO, &info);
	if (res < 0) {
		perror("HIDIOCGRAWINFO");
	} else {
		printf("Raw Info:\n");
		printf("\tbustype: %d (%s)\n",
			info.bustype, bus_str(info.bustype));
		printf("\tvendor: 0x%04hx\n", info.vendor);
		printf("\tproduct: 0x%04hx\n", info.product);
	}

//	/* Set Feature */
//	buf[0] = 0x9; /* Report Number */
//	buf[1] = 0xff;
//	buf[2] = 0xff;
//	buf[3] = 0xff;
//	res = ioctl(fd, HIDIOCSFEATURE(4), buf);
//	if (res < 0)
//		perror("HIDIOCSFEATURE");
//	else
//		printf("ioctl HIDIOCGFEATURE returned: %d\n", res);
//
//	/* Get Feature */
//	buf[0] = 0x9; /* Report Number */
//	res = ioctl(fd, HIDIOCGFEATURE(256), buf);
//	if (res < 0) {
//		perror("HIDIOCGFEATURE");
//	} else {
//		printf("ioctl HIDIOCGFEATURE returned: %d\n", res);
//		printf("Report data (not containing the report number):\n\t");
//		for (i = 0; i < res; i++)
//			printf("%hhx ", buf[i]);
//		puts("\n");
//	}

//	/* Send a Report to the Device */
//	buf[0] = 0x1; /* Report Number */
//	buf[1] = 0x77;
//	res = write(fd, buf, 2);
//	if (res < 0) {
//		printf("Error: %d\n", errno);
//		perror("write");
//	} else {
//		printf("write() wrote %d bytes\n", res);
//	}


	int len;

#define TEST_TX

#ifdef TEST_TX
	/* Send a msg to the Device */
	int sBuf[256];
	i=0;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x13;
	sBuf[i++] = 0x13;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x08;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x0b;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x03;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x29;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0xbc;
	sBuf[i++] = 0xe0;
	sBuf[i++] = 0x10;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x0c;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x81;
	len=i;
	printf("sBuf[%i]:", len);
	for (i=0; i<len; i++){
		printf("%.2X ", sBuf[i]);
	}
	printf("\n");

	res = write(fd, sBuf, len);
	if (res < 0) {
		printf("Error: %d\n", errno);
		perror("write");
	} else {
		printf("write() wrote %d bytes\n", res);
	}

	printf("Premere invio per continuare...");
	scanf(sBuf);

	i=0;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x13;
	sBuf[i++] = 0x13;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x08;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x0b;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x03;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x29;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0xbc;
	sBuf[i++] = 0xe0;
	sBuf[i++] = 0x10;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x0c;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x80;
	len=i;
	printf("sBuf[%i]:", len);
	for (i=0; i<len; i++){
		printf("%.2X ", sBuf[i]);
	}
	printf("\n");

	res = write(fd, sBuf, len);
	if (res < 0) {
		printf("Error: %d\n", errno);
		perror("write");
	} else {
		printf("write() wrote %d bytes\n", res);
	}

	printf("Premere invio per continuare...");
	scanf(sBuf);
	printf("\n");
#endif

    time_t timer;
    char timebuffer[26];
    struct tm* tm_info;


	/* Get a report from the device */
	while (1) {
	res = read(fd, buf, 64);
	if (res < 0) {
		perror("read");
	} else {
		time(&timer);
		tm_info = localtime(&timer);
		strftime(timebuffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
		//printf("read() read %d bytes:\n\t", res);
		printf("%s \t", timebuffer);
		len=buf[2]+3;
		for (i = 0; i < len; i++) {
			switch (i) {
				case 14:
					// control field
					printf("\t");
					printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
					break;
				case 15:
				case 16:
					// src addr
					printf(ANSI_COLOR_GREEN "%.2hhx " ANSI_COLOR_RESET, buf[i]);
					break;
				case 17:
				case 18:
					// dst addr
					printf(ANSI_COLOR_RED "%.2hhx " ANSI_COLOR_RESET, buf[i]);
					break;
				case 19:
				{
					// len
					printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
					break;
				}
				default:
					printf("%.2hhx ", buf[i]);
					break;
			}
		}
		puts("");
	}
	}
	close(fd);
	return 0;
}

const char *
bus_str(int bus)
{
	switch (bus) {
	case BUS_USB:
		return "USB";
		break;
	case BUS_HIL:
		return "HIL";
		break;
	case BUS_BLUETOOTH:
		return "Bluetooth";
		break;
	case BUS_VIRTUAL:
		return "Virtual";
		break;
	default:
		return "Other";
		break;
	}
}
