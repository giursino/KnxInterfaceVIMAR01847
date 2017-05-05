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
	res = LKU_SendGroupValueWrite(pDevice, buf, len);
	if (res < 0) {
		perror("LKU_SendGroupValueWrite");
		exit(1);
	}
	printf("Sent msg: ");
	for (i=0; i<len; i++){
		printf("%.2X ", buf[i]);
	}
	printf(".\n");

	res = LKU_Deinit(pDevice);
	if (res < 0) {
		perror("LKU_Deinit");
		exit(1);
	}
	printf("Done.\n");

	return 0;
}

//-END----------------------------- Functions --------------------------------//










