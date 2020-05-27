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
LOCAL int LKU_LData2CEmi(const uint8_t* pMsgLData, uint8_t u8MsgLDataLen,
		uint8_t* pMsgCEmi, uint8_t u8MsgCEmiLen);
LOCAL int LKU_CEmi2LData(const uint8_t* pMsgCEmi, uint8_t u8MsgCEmiLen,
		uint8_t* pMsgLData, uint8_t u8MsgLDataLen);
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


/// Init library
/// @param 	pDevice return pointer to device handler created by library
/// @return	0 on success and -1 on error
///
GLOBAL int LKU_Init(hid_device** pDevice) {
	int res;
	res = hid_init();
	if (res < 0) {
		return -1;
	}
	*pDevice=NULL;
	if (*pDevice==NULL) {
		// work version
		*pDevice = hid_open(0xc251, 0x1101, NULL);  // 01847
	}
	if (*pDevice==NULL) {
		// home version
		*pDevice = hid_open(0x24a0, 0x1101, NULL);  // 01847
	}
	if (*pDevice==NULL) {
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
/// @param  addr	dest group address
/// @param  dpt		datapoint type
/// @param  payload	datapoint payload
/// @param  len		payload length
/// @return	0 on success and -1 on error
///
GLOBAL int LKU_SendGroupValueWrite(hid_device* pDevice, LKU_ADDR_TYPE addr,
		LKU_DPT_TYPE dpt, const uint8_t* payload, uint8_t len) {
	int res;
	uint8_t msg[LKU_KNX_MSG_LENGTH];
	uint8_t msglen;

	msg[LKU_KNX_MSG_CTRLFIELD] = LKU_DEF_CTRLFIELD;
	msg[LKU_KNX_MSG_SRCADDR1] = LKU_DEF_SRCADDR1;
	msg[LKU_KNX_MSG_SRCADDR2] = LKU_DEF_SRCADDR2;
	msg[LKU_KNX_MSG_DSTADDR1] = addr.byte[0];
	msg[LKU_KNX_MSG_DSTADDR2] = addr.byte[1];
	msg[LKU_KNX_MSG_NETFIELD] = LKU_DEF_DAF_GROUP;

	if (dpt==LKU_DPT_6BIT) {
		if (len!=1) return -1;
		msg[LKU_KNX_MSG_LENFIELD] |= 1;
	}
	else if ((dpt>LKU_DPT_6BIT) && (dpt<LKU_DPT_MAX)) {
		if (len!=dpt) return -1;
		msg[LKU_KNX_MSG_LENFIELD] |= (dpt+1);
	}
	else {
		return -1;
	}

	msg[LKU_KNX_MSG_APCIFIELD1] = (LKU_DEF_AGROUPWRITE & 0xFF00) << 8;
	msg[LKU_KNX_MSG_APCIFIELD2] = (LKU_DEF_AGROUPWRITE & 0x00FF);
	if (dpt==LKU_DPT_6BIT) {
		msg[LKU_KNX_MSG_PAYLOAD0] |= (payload[0] & 0x3F);
		msglen=LKU_KNX_MSG_APCIFIELD2+1;
	}
	else {
		memcpy(&msg[LKU_KNX_MSG_PAYLOAD], payload, len);
		msglen=LKU_KNX_MSG_APCIFIELD2+len;
	}

	res = LKU_SendRawMessage(pDevice, msg, msglen);

	return res;
}

/// Send a raw message to knx bus
/// @param 	pDevice	pointer to device handler
/// @param  pMsg	point to msg buffer
/// @return	0 on success and -1 on error
///
GLOBAL int LKU_SendRawMessage(hid_device* pDevice, const uint8_t* pMsg, uint8_t u8MsgLen) {
	uint8_t pMsgCEmi[LKU_CEMI_MSG_LENGTH];
	int res, len;

	len = LKU_LData2CEmi(pMsg, u8MsgLen, pMsgCEmi, LKU_CEMI_MSG_LENGTH);

	res = hid_write(pDevice, pMsgCEmi, len);

	if (res==len) {
		res=0;
	}
	else {
		res=-1;
	}

	DebugPrintMsg("hid_write", pMsgCEmi, len);

	return res;
}


/// Receive a raw message from knx bus. Function blocking.
/// @param 	pDevice	pointer to device handler
/// @param  pMsg	pointer to received msg buffer
/// @param  u8MsgLen received msg buffer len
/// @return	number of byte received and -1 on error
///
GLOBAL int LKU_ReceiveRawMessage(hid_device* pDevice, uint8_t* pMsg, uint8_t u8MsgLen) {
	uint8_t msg[LKU_CEMI_MSG_LENGTH];
	int res;

	res = hid_read(pDevice, msg, LKU_CEMI_MSG_LENGTH);
	if (res < 0) {
		fprintf(stderr, "Error on reading raw message\n");
		return -1;
	}
	res = LKU_CEmi2LData(msg, res, pMsg, u8MsgLen);
	if (res < 0) {
		fprintf(stderr, "Error decoding CEMI message\n");
		return -1;
	}
	return res;
}

/// Convert L_DATA msg to cEmi msg
/// @param 	pMsgLData	 input LData msg
/// @param 	pMsgLDataLen LData msg len
/// @param 	pMsgCEmi	 output CEmi msg
/// @param 	pMsgCEmiLen  CEmi msg len
/// @return	cEmi msg len on success and -1 on error
///
LOCAL int LKU_LData2CEmi(const uint8_t* pMsgLData, uint8_t u8MsgLDataLen,
		uint8_t* pMsgCEmi, uint8_t u8MsgCEmiLen) {

	if (u8MsgCEmiLen < u8MsgLDataLen + 19) {
		fprintf(stderr, "Message destination size lower than source\n");
		return -1;
	}

	int i=0;
	// KNX HID Report Header
	pMsgCEmi[i++] = 0x01; //ReportId
	pMsgCEmi[i++] = 0x13; //PacketInfo
	pMsgCEmi[i++] = u8MsgLDataLen + 3 + 8; //Datalength: number of octets of the data field (KNX HID Report Body)
	// KNX HID Report Body
	// KNX USB Transfer Protocol Header (only in start packet!)
	pMsgCEmi[i++] = 0x00; //ProtocolVersion
	pMsgCEmi[i++] = 0x08; //HeaderLength
	pMsgCEmi[i++] = 0x00;                 //BodyLength: number of octets of the KNX USB Transfer Protocol Body
	pMsgCEmi[i++] = u8MsgLDataLen + 3;    //     "
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
	// copy the rest of data byte
	for (int j=0; j<(u8MsgLDataLen-7); j++) {
		pMsgCEmi[i+j] = pMsgLData[7+1+j];
	}
	return i;
}

/// Convert cEmi to L_DATA msg
/// @param 	pMsgCEmi	 input CEmi msg
/// @param 	pMsgCEmiLen  CEmi msg len
/// @param 	pMsgLData	 output LData msg
/// @param 	pMsgLDataLen LData msg len
/// @return	LData msg len on success and -1 on error
///
LOCAL int LKU_CEmi2LData(const uint8_t* pMsgCEmi, uint8_t u8MsgCEmiLen,
		uint8_t* pMsgLData, uint8_t u8MsgLDataLen) {

	if (u8MsgLDataLen  <  u8MsgCEmiLen - 19) {
		fprintf(stderr, "Message destination size lower than source\n");
		return -1;
	}
	int i=0;
	int mlen;
	int sdata;
	// KNX HID Report Header
	checkCEmiByte(pMsgCEmi[i++], 0x01); //ReportId
	checkCEmiByte(pMsgCEmi[i++], 0x13); //PacketInfo
	i++; //Datalength
	// KNX HID Report Body
	// KNX USB Transfer Protocol Header (only in start packet!)
	checkCEmiByte(pMsgCEmi[i++], 0x00); //ProtocolVersion
	checkCEmiByte(pMsgCEmi[i++], 0x08); //HeaderLength
	mlen = pMsgCEmi[i++]<<8; //BodyLength
	mlen |= pMsgCEmi[i++]; //    "
	mlen -= 3; // calculate payload length
	checkCEmiByte(pMsgCEmi[i++], 0x01); //ProtocolId
	checkCEmiByte(pMsgCEmi[i++], 0x03); //EMIID (cEMI)
	checkCEmiByte(pMsgCEmi[i++], 0x00); //ManufacturerCode
	checkCEmiByte(pMsgCEmi[i++], 0x00); //    "
	// KNX USB Transfer Protocol Body
	//checkCEmiByte(pMsgCEmi[i++], 0x29); //EMIMessageCode (29=rx, 11=tx)
	i++;
	// Data
	checkCEmiByte(pMsgCEmi[i++], 0x00);
	sdata = i;

	// Creating LDATA msg
	pMsgLData[0] = pMsgCEmi[sdata+0]; // Ctrl field
	pMsgLData[1] = pMsgCEmi[sdata+2]; // Src address
	pMsgLData[2] = pMsgCEmi[sdata+3]; //     "
	pMsgLData[3] = pMsgCEmi[sdata+4]; // Dst address
	pMsgLData[4] = pMsgCEmi[sdata+5]; //     "
	pMsgLData[5] = (pMsgCEmi[sdata+6]&0x0F) | (pMsgCEmi[sdata+1]&0xF0); // Len + Network
	for (i=0; i<mlen-6; i++) {
		pMsgLData[6+i] = pMsgCEmi[sdata+7+i];
	}
	return mlen;
}

LOCAL void DebugPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	printf("%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		printf("%.2X ", pMsg[i]);
	}
	printf(".\n");
}
//-END----------------------------- Functions --------------------------------//

