///******************************************************************************
/// @file 01847-sendmsg.c
/// @brief Application to send knx message from command line using 01847 interface
///******************************************************************************


//-START--------------------------- Definitions ------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include "config.h"
#include "libknxusb.h"

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL


#define LogPrint(strprefix, ...)		{       \
	if ((strprefix) && (strlen(strprefix))) {   \
		fprintf(stdout, "%s: ", strprefix);     \
	}                                           \
	fprintf(stdout, __VA_ARGS__);               \
	fprintf(stdout, "\n");                      \
}

//-END----------------------------- Definitions ------------------------------//


//-START----------------------- Functions Declaration ------------------------//
LOCAL void LogPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len);
//-END------------------------- Functions Declaration ------------------------//


//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//


//-START----------------------------- Variables ------------------------------//
//-PUBLIC-
//-PRIVATE-
//-END------------------------------ Variables -------------------------------//


//-START------------------------------- ISR ----------------------------------//
//-END--------------------------------- ISR ----------------------------------//


//-START--------------------------- Functions --------------------------------//


/// Print hex message on log
LOCAL void LogPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	fprintf(stdout, "%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		fprintf(stdout, "%.2X ", pMsg[i]);
	}
	fprintf(stdout, ".\n");
}


/// Remove space from string
LOCAL void RemoveSpace (char* str) {
	char *write = str, *read = str;
	do {
	   if (*read != ' ')
	       *write++ = *read;
	} while (*read++);
}


/// Send message
LOCAL int SendStringMsg (hid_device* pDevice, char* strmsg) {
	int len, i, res;
	uint8_t msg[LKU_KNX_MSG_LENGTH];

	RemoveSpace(strmsg);

	if (strmsg == NULL) {
		return -1;
	}
	len = strlen(strmsg);
	if (len <= 0) {
		return -1;
	}
	if (len % 2) {
		return -1;
	}
	if (len > (LKU_KNX_MSG_LENGTH*2)) {
		return -1;
	}

	for (i=0; i<len; i+=2) {
		char strbyte[3];
		char *err;
		strbyte[0]=strmsg[i];
		strbyte[1]=strmsg[i+1];
		strbyte[2]='\0';
		msg[i/2] = (uint8_t) strtoul(strbyte, &err, 16);
		if (*err != '\0') {
			return -1;
		}
	}
	len/=2;

	res = LKU_SendRawMessage(pDevice, msg, len);
	if (res < 0) {
		return -1;
	}
	LogPrintMsg("SendStringMsg", msg, len);

	return len;
}



/// Main function
///
int main(int argc, char* argv[]) {

	printf("Welcome to %s.\n", PACKAGE_STRING);

	hid_device* pDevice;
	int res;

    // parse command line
    if (argc != 2) {
        fprintf(stderr, "ERROR: Invalid arguments.\n");
        fprintf(stdout, "Usage: %s \"BC 110F 21D6 E1 00 80\"\n", argv[0]);
        exit(1);
    }


	// Init Lib Knx Usb
	res = LKU_Init(&pDevice);
	if (res < 0) {
		fprintf(stderr, "ERROR: cannot init library LKU.\n");
		exit(1);
	}
	LogPrint("LKU_Init", "file descriptor: %p.", pDevice);


	// send msg
	if (SendStringMsg(pDevice, argv[1]) < 0) {
		LogPrint("SendStringMsg", "ERROR: message not sent");
	}

	res = LKU_Deinit(pDevice);
	if (res < 0) {
		fprintf(stderr, "ERROR: cannot de-init library LKU.\n");
		exit(1);
	}

	printf("Done.\n");

	return 0;
}

//-END----------------------------- Functions --------------------------------//










