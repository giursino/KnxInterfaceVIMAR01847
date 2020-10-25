//******************************************************************************
/// @file knxutils.c
/// @brief (*[TODO]: inserire descrizione breve del modulo knxutils *)
//  giursino (c) 23:20:58 25 ott 2020
//******************************************************************************

//-START--------------------------- Definitions ------------------------------//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "knxutils.h"

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
GLOBAL float DptValueTemp2Float (uint8_t dpt[2]) {
	uint16_t t_raw = (dpt[0] << 8) + dpt[1];
	int16_t m = t_raw & 0x7FF;
	if ((t_raw & 0x8000) == 0x8000) {
		m = -m;
	}
	uint8_t e = (uint8_t) ((t_raw & 0x7800) >> 11);
	float t = (0.01*m) * powf(2,e);

	#ifdef DEBUG
		fprintf(stdout, "* t_raw=0x%.4X\n", t_raw);
		fprintf(stdout, "* m=%i (0x%.4X)\n", m, (uint16_t) m);
		fprintf(stdout, "* e=%i (0x%.4X)\n", e, e);
		fprintf(stdout, "* t=%f\n", t);
		fprintf(stdout, "* 2^e=%f\n", powf(2,e));
		fprintf(stdout, "* 0.01*m=%f\n", 0.01*m);
	#endif

	return t;
}
//-END----------------------------- Functions --------------------------------//
