//******************************************************************************
/// @file 01847-test.c
/// @brief Applicativo di test per l'articolo 01847
/// $Author$
/// $Date$
/// $Revision$
///
///
/// @note La storia dello sviluppo del modulo è descritta in @ref revnote01847-test
//******************************************************************************

//
// -----
// $Id$
// -----
//


/**
*@page revnote01847-test Revision History del modulo 01847-test
*@section revnote01847-test_rev0 giursino 23:40:27 23 mar 2017
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
#include <hidapi/hidapi.h>
#include "config.h"
#include "libknxusb.h"
#include "01847-test.h"

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL

//-END----------------------------- Definitions ------------------------------//


//-START----------------------- Functions Declaration ------------------------//
LOCAL void DebugPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len);
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

/// Stampa messaggio in hex
LOCAL void DebugPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	printf("%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		printf("%.2X ", pMsg[i]);
	}
	printf(".\n");
}

/// Funzione principale
///
int main(int argc, char* argv[]) {

	printf("Welcome to %s.\n", PACKAGE_STRING);

	hid_device* pDevice;
	int res;

	res = LKU_Init(&pDevice);
	if (res < 0) {
		perror("LKU_Init");
		exit(1);
	}
	printf("File descriptor: %p.\n", pDevice);


	// Send msg to device
	printf("\n");
	printf("Send A_GroupValueWrite to 0x0C0A with value ON.\n");
	printf("Press enter to continue...");
	getc(stdin);
	uint8_t buf[8];
	uint8_t i=0;
	uint8_t len;
	buf[i++] = 0xbc;
	buf[i++] = 0x10;
	buf[i++] = 0x01;
	buf[i++] = 0x0c;
	buf[i++] = 0x0a;
	buf[i++] = 0xE1;
	buf[i++] = 0x00;
	buf[i++] = 0x81;
	len=i;
	res = LKU_SendRawMessage(pDevice, buf, len);
	if (res < 0) {
		perror("LKU_SendRawMessage");
		exit(1);
	}
	DebugPrintMsg("LKU_SendRawMessage", buf, len);


	// Send msg to device
	printf("\n");
	printf("Send A_GroupValueWrite to 0x0C0A with value OFF.\n");
	printf("Press enter to continue...");
	getc(stdin);
	LKU_ADDR_TYPE ga;
	ga.byte[0] = 0x0C;
	ga.byte[1] = 0x0A;
	int data = 0;
	res = LKU_SendGroupValueWrite(pDevice, ga, LKU_DPT_6BIT, &data, 1);
	if (res < 0) {
		perror("LKU_SendGroupValueWrite");
		exit(1);
	}
	DebugPrintMsg("LKU_SendGroupValueWrite", &data, 1);

	res = LKU_Deinit(pDevice);
	if (res < 0) {
		perror("LKU_Deinit");
		exit(1);
	}
	printf("Done.\n");

	return 0;
}

//-END----------------------------- Functions --------------------------------//










