//******************************************************************************
/// @file cemi.c
/// @brief Handler of KNX HID messages
//******************************************************************************

//-START--------------------------- Definitions ------------------------------//
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "config.h"
#include "cemi.h"

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
GLOBAL int CEMI_Decode(
		const CEMI_Frame* emi_data,
		CEMI_MC_TYPE* cEmi_message_code,
		uint8_t* cEmi_data)
{
	*cEmi_message_code = (CEMI_MC_TYPE) (emi_data->cEMI.MessageCode);
	return 0;
}
//-END----------------------------- Functions --------------------------------//

