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
#include <byteswap.h>

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
		uint8_t** emi_data)
{
	if (knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Header.ReportID != 0x01) {
		fprintf(stderr, "KNX USB HID message not recognized: Report ID not valid\n");
		return -1;
	}

	if ((knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Header.PacketInfo.packet_type != 3) ||
		(knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Header.PacketInfo.sequence_number != 1))
	{
		fprintf(stderr, "KNX USB HID message managed: multiple HID reports not managed\n");
		return -1;
	}

	if (knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.ProtocolVersion != 0x00) {
		fprintf(stderr, "KNX HID: Protocol Version not valid\n");
		return -1;
	}

	if (knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.HeaderLength != 0x08) {
		fprintf(stderr, "KNX HID: Header length not valid\n");
		return -1;
	}

	// TODO: manage better the endianness
	uint8_t body_len = bswap_16(knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.BodyLength);

	if (knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.ProtocolID != PROTOCOL_ID_KNX_TUNNEL) {
		fprintf(stderr, "KNX HID: Protocol ID not managed\n");
		return -1;
	}

	*emi_type = (EMI_ID_TYPE) (knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.EMI_ID);

	if (knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.ProtocolID != PROTOCOL_ID_KNX_TUNNEL) {
		fprintf(stderr, "KNX HID: Protocol ID not managed\n");
		return -1;
	}

	if (bswap_16(knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Header.ManufacturerCode) != 0x0000) {
		fprintf(stderr, "KNX HID: Manufacturer Code not managed\n");
		return -1;
	}

	*emi_data = (uint8_t*) (&knx_hid_frame->KNX_HID_Report.KNX_HID_Report_Body.KNX_USB_Transfer_Protocol_Body);

	return body_len;
}
//-END----------------------------- Functions --------------------------------//

