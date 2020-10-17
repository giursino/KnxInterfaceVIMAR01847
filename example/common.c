//******************************************************************************
/// @file 	common.c
/// @brief 	funzioni di comune utilità
/// $Author: Giuseppe Ursino $
/// $Date: 2010-11-09 10:04:16 +0000 (mar, 09 nov 2010) $
/// $Revision: 43 $
///
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software
/// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
///
//******************************************************************************

//-START--------------------------- Definitions ------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include "common.h"


#ifdef LOCAL
	#undef LOCAL
#endif
#define LOCAL static	///< funzioni o variabili invocate solo all'interno del modulo
#ifdef GLOBAL
	#undef GLOBAL
#endif
#define GLOBAL			///< funzioni o variabili rese disponibili ad altri moduli
//-END----------------------------- Definitions ------------------------------//

//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//


//-START----------------------------- Variables ------------------------------//
GLOBAL int				debug_verbosity = DEFAULT_VERBOSITY;	///< valore di verbosità del debug
//-END------------------------------- Variables ------------------------------//


//-START----------------------- Functions Declaration ------------------------//
LOCAL	void		vdbg_message (int priority, const char *fmt, va_list va);
//-END------------------------- Functions Declaration ------------------------//

//-START----------------------------- Functions ------------------------------//


/// @brief	Gestione scrittura messaggi di debug
/// @param	priority priorità informazione da stampare
/// @param	fmt stringa da stampare (come printf)
/// @param	va argomenti successivi alla stringa
LOCAL void	vdbg_message (int priority, const char *fmt, va_list va)
{
	int		ret;
	char	buf[LARGEBUF];

//	ret = vsnprintf(buf, sizeof(buf), fmt, va);	// non è ANSI!
	ret = vsprintf(buf, fmt, va);

	if ((ret < 0) || (ret >= (int) sizeof(buf))){
		fprintf(stderr, "vdbg_message: vsprintf needed more than %d bytes\n", LARGEBUF);
	}

	if (priority <= debug_verbosity) {
		fprintf(stderr, "%s", buf);
	}

}


/// @brief	Scrittura messaggi di debug, simile a printf
/// @note	Usare questa funzione al posto di printf per stampare informazioni
/// di debug.
/// @param	priority priorità informazione da stampare
/// @param	fmt stringa da stampare (come printf)
GLOBAL void	dbg_message (int priority, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vdbg_message(priority, fmt, va);
	va_end(va);
}


/// @brief	Alloca memoria e ritorna puntatore dove è stata allocata
/// @note	Gestiste errore sforamento di memoria
/// @param	size dimensione memoria da allocare (deve esser > 0)
/// @return	puntatore alla memoria allocata, NULL su errore
GLOBAL void*	mmalloc (int size) {
	void* ret;

	if (size < 1) {
		dbg_message(DBG_WARN, "WARNING: void* mmalloc (int size):\n\tsize must be > 0\n");
		return NULL;
	}

	ret = (void*) malloc(size);

	if (ret == NULL) {
		dbg_message(DBG_ERR, "ERROR: Cannot allocate memory\n");
		exit(1);
	}

	return ret;

}


/// @brief	Duplica una stringa in memoria e ritorna puntatore a nuova stringa
GLOBAL char*	mstrdup (const char *s) {
	char *p;

	if (s == NULL) {
		dbg_message(DBG_WARN, "WARNING: char* mstrdup(char *s):\n\ts is NULL pointer\n");
		return NULL;
	}

	p = (char*) ( mmalloc (strlen(s)+1) ); // +1 for '\0'
	strcpy(p, s);
	return p;
}


/// @brief	Libera memoria associato ad un puntatore e pone a NULL il puntatore.
///			Evita errori di doppio uso della funzione free.
GLOBAL void	mfree (void *ptr) {

	free (ptr);
	ptr = NULL;
}


///	@brief	Confronta due stringe, anche con puntatori NULL
/// @param	str1, str2 stringhe da confrontare
///	@return	TRUE se uguali, FALSE se diverse o su errore
GLOBAL bool	mstrcmp (const char *str1, const char *str2) {

	if ((str1==NULL) || (str2==NULL)) {
		dbg_message (DBG_WARN, "WARNING: bool mstrcmp (char *str1, char *str2):\n\tat least one string is NULL pointer\n");
		return FALSE;
	}

	return (bool) (! strcmp (str1, str2) );

}


///	@brief	Controlla che la stringa passata sia un numero esadecimale
/// @param	str stringa da controllare
///	@return	TRUE se stringa è un numero esadecimale, FALSE, altrimenti
GLOBAL bool	isstrhex (const char *str) {
	int i;

	for (i=0; i<strlen(str); i++) {
		if (! isxdigit (str[i]) ) {
			return FALSE;
		}
	}
	return TRUE;
}


///	@brief	Converte stringa di un numero esadecimale in un long
/// @note	Non vengon convertiti numeri negativi.
/// @param	str stringa da convertire
/// @return	numero intero positivo, -1 su errore
GLOBAL long	shextol (const char *str) {

	char	*endptr;
	long	val;

	if (! isstrhex (str) ) {
		// la stringa non è esadecimale
		dbg_message(DBG_WARN, "WARNING: long shextol(char *str):\n\tstr is not hex number\n");
		return -1;
	}

	val = strtol (str, &endptr, 16);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
		(errno != 0 && val == 0) || (endptr == str) ) {
		// errore
		dbg_message(DBG_WARN, "WARNING: long shextol(char *str):\n\tstr cannot convert to hex number\n");
		return -1;
	}

	return val;
}



/// @brief aborts the program and prints message
/// @param msg Message (printf like)
///
GLOBAL void die (const char *msg, ...) {
  va_list ap;
  va_start (ap, msg);
  vprintf (msg, ap);
  va_end (ap);
  printf (": %s\n", strerror (errno));
  exit (1);
}

/// @brief difference between two timeval (end_time - start_time)
/// @param end_time last timeval
/// @param start_time previous timeval
/// @return pointer to timeval difference
///
GLOBAL int64_t		timeval_diff	(struct timeval *difference,
									 struct timeval *end_time,
									 struct timeval *start_time)
{
	struct timeval temp_diff;

	if (end_time == NULL || start_time == NULL) {
		difference = NULL;
		return 0;
	}

	if(difference==NULL)
	{
		difference=&temp_diff;
	}

	difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
	difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

	/* Using while instead of if below makes the code slightly more robust. */

	while(difference->tv_usec<0)
	{
		difference->tv_usec+=1000000;
		difference->tv_sec -=1;
	}

	return		1000000LL *	difference->tv_sec+
							difference->tv_usec;

}



/// @brief	parses a hex number
/// @param	addr string
/// @return parsed hex number
///
GLOBAL unsigned		readHex (const char *addr) {
  int i;
  sscanf (addr, "%x", &i);
  return i;
}


/// @brief	parse hex numbers (byte size) out of a comand line
/// @param	buf output buffer
/// @param	size buffer size
/// @param	ac argument count
/// @param	ag argument array
/// @return parsed bytes
///
GLOBAL int	readBlock (char* buf, int size, int ac, char *ag[]) {
  int i = 0;
  while (size - i > 0 && ac > 0)
    {
      buf[i] = readHex (ag[i]);
      i++;
      ac--;
    }
  return i;
}

//-END----------------------------- Functions --------------------------------//

