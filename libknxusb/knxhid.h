//******************************************************************************
/// @file knxhid.h
/// @brief Handler of KNX HID messages
//******************************************************************************


#ifndef KNXHID_H_
#define KNXHID_H_

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
	EMI_ID_EMI1 = 0x01,
	EMI_ID_EMI2 = 0x02,
	EMI_ID_cEMI = 0x03,

	EMI_ID_MAX,
} EMI_ID_TYPE;

typedef enum {
	PROTOCOL_ID_KNX_TUNNEL = 0x01,
	PROTOCOL_ID_M_BUS_TUNNEL = 0x02,
	PROTOCOL_ID_BATI_BUS_TUNNEL = 0x03,
	PROTOCOL_ID_BUS_ACCESS_SERVER_FEATURE_SERVICE = 0x0F,
} PROTOCOL_ID_TYPE;

#pragma pack(1)
typedef union {

	struct {

		struct {
			u_int8_t ReportID;
			struct {
				u_int8_t packet_type:4;
				u_int8_t sequence_number:4;
			} PacketInfo;
			u_int8_t Datalength;
		} KNX_HID_Report_Header;
		union {

			struct {
				struct {
					u_int8_t ProtocolVersion;
					u_int8_t HeaderLength;
					u_int16_t BodyLength;
					u_int8_t ProtocolID;
					u_int8_t EMI_ID;
					u_int16_t ManufacturerCode;
				} KNX_USB_Transfer_Protocol_Header;
				struct {
					u_int8_t EMI_MessageCode;
					u_int8_t EMI_Data[52];
				} KNX_USB_Transfer_Protocol_Body;
			} KNX_HID_Report_Body;

			u_int8_t data[61];
		};
	} KNX_HID_Report;

	u_int8_t byte[64];
} KNXHID_Frame;
#pragma pack()
//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
GLOBAL int KNXHID_Decode(
		const KNXHID_Frame* knx_hid_frame,
		EMI_ID_TYPE* emi_type,
		uint8_t** emi_data);
//-END------------------------- Functions Declaration ------------------------//

#endif
