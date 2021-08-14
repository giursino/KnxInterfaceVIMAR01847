/*
 * Example application to test the KNX Interface VIMAR 01847
 * It do not use libknxusb
 * It only uses libhid
 *
 * Copyright (c) 2017 Giuseppe Ursino
 */

#include "config.h"

#if USE_HIDRAW
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

	printf("Get supported EMI Type\n");
	printf("Press enter to continue...");
	getc(stdin);
	// --> 01 13 09 00 08 00 01 0F 01 00 00 01
	// <-- 01 13 0B 00 08 00 03 0F 02 00 00 01 xx xx
	i=0;
	// KNX HID Report Header
	sBuf[i++] = 0x01; //ReportId
	sBuf[i++] = 0x13; //PacketInfo
	sBuf[i++] = 0x09; //Datalength
	// KNX HID Report Body
	// KNX USB Transfer Protocol Header (only in start packet!)
	sBuf[i++] = 0x00; //ProtocolVersion
	sBuf[i++] = 0x08; //HeaderLength
	sBuf[i++] = 0x00; //BodyLength
	sBuf[i++] = 0x01; //    "
	sBuf[i++] = 0x0F; //ProtocolId
	sBuf[i++] = 0x01; //EMIID (cEMI)
	sBuf[i++] = 0x00; //ManufacturerCode
	sBuf[i++] = 0x00; //    "
	// KNX USB Transfer Protocol Body
	sBuf[i++] = 0x01; //EMIMessageCode (29=rx, 11=tx)
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
	res = read(fd, buf, 64);
	if (res < 0) {
		perror("read");
	} else {
		len=buf[2]+3;
		for (i = 0; i < len; i++) {
			printf("%.2hhx ", buf[i]);
		}
		puts("");
	}


	printf("Send A_GroupValueWrite to 0x0C00 with value ON.\n");
	printf("Press enter to continue...");
	getc(stdin);
	i=0;
	// KNX HID Report Header
	sBuf[i++] = 0x01; //ReportId
	sBuf[i++] = 0x13; //PacketInfo
	sBuf[i++] = 0x13; //Datalength
	// KNX HID Report Body
	// KNX USB Transfer Protocol Header (only in start packet!)
	sBuf[i++] = 0x00; //ProtocolVersion
	sBuf[i++] = 0x08; //HeaderLength
	sBuf[i++] = 0x00; //BodyLength
	sBuf[i++] = 0x0b; //    "
	sBuf[i++] = 0x01; //ProtocolId
	sBuf[i++] = 0x03; //EMIID (cEMI)
	sBuf[i++] = 0x00; //ManufacturerCode
	sBuf[i++] = 0x00; //    "
	// KNX USB Transfer Protocol Body
	sBuf[i++] = 0x11; //EMIMessageCode (29=rx, 11=tx)
	// Data
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


	printf("Send A_GroupValueWrite to 0x0C00 with value OFF.\n");
	printf("Press enter to continue...");
	getc(stdin);
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
	sBuf[i++] = 0x11;
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

	printf("Busmonitor.\n");
	printf("Press enter to continue...");
	getc(stdin);
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
				case 13:
					// control field
					printf("\t");
				case 14:
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
#endif


#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <hidapi/hidapi.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define CEMI_M_CODE_POS 11
#define CEMI_PROP_DATA_POS 18

#define CEMI_M_PropRead_req  0xFC
#define CEMI_M_PropRead_con  0xFB

#define CEMI_M_PropWrite_req 0xF6
#define CEMI_M_PropWrite_con 0xF5


#define MAX_STR 255

static bool toexit = false;

