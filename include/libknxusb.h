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
//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
GLOBAL uint8_t LKU_LData2cEmi(uint8_t* pData, uint8_t* pCEmi, uint8_t u8len);
//-END------------------------- Functions Declaration ------------------------//



