//******************************************************************************
/// @file libknx810_priv.h
/// @brief Header file del modulo libknx810
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
#ifndef LIBKNX810_PRIV_H_
#define LIBKNX810_PRIV_H_

//-START--------------------------- Definitions ------------------------------//

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static			///< Identifica tutti gli oggetti a carattere locale
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL extern			///< Identifica tutti gli oggetti a carattere globale

/// Max length of message
#define LK8_MSG_LENGTH		32

/// KNX msg field
/// @{
#define LK8_KNX_MSG_CTRLFIELD	0

#define LK8_KNX_MSG_SRCADDR		LK8_KNX_MSG_SRCADDR1
#define LK8_KNX_MSG_SRCADDR1	1
#define LK8_KNX_MSG_SRCADDR2	2

#define LK8_KNX_MSG_DSTADDR		LK8_KNX_MSG_DSTADDR1
#define LK8_KNX_MSG_DSTADDR1	3
#define LK8_KNX_MSG_DSTADDR2	4

#define LK8_KNX_MSG_DAF			5
#define LK8_KNX_MSG_LENFIELD	5
#define LK8_KNX_MSG_NETFIELD	5

#define LK8_KNX_MSG_TPCIFIELD	6
#define LK8_KNX_MSG_APCIFIELD	LK8_KNX_MSG_APCIFIELD1
#define LK8_KNX_MSG_APCIFIELD1	6
#define LK8_KNX_MSG_APCIFIELD2	7

#define LK8_KNX_MSG_PAYLOAD0	7
#define LK8_KNX_MSG_PAYLOAD 	8
/// @}

/// KNX MSG Default Control Field
#define LK8_DEF_CTRLFIELD		0xBC

/// KNX MSG Default Source Address
#define LK8_DEF_SRCADDR1		0x11
#define LK8_DEF_SRCADDR2		0x0F

/// KNX Destination address flag, map on byte 3
#define LK8_DEF_DAF_GROUP		0xE0
#define LK8_DEF_DAF_INDIVIDUAL	0x00

/// APCI
#define LK8_DEF_AGROUPWRITE 	0x0080
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
//-END------------------------- Functions Declaration ------------------------//
#endif

