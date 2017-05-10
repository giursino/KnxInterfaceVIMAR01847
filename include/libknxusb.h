//******************************************************************************
/// @file libknxusb.h
/// @brief Header file del modulo libknxusb
/// $Author$
/// $Date$
/// $Revision$
///
///
///
//******************************************************************************

//
// -----
// $Id$
// -----
//


//-START--------------------------- Definitions ------------------------------//

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static			///< Identifica tutti gli oggetti a carattere locale
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL extern			///< Identifica tutti gli oggetti a carattere globale

//-END----------------------------- Definitions ------------------------------//


//-START------------------------------ Types ---------------------------------//
///KNX DPT size
typedef enum {
	LKU_DPT_6BIT,
	LKU_DPT_1BYTE,
	LKU_DPT_2BYTE,
	LKU_DPT_3BYTE,
	LKU_DPT_4BYTE,
	LKU_DPT_5BYTE,
	LKU_DPT_6BYTE,
	LKU_DPT_7BYTE,
	LKU_DPT_8BYTE,
	LKU_DPT_9BYTE,
	LKU_DPT_10BYTE,
	LKU_DPT_11BYTE,
	LKU_DPT_12BYTE,
	LKU_DPT_13BYTE,
	LKU_DPT_14BYTE,
	LKU_DPT_MAX,
} LKU_DPT_TYPE;

/// KNX address
typedef union {
	uint8_t		byte[2];
	uint16_t	glb;
} LKU_ADDR_TYPE;

/// Max length of KNX message
#define LKU_KNX_MSG_LENGTH		22
//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
GLOBAL int LKU_Init(hid_device** pDevice);
GLOBAL int LKU_Deinit(hid_device* pDevice);
GLOBAL int LKU_SendRawMessage(hid_device* pDevice, uint8_t* pMsg, uint8_t u8MsgLen);
GLOBAL int LKU_SendGroupValueWrite(hid_device* pDevice, LKU_ADDR_TYPE addr,
		LKU_DPT_TYPE dpt, uint8_t* payload, uint8_t len);
//-END------------------------- Functions Declaration ------------------------//



