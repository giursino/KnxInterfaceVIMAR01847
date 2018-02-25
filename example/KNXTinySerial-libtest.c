//******************************************************************************
/// @file KNXTinySerial-libtest.c
/// @brief test della libreria libknx810
/// $Author$
/// $Date$
/// $Revision$
///
///
/// @note La storia dello sviluppo del modulo è descritta in @ref revnoteKNXTinySerial-libtest
//******************************************************************************

//
// -----
// $Id$
// -----
//


/**
*@page revnoteKNXTinySerial-libtest Revision History del modulo KNXTinySerial-libtest
*@section revnoteKNXTinySerial-libtest_rev0 giursino 18:01:09 25 feb 2018
* - Creazione del modulo.
*/

//-START--------------------------- Definitions ------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include "config.h"
#include "libknx810.h"
#include "common.h"

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL

/// Print message on log
#define LogPrint(strprefix, ...)		{       \
	if ((strprefix) && (strlen(strprefix))) {   \
		fprintf(stdout, "%s: ", strprefix);     \
	}                                           \
	fprintf(stdout, __VA_ARGS__);               \
	fprintf(stdout, "\n");                      \
}
//-END----------------------------- Definitions ------------------------------//


//-START----------------------- Functions Declaration ------------------------//
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


/// Print hex message on recevie buffer
LOCAL void PrintReceivedMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	fprintf(stdout, "%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		switch (i) {
			case 1:
			case 3:
			case 5:
			case 6:
			case 7:
			case 8:
				fprintf(stdout, " ");
				break;

			default:
				break;
		}
		fprintf(stdout, "%.2X", pMsg[i]);
	}
	fprintf(stdout, ".\n");
}

/// Main function
///
int main(int argc, char* argv[]) {

	printf("Welcome to %s.\n", PACKAGE_STRING);

	int pDevice;
	int res;
	uint8_t buf[32];

	// Init Lib Knx Usb
	res = LK8_Init(&pDevice);
	if (res < 0) {
		perror("LK8_Init");
		exit(1);
	}
	LogPrint("LK8_Init", "file descriptor: %i.", pDevice);



	while(1) {
		res = LK8_ReceiveRawMessage(pDevice, buf, 32);
		if (res < 0) {
			perror("Error receiving data.");
			exit(1);
		}

		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		char sTime[64];
		strftime(sTime, sizeof(sTime), "%c", tm);
		PrintReceivedMsg(sTime, buf, res);
	}

	exit(0);
}

//-END----------------------------- Functions --------------------------------//

