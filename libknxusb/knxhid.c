//******************************************************************************
/// @file knxhid.c
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
#include "knxhid.h"

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
GLOBAL int KNXHID_Decode(
		const KNXHID_Frame* knx_hid_frame,
		EMI_ID_TYPE* emi_type,
		uint8_t* emi_data)
{
	*emi_type = knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.EMI_ID;
	emi_data = &knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Body;
	return knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.BodyLength;
}
//-END----------------------------- Functions --------------------------------//

