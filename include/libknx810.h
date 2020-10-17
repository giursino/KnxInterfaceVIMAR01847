//******************************************************************************
/// @file libknx810.h
/// @brief Header file of libknx810
//******************************************************************************

#ifndef LIBKNX810_H_
#define LIBKNX810_H_

//-START--------------------------- Definitions ------------------------------//

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL extern

//-END----------------------------- Definitions ------------------------------//


//-START------------------------------ Types ---------------------------------//
///KNX DPT size
typedef enum {
	LK8_DPT_6BIT,
	LK8_DPT_1BYTE,
	LK8_DPT_2BYTE,
	LK8_DPT_3BYTE,
	LK8_DPT_4BYTE,
	LK8_DPT_5BYTE,
	LK8_DPT_6BYTE,
	LK8_DPT_7BYTE,
	LK8_DPT_8BYTE,
	LK8_DPT_9BYTE,
	LK8_DPT_10BYTE,
	LK8_DPT_11BYTE,
	LK8_DPT_12BYTE,
	LK8_DPT_13BYTE,
	LK8_DPT_14BYTE,
	LK8_DPT_MAX,
} LK8_DPT_TYPE;

/// KNX address
typedef union {
	uint8_t		byte[2];
	uint16_t	glb;
} LK8_ADDR_TYPE;

/// Max length of KNX message
#define LK8_KNX_MSG_LENGTH		22
//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
GLOBAL int LK8_Init(int* pDevice);
GLOBAL int LK8_Deinit(int pDevice);
GLOBAL int LK8_SendRawMessage(int pDevice, const uint8_t* pMsg, uint8_t u8MsgLen);
GLOBAL int LK8_SendGroupValueWrite(int pDevice, LK8_ADDR_TYPE addr,
		LK8_DPT_TYPE dpt, const uint8_t* payload, uint8_t len);
GLOBAL int LK8_ReceiveRawMessage(int pDevice, uint8_t* pMsg, uint8_t u8MsgLen);

//-END------------------------- Functions Declaration ------------------------//

#endif

