//******************************************************************************
/// @file cemi.c
/// @brief Handler of KNX HID messages
//******************************************************************************

//-START--------------------------- Definitions ------------------------------//
#include <stddef.h>
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
GLOBAL CEMI_MC_TYPE CEMI_GetMessageCode(CEMI_Frame* frame) {
	return (CEMI_MC_TYPE) (frame->cEMI.MessageCode);
}

GLOBAL int CEMI_L_Data_Get(
		const CEMI_Frame* in,
		uint8_t in_len,
		uint8_t* out,
		uint8_t out_len)
{
	CEMI_L_Data* data = (CEMI_L_Data*)(&in->cEMI.Data);

	if (data->L_Data.AdditionalInfoLength != 0x00) {
		fprintf(stderr, "L_Data message not managed\n");
		return -1;
	}

	out[0] = data->L_Data.ControlField1;
	memcpy(&out[1], &data->L_Data.SourceAddress, sizeof(data->L_Data.SourceAddress));
	memcpy(&out[3], &data->L_Data.DestAddress, sizeof(data->L_Data.DestAddress));
	out[5] = (data->L_Data.Length & 0x0F) | (data->L_Data.ControlField2 & 0xF0);
	memcpy(&out[6], data->L_Data.Data, in_len - (offsetof(CEMI_L_Data, L_Data.Data)));

	return 7 + data->L_Data.Length;
}

GLOBAL int CEMI_L_Busmon_Get(
		const CEMI_Frame* in,
		uint8_t in_len,
		uint8_t* out,
		uint8_t out_len)
{
	return -1;
}
//-END----------------------------- Functions --------------------------------//