/// Signal handler, to exit from main loop
static void SignalHandler(int signo) {
	switch  (signo) {
		case SIGINT:
		case SIGTERM:
			printf("Catched signal %i.\n", signo);
			toexit=true;
			break;
		default:
			break;
	}

}

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	printf("Welcome to %s.\n", PACKAGE_STRING);

	printf("hid_init...\n");

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	printf("hid_open...");
	handle=NULL;
	if (handle==NULL) {
		handle = hid_open(0xc251, 0x1101, NULL);  // 01847
	}
	if (handle==NULL) {
		handle = hid_open(0x24a0, 0x1101, NULL);  // 01847
	}
	if (handle==NULL) {
		perror("Cannot open the device");
		exit(1);
	}
	printf("[handle=%p]\n", handle);


	// Read the Manufacturer String
	printf("hid_get_manufacturer_string...");
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	printf("%i\n", res);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	printf("hid_get_product_string...");
	res = hid_get_product_string(handle, wstr, MAX_STR);
	printf("%i\n", res);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	printf("hid_get_serial_number_string...");
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	printf("%i\n", res);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	printf("hid_get_indexed_string...");
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	printf("%i\n", res);
	wprintf(L"Indexed String 1: %s\n", wstr);

	// Flush buffer
	printf("Flush buffer...\n");
	do {
		res = hid_read_timeout(handle, buf, 65, 1000);
		printf(" flush %i bytes\n", res);
	} while (res > 0);

	// Enable bus-monitor mode
	printf("Enable bus-monitor mode...");
	const uint8_t msg_busmonitor_enable[] = {
		0x01, 0x13, 0x10, 0x00, 0x08, 0x00, 0x08, 0x01, 0x03, 0x00, 0x00,
		CEMI_M_PropWrite_req,
		0x00, 0x08, 0x01, 0x34, 0x10, 0x01, 0x01
	};
	res = hid_write(handle, msg_busmonitor_enable, sizeof(msg_busmonitor_enable));
	printf("%i\n", res);
	printf("Waiting response...");
	res = hid_read_timeout(handle, buf, 65, 1000);
	printf("%i\n", res);
	for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
	if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropWrite_con)) {
		printf("property written\n");
	}

	printf("Check bus-monitor mode...");
	const uint8_t msg_busmonitor_get[] = {
		0x01, 0x13, 0x0F, 0x00, 0x08, 0x00, 0x07, 0x01, 0x03, 0x00, 0x00,
		CEMI_M_PropRead_req,
		0x00, 0x08, 0x01, 0x34, 0x10, 0x01
	};
	res = hid_write(handle, msg_busmonitor_get, sizeof(msg_busmonitor_get));
	printf("%i\n", res);
	printf("Waiting response...");
	res = hid_read_timeout(handle, buf, 65, 1000);
	printf("%i\n", res);
	for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
	if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropRead_con) && (buf[CEMI_PROP_DATA_POS]==0x01)) {
		printf("bus-monitor enabled!\n");
	}


	// Register signal handler
	if (signal(SIGINT, SignalHandler) == SIG_ERR) {
		perror("Cannot catch SIGINT");
		exit(1);
	}
	if (signal(SIGTERM, SignalHandler) == SIG_ERR) {
		perror("Cannot catch SIGTERM");
		exit(1);
	}

	int len;

	// Read requested state
	printf("hid_read...\n");
	while(!toexit) {
		res = hid_read_timeout(handle, buf, 65, 1000);

		// Print out the returned buffer.
		if (res < 0) {
			perror("read");
		}
		else if (res == 0) {
			continue;
		}
		else {
			time_t timer;
			char timebuffer[26];
			struct tm* tm_info;

			time(&timer);
			tm_info = localtime(&timer);
			strftime(timebuffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
			printf("%s \t", timebuffer);
			int len=buf[2]+3;
      const int start = 20;
			for (i = start; i < len; i++) {
				if (i == start+0) {
            // ctrl
						printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
				}
        else if ((i == start+1) || (i == start+2)) {
						// src addr
						printf(ANSI_COLOR_GREEN "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if ((i == start+3) || (i == start+4)) {
						// dst addr
						printf(ANSI_COLOR_RED "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if (i == start+5) {
						// len
						printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if ((i > start+7) && (i < len-1)) {
            // data
						printf(ANSI_COLOR_MAGENTA "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if (i == (len-1)) {
            // crc
						printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else {
						printf("%.2hhx ", buf[i]);
				}
			}
			puts("");
		}
	}

	// Disable bus-monitor mode
	printf("Disable bus-monitor mode...");
	const uint8_t msg_busmonitor_disable[] = {
		0x01, 0x13, 0x10, 0x00, 0x08, 0x00, 0x08, 0x01, 0x03, 0x00, 0x00,
		CEMI_M_PropWrite_req,
		0x00, 0x08, 0x01, 0x34, 0x10, 0x01, 0x00
	};
	res = hid_write(handle, msg_busmonitor_disable, sizeof(msg_busmonitor_disable));
	printf("%i\n", res);
	printf("Waiting response...");
	res = hid_read_timeout(handle, buf, 65, 1000);
	printf("%i\n", res);
	for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
	if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropWrite_con)) {
		printf("property written\n");
	}

	printf("Check bus-monitor mode...");
	res = hid_write(handle, msg_busmonitor_get, sizeof(msg_busmonitor_get));
	printf("%i\n", res);
	printf("Waiting response...");
	res = hid_read_timeout(handle, buf, 65, 1000);
	printf("%i\n", res);
	for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
	if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropRead_con) && (buf[CEMI_PROP_DATA_POS]==0x00)) {
		printf("bus-monitor disabled!\n");
	}

	// Finalize the hidapi library
	printf("hid_exit...\n");
	res = hid_exit();

	return 0;
}
