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
#include <time.h>
#include <hidapi/hidapi.h>

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
LOCAL int LKU_LData2CEmi(uint8_t* pMsgLData, uint8_t u8MsgLDataLen,
		uint8_t* pMsgCEmi, uint8_t u8MsgCEmiLen);
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


/// Init library
/// @param 	pDevice return pointer to device handler created by library
/// @return	0 on success and -1 on error
///
GLOBAL int LKU_Init(hid_device* pDevice) {
	int res;
	res = hid_init();
	if (res < 0) {
		return -1;
	}
	pDevice=NULL;
	if (pDevice==NULL) {
		// work version
		pDevice = hid_open(0xc251, 0x1101, NULL);  // 01847
	}
	if (pDevice==NULL) {
		// home version
		pDevice = hid_open(0x24a0, 0x1101, NULL);  // 01847
	}
	if (pDevice==NULL) {
		// cannot open the device
		return -1;
	}
	return 0;
}


/// De-init library
/// @param 	pDevice  pointer to device handler
/// @return	0 on success and -1 on error
///
GLOBAL int LKU_Deinit(hid_device* pDevice) {
	int res;
	hid_close(pDevice);
	res = hid_exit();
	if (res < 0) {
		return -1;
	}
	return 0;
}


/// Send A_GroupValueWrite message to knx bus
/// @param 	pDevice	pointer to device handler
/// @param  pMsg	point to msg buffer
/// @return	0 on success and -1 on error
///
GLOBAL int LKU_SendGroupValueWrite(hid_device* pDevice, uint8_t* pMsg, uint8_t u8MsgLen) {
	uint8_t pMsgCEmi[LKU_CEMI_MSG_LENGTH];
	int res, len;

	len = LKU_LData2CEmi(pMsg, u8MsgLen, pMsgCEmi, LKU_CEMI_MSG_LENGTH);

	res = hid_write(pDevice, pMsgCEmi, len);

	return 0;
}


/// Convert L_DATA msg to cEmi msg
LOCAL int LKU_LData2CEmi(uint8_t* pMsgLData, uint8_t u8MsgLDataLen,
		uint8_t* pMsgCEmi, uint8_t u8MsgCEmiLen) {

	if (u8MsgCEmiLen < u8MsgLDataLen + 19) {
		perror("Message destination size lower than source");
		return -1;
	}

	int i=0;
	// KNX HID Report Header
	pMsgCEmi[i++] = 0x01; //ReportId
	pMsgCEmi[i++] = 0x13; //PacketInfo
	pMsgCEmi[i++] = 0x13; //Datalength
	// KNX HID Report Body
	// KNX USB Transfer Protocol Header (only in start packet!)
	pMsgCEmi[i++] = 0x00; //ProtocolVersion
	pMsgCEmi[i++] = 0x08; //HeaderLength
	pMsgCEmi[i++] = 0x00; //BodyLength
	pMsgCEmi[i++] = 0x0b; //    "
	pMsgCEmi[i++] = 0x01; //ProtocolId
	pMsgCEmi[i++] = 0x03; //EMIID (cEMI)
	pMsgCEmi[i++] = 0x00; //ManufacturerCode
	pMsgCEmi[i++] = 0x00; //    "
	// KNX USB Transfer Protocol Body
	pMsgCEmi[i++] = 0x11; //EMIMessageCode (29=rx, 11=tx)
	// Data
	pMsgCEmi[i++] = 0x00;
	pMsgCEmi[i++] = pMsgLData[0];
	pMsgCEmi[i++] = pMsgLData[5]&0xF0;
	pMsgCEmi[i++] = pMsgLData[1];
	pMsgCEmi[i++] = pMsgLData[2];
	pMsgCEmi[i++] = pMsgLData[3];
	pMsgCEmi[i++] = pMsgLData[4];
	pMsgCEmi[i++] = pMsgLData[5]&0x0F;
	pMsgCEmi[i++] = pMsgLData[6];
	pMsgCEmi[i++] = pMsgLData[7];
	return i-1;
}
//-END----------------------------- Functions --------------------------------//

