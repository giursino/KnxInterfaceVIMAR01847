//******************************************************************************
/// @file cemi.h
/// @brief Handler of KNX HID messages
//******************************************************************************


#ifndef CEMI_H_
#define CEMI_H_

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
typedef enum {
	CEMI_MC_L_Busmon_ind = 0x2B,
	CEMI_MC_L_Data_req = 0x11,
	CEMI_MC_L_Data_con = 0x2E,
	CEMI_MC_L_Data_ind = 0x29,
	CEMI_MC_L_Raw_req = 0x10,
	CEMI_MC_L_Raw_ind = 0x2D,
	CEMI_MC_L_Raw_con = 0x2F,
	CEMI_MC_MAX
} CEMI_MC_TYPE;

#pragma pack(1)
typedef union {
	struct {
		uint8_t AdditionalInfoLength;
		uint8_t ControlField1;
		uint8_t ControlField2;
		uint16_t SourceAddress;
		uint16_t DestAddress;
		uint8_t Length;
		uint8_t Data[44];
	} L_Data;

	u_int8_t byte[52];
} CEMI_L_Data;

typedef union {
	struct {
		uint8_t AdditionalInfoLength;
		uint8_t Data[51];
	} L_Busmon;

	u_int8_t byte[52];
} CEMI_L_Busmon;

typedef union {
	struct {
		uint8_t MessageCode;
		uint8_t Data[52];
	} cEMI;

	u_int8_t byte[53];
} CEMI_Frame;
#pragma pack()

//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
GLOBAL int CEMI_Decode(
		const CEMI_Frame* emi_data,
		CEMI_MC_TYPE* cEmi_message_code,
		uint8_t* cEmi_data);
//-END------------------------- Functions Declaration ------------------------//

#endif
