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
typedef enum {
	LKU_DPT_1xxx,
	LKU_DPT_2xxx,
	LKU_DPT_3xxx,
	LKU_DPT_4xxx,
	LKU_DPT_5xxx,
	LKU_DPT_6xxx,
	LKU_DPT_7xxx,
	LKU_DPT_8xxx,
	LKU_DPT_9xxx,
	LKU_DPT_10xxx,
} LKU_DPT_TYPE;
//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
GLOBAL int LKU_Init(hid_device* pDevice);
GLOBAL int LKU_Deinit(hid_device* pDevice);
GLOBAL int LKU_SendGroupValueWrite(hid_device* pDevice, uint8_t* pMsg, uint8_t u8MsgLen);
//-END------------------------- Functions Declaration ------------------------//



