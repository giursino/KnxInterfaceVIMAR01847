//******************************************************************************
/// @file 01847-test.c
/// @brief Applicativo di test per l'articolo 01847
/// $Author$
/// $Date$
/// $Revision$
///
///
/// @note La storia dello sviluppo del modulo è descritta in @ref revnote01847-test
//******************************************************************************

//
// -----
// $Id$
// -----
//


/**
*@page revnote01847-test Revision History del modulo 01847-test
*@section revnote01847-test_rev0 giursino 23:40:27 23 mar 2017
* - Creazione del modulo.
*/

//-START--------------------------- Definitions ------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <ncurses.h>
#include "config.h"
#include "libknxusb.h"
#include "01847-test.h"

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
LOCAL void DebugPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len);
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

/// Stampa messaggio in hex
LOCAL void DebugPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	printf("%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		printf("%.2X ", pMsg[i]);
	}
	printf(".\n");
}

// Send msg to device
LOCAL void SendMsg1(hid_device* pDevice) {
	int res;
	uint8_t buf[8];
	uint8_t i=0;
	uint8_t len;

	printf("\n");
	printf("Send A_GroupValueWrite to 0x0C0A with value ON.\n");
	printf("Press enter to continue...");
	getc(stdin);

	buf[i++] = 0xbc;
	buf[i++] = 0x10;
	buf[i++] = 0x01;
	buf[i++] = 0x0c;
	buf[i++] = 0x0a;
	buf[i++] = 0xE1;
	buf[i++] = 0x00;
	buf[i++] = 0x81;
	len=i;

	res = LKU_SendRawMessage(pDevice, buf, len);
	if (res < 0) {
		perror("LKU_SendRawMessage");
		exit(1);
	}
	DebugPrintMsg("LKU_SendRawMessage", buf, len);
}

// Send msg to device
LOCAL void SendMsg2 (hid_device* pDevice) {
	int res;
	LKU_ADDR_TYPE ga;
	uint8_t data;

	printf("\n");
	printf("Send A_GroupValueWrite to 0x0C0A with value OFF.\n");
	printf("Press enter to continue...");
	getc(stdin);

	ga.byte[0] = 0x0C;
	ga.byte[1] = 0x0A;
	data = 0;

	res = LKU_SendGroupValueWrite(pDevice, ga, LKU_DPT_6BIT, &data, 1);
	if (res < 0) {
		perror("LKU_SendGroupValueWrite");
		exit(1);
	}
	DebugPrintMsg("LKU_SendGroupValueWrite", &data, 1);
}


/// Funzione principale
///
int main(int argc, char* argv[]) {

	printf("Welcome to %s.\n", PACKAGE_STRING);

	hid_device* pDevice;
	int res;
	int ch;
	WINDOW* wlog;
	WINDOW* wtx;
	WINDOW* wrx;
	WINDOW* borderwin;
	int startx, starty, width, height, cline;
	bool toexit = FALSE;

	// Init curses
	if (initscr() == NULL) {
		perror("Could not initialize screen");
		exit(1);
	}
	// Line buffering disabled on input
	// (when use getch you don't press enter to accept the keypressed)
	raw();
	// We get F1, F2 etc..
	keypad(stdscr, TRUE);
	// Don't echo() while we do getch
	noecho();
	// Use newline on print
	nl();

	printw("Press F2 to exit");
	refresh();

	cline=0;

	// log window
	height = LINES/6;
	width = COLS;
	starty = 0;
	startx = 0;
	borderwin = newwin(height, width, starty, startx);
	box(borderwin, 0 , 0);
	wmove(borderwin, 0, 2);
	wprintw(borderwin, "Log");
	wrefresh(borderwin);
	wlog = newwin(height-2, width-2, starty+1, startx+1);
	scrollok(wlog, TRUE);
	cline+=height;

	// send window
	height = 3;
	width = COLS;
	starty = LINES/6;
	startx = 0;
	borderwin = newwin(height, width, starty, startx);
	box(borderwin, 0 , 0);
	wmove(borderwin, 0, 2);
	wprintw(borderwin, "Send");
	wrefresh(borderwin);
	wtx = newwin(height-2, width-2, starty+1, startx+1);
	cline+=height;

	// receive window
	height = LINES-cline;
	width = COLS;
	starty = cline;
	startx = 0;
	borderwin = newwin(height, width, starty, startx);
	box(borderwin, 0 , 0);
	wmove(borderwin, 0, 2);
	wprintw(borderwin, "Receive");
	wrefresh(borderwin);
	wrx = newwin(height-2, width-2, starty+1, startx+1);
	scrollok(wrx, TRUE);
	cline+=height;

	// refresh
	refresh();
	wrefresh(wlog);
	wrefresh(wtx);
	wrefresh(wrx);

	// Init Lib Knx Usb
	res = LKU_Init(&pDevice);
	if (res < 0) {
		perror("LKU_Init");
		exit(1);
	}
	wprintw(wlog, "File descriptor: %p.\n", pDevice);

	// todo
	wmove(wlog, 1, 1);
	//wsetscrreg(wlog, 1, height-2);
	wrefresh(wlog);

	while(!toexit) {
		ch = getch();
		if ((ch == KEY_F(2)) || (ch == 'q')) {
			toexit = TRUE;
		}
		else if (ch == 's') {
			// send msg
			char msg[22];
			wmove(wtx, 0, 0);
			wprintw(wtx, "Write msg: ");
			echo();
			wgetstr(wtx, msg);
			noecho();
			wclear(wtx);
			wprintw(wlog, "Sent: %s\n", msg);
		}
		else {
			//wmove(wlog,1,1);
			wprintw(wlog, "The pressed key is ");
			attron(A_BOLD);
			wprintw(wlog, "%c\n", ch);
			attroff(A_BOLD);
		}

		// refresh
		refresh();
		wrefresh(wlog);
		wrefresh(wtx);
		wrefresh(wrx);
	}

#if 1
	// End curses mode
	endwin();
	exit(1);
#endif


	SendMsg1(pDevice);

	SendMsg2(pDevice);

	res = LKU_Deinit(pDevice);
	if (res < 0) {
		perror("LKU_Deinit");
		exit(1);
	}

	endwin();			/* End curses mode		  */

	printf("Done.\n");

	return 0;
}

//-END----------------------------- Functions --------------------------------//










