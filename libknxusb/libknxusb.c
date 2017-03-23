//******************************************************************************
/// @file libknxusb.c
/// @brief Libreria per interfacciarsi al bus KNX tramite USB
/// $Author$
/// $Date$
/// $Revision$
///
///
/// @note La storia dello sviluppo del modulo è descritta in @ref revnotelibknxusb
//******************************************************************************

//
// -----
// $Id$
// -----
//


/**
*@page revnotelibknxusb Revision History del modulo libknxusb
*@section revnotelibknxusb_rev0 giursino 23:40:27 23 mar 2017
* - Creazione del modulo.
*/

//-START--------------------------- Definitions ------------------------------//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "libknxusb.h"
#include "libknxusb_priv.h"

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

/// Conversione dati da knx a cEmi
///
GLOBAL uint8_t LKU_LData2cEmi(uint8_t* pData, uint8_t* pCEmi, uint8_t u8len) {
	memcpy(pCEmi, pData, u8len);
	printf("...%s...", PIPPO);
	return 0;
}

//-END----------------------------- Functions --------------------------------//

