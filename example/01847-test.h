//******************************************************************************
/// @file 01847-test.h
/// @brief Header file del modulo 01847-test 
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

#ifndef __01847_TEST_H_
#define __01847_TEST_H_

//-START--------------------------- Definitions ------------------------------//

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static			///< Identifica tutti gli oggetti a carattere locale
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL extern			///< Identifica tutti gli oggetti a carattere globale

/// Print message on log
#define LogPrint(strprefix, ...)		{       \
	if ((strprefix) && (strlen(strprefix))) {   \
		wprintw(wlog, "%s: ", strprefix);       \
	}                                           \
	wprintw(wlog, __VA_ARGS__);                 \
	wprintw(wlog, "\n");                        \
	wrefresh(wlog);                             \
}

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
