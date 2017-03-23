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

	uint8_t buf[3]= {1,2,3};
	uint8_t dest[3]= {0,0,0};

	int res= LKU_LData2cEmi(buf, dest, 3);

	printf("res=%d...dest=%d %d %d\n", res, dest[0], dest[1], dest[2]);


	return 0;
}

//-END----------------------------- Functions --------------------------------//










