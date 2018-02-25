//******************************************************************************
/// @file libknxusb_priv.h
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

#ifndef LIBKNXUSB_PRIV_H_
#define LIBKNXUSB_PRIV_H_

//-START--------------------------- Definitions ------------------------------//

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static			///< Identifica tutti gli oggetti a carattere locale
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL extern			///< Identifica tutti gli oggetti a carattere globale

/// Max length of cEmi message
#define LKU_CEMI_MSG_LENGTH		64

/// KNX msg field
/// @{
#define LKU_KNX_MSG_CTRLFIELD	0

#define LKU_KNX_MSG_SRCADDR		LKU_KNX_MSG_SRCADDR1
#define LKU_KNX_MSG_SRCADDR1	1
#define LKU_KNX_MSG_SRCADDR2	2

#define LKU_KNX_MSG_DSTADDR		LKU_KNX_MSG_DSTADDR1
#define LKU_KNX_MSG_DSTADDR1	3
#define LKU_KNX_MSG_DSTADDR2	4

#define LKU_KNX_MSG_DAF			5
#define LKU_KNX_MSG_LENFIELD	5
#define LKU_KNX_MSG_NETFIELD	5

#define LKU_KNX_MSG_TPCIFIELD	6
#define LKU_KNX_MSG_APCIFIELD	LKU_KNX_MSG_APCIFIELD1
#define LKU_KNX_MSG_APCIFIELD1	6
#define LKU_KNX_MSG_APCIFIELD2	7

#define LKU_KNX_MSG_PAYLOAD0	7
#define LKU_KNX_MSG_PAYLOAD 	8
/// @}

/// KNX MSG Default Control Field
#define LKU_DEF_CTRLFIELD		0xBC

/// KNX MSG Default Source Address
#define LKU_DEF_SRCADDR1		0x11
#define LKU_DEF_SRCADDR2		0x0F

/// KNX Destination address flag, map on byte 3
#define LKU_DEF_DAF_GROUP		0xE0
#define LKU_DEF_DAF_INDIVIDUAL	0x00

/// APCI
#define LKU_DEF_AGROUPWRITE 	0x0080
//-END----------------------------- Definitions ------------------------------//


//-START------------------------------ Types ---------------------------------//
//-END-------------------------------- Types ---------------------------------//

//-START------------------------------ Macro ---------------------------------//
#define checkCEmiByte(in, exptected) \
	if ((uint8_t) (in) !=  (uint8_t) (exptected)) { \
		perror("CEMI message not recognized"); \
		return -1; \
	}; \
//-END-------------------------------- Macro ---------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//

//-START---------------------------- Variables -------------------------------//
//-END------------------------------ Variables -------------------------------//

//-START----------------------- Functions Declaration ------------------------//
//-END------------------------- Functions Declaration ------------------------//

#endif
