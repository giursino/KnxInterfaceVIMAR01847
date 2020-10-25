/*
 * Example application to send a message with KNX Interface VIMAR 01847
 * It does not use libknxusb, it only use libhid
 *
 * Copyright (c) 2017 Giuseppe Ursino
 */

#include "config.h"


#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <hidapi/hidapi.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define MAX_STR 255

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


	// Send msg to device
	unsigned char sBuf[256];
	int len;

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
	sBuf[i++] = 0x0a;
	sBuf[i++] = 0x01;
	sBuf[i++] = 0x00;
	sBuf[i++] = 0x81;
	len=i;
	printf("sBuf[%i]:", len);
	for (i=0; i<len; i++){
		printf("%.2X ", sBuf[i]);
	}
	printf("\n");

	res = hid_write(handle, sBuf, len);
	if (res < 0) {
		printf("Error: %d\n", errno);
		perror("write");
	} else {
		printf("write() wrote %d bytes\n", res);
	}


	// Finalize the hidapi library
	printf("hid_exit...\n");
	res = hid_exit();

	return 0;
}
