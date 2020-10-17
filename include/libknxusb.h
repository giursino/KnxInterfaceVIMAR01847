//******************************************************************************
/// @file libknxusb.h
/// @brief Header file of libknxusb
//******************************************************************************

#ifndef LIBKNXUSB_H_
#define LIBKNXUSB_H_

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
struct LKU_INSTANCE_;
typedef struct LKU_INSTANCE_ LKU_INSTANCE; /**< opaque LKU structure */

///Driver mode
typedef enum {
	LKU_MODE_NORMAL,
	LKU_MODE_BUSMONITOR,

	LKU_NODE_MAX,
} LKU_MODE;

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

typedef enum {
	LKU_KNXMSG_L_Data,
	LKU_KNXMSG_L_Busmon,
	LKU_KNXMSG_L_Raw,
	LKU_KNXMSG_L_Poll_Data,
} LKU_KNXMSG_TYPE;

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
GLOBAL int LKU_Init(hid_device** pDevice, LKU_MODE mode);
GLOBAL int LKU_Deinit(hid_device* pDevice);
GLOBAL int LKU_SendRawMessage(hid_device* pDevice, const uint8_t* pMsg, uint8_t u8MsgLen);
GLOBAL int LKU_SendGroupValueWrite(hid_device* pDevice, LKU_ADDR_TYPE addr,
		LKU_DPT_TYPE dpt, const uint8_t* payload, uint8_t len);
GLOBAL int LKU_ReceiveRawMessage(hid_device* pDevice, uint8_t* pMsg, uint8_t u8MsgLen);
GLOBAL int LKU_ReceiveLBusmonMessage(hid_device* pDevice, uint8_t* msg, uint8_t msg_len);
GLOBAL int LKU_ReceiveLDataMessage(hid_device* pDevice, uint8_t* msg, uint8_t msg_len);
GLOBAL int LKU_ReceiveMessage(hid_device* pDevice, LKU_KNXMSG_TYPE* msg_type, uint8_t* msg, uint8_t msg_len);
GLOBAL void LKU_ClearBuffer(hid_device* pDevice);

//-END------------------------- Functions Declaration ------------------------//

#endif

