//******************************************************************************
/// @file 	common.h
/// @brief 	Header file of common.c
/// $Author: Giuseppe Ursino $
/// $Date: 2010-11-09 10:04:16 +0000 (mar, 09 nov 2010) $
/// $Revision: 43 $
//******************************************************************************

#ifndef __COMMON_H__
	#define __COMMON_H__

//-START--------------------------- Definitions ------------------------------//

#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL extern

#define TRUE					1			///< Valore TRUE boolean
#define FALSE					0			///< Valore FALSE boolean

#define LARGEBUF				1024		///< Dimensione massima del buffer di memoria maggiore
#define DEFAULT_VERBOSITY		DBG_ERR		///< Valore di verbosità di default

#define mDIFF_OVF(a,b,max)		((a<b)?(a+(max+1-b)):(a-b))	///< sottrazione su numero interi senza segno di grandezza massima max, si tiene conto dell'overflow
#define mDIFF_U16(a,b)			mDIFF_OVF(a,b,UINT16_MAX) 		///< sottrazione su numeri a 16bit senza segno, si tiene conto dell'overflow

//-END----------------------------- Definitions ------------------------------//

//-START------------------------------ Types ---------------------------------//
typedef		unsigned char		bool;		///< Tipo dati boolean

typedef		struct timeval 		timeval_t;	///< timeval (sec+usec)
typedef		struct tm			tm_t;		///< time structure (year,month,day,...)

typedef		unsigned char		U8;		///< Tipo dati 8bit senza segno
typedef		unsigned short		U16;	///< Tipo dati 16bit senza segno
typedef		unsigned int		U32;	///< Tipo dati 32bit senza segno

/// Enumeration
typedef enum enumDBG {
	DBG_ERR = 0,
	DBG_WARN = 5,
	DBG_INFO = 10,
	DBG_DEBUG = 110,
	
} DBG_TYPE;
//-END-------------------------------- Types ---------------------------------//


//-START----------------------------- Variables ------------------------------//
GLOBAL int				debug_verbosity;
//-END------------------------------- Variables ------------------------------//


//-START----------------------- Functions Declaration ------------------------//

int		main (int argc, char *argv[]);

// funzioni di "libreria"
GLOBAL	void		dbg_message (int priority, const char* fmt, ...);
GLOBAL	void*		mmalloc (int size);
GLOBAL	char*		mstrdup (const char *s);
GLOBAL	void		mfree (void *ptr);
GLOBAL	bool		mstrcmp (const char *str1, const char *str2);
GLOBAL	bool		isstrhex (const char *str);
GLOBAL	long		shextol (const char *str);
GLOBAL	void		die (const char *msg, ...);
GLOBAL	int64_t		timeval_diff (struct timeval *difference, struct timeval *end_time, struct timeval *start_time);

GLOBAL unsigned		readHex (const char *addr);
GLOBAL int			readBlock (char* buf, int size, int ac, char *ag[]);

//-END------------------------- Functions Declaration ------------------------//



#endif

