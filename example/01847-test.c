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
LOCAL void LogPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len);
//-END------------------------- Functions Declaration ------------------------//


//-START------------------------------ Const ---------------------------------//
//-END-------------------------------- Const ---------------------------------//


//-START----------------------------- Variables ------------------------------//
//-PUBLIC-
GLOBAL WINDOW* wlog;
GLOBAL WINDOW* wtx;
GLOBAL WINDOW* wrx;
//-PRIVATE-
//-END------------------------------ Variables -------------------------------//


//-START------------------------------- ISR ----------------------------------//
//-END--------------------------------- ISR ----------------------------------//


//-START--------------------------- Functions --------------------------------//

/// Print hex message on log
LOCAL void LogPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	wprintw(wlog, "%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		wprintw(wlog, "%.2X ", pMsg[i]);
	}
	wprintw(wlog, ".\n");
	wrefresh(wlog);
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
	LogPrintMsg("LKU_SendRawMessage", buf, len);
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
	LogPrintMsg("LKU_SendGroupValueWrite", &data, 1);
}

// Remove space from string
LOCAL void RemoveSpace (char* str) {
	char *write = str, *read = str;
	do {
	   if (*read != ' ')
	       *write++ = *read;
	} while (*read++);
}

// Send message
LOCAL int SendStringMsg (hid_device* pDevice, char* strmsg) {
	int len, i, res;
	uint8_t msg[LKU_KNX_MSG_LENGTH];

	RemoveSpace(strmsg);

	if (strmsg == NULL) {
		return -1;
	}
	len = strlen(strmsg);
	if (len <= 0) {
		return -1;
	}
	if (len % 2) {
		return -1;
	}
	if (len > (LKU_KNX_MSG_LENGTH*2)) {
		return -1;
	}

	for (i=0; i<len; i+=2) {
		char strbyte[3];
		char *err;
		strbyte[0]=strmsg[i];
		strbyte[1]=strmsg[i+1];
		strbyte[2]='\0';
		msg[i/2] = (uint8_t) strtoul(strbyte, &err, 16);
		if (*err != '\0') {
			return -1;
		}
	}
	len/=2;

	res = LKU_SendRawMessage(pDevice, msg, len);
	if (res < 0) {
		return -1;
	}
	LogPrintMsg("SendStringMsg", msg, len);

	return len;
}

// Create and draw window with border on screen
LOCAL WINDOW* CreateWinWithBorder(char* name, bool scroll,
		int startx, int starty,	int width, int height) {

	WINDOW* borderwin;
	WINDOW* containerwin;
	borderwin = newwin(height, width, starty, startx);
	box(borderwin, 0 , 0);
	wmove(borderwin, 0, 2);
	wprintw(borderwin, name);
	wrefresh(borderwin);
	containerwin = newwin(height-2, width-2, starty+1, startx+1);
	scrollok(containerwin, scroll);
	wrefresh(containerwin);

	return containerwin;
}

/// Refresh the screen optimized
LOCAL void RefreshAll() {
	wnoutrefresh(wlog);
	wnoutrefresh(wtx);
	wnoutrefresh(wrx);
	doupdate();
}

/// Main function
///
int main(int argc, char* argv[]) {

	printf("Welcome to %s.\n", PACKAGE_STRING);

	hid_device* pDevice;
	int res;
	int ch;
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
	// Refresh to init the screen
	refresh();

	// log window
	wlog = CreateWinWithBorder("Log", TRUE, 0, 0, COLS, LINES/6);
	// send window
	wtx = CreateWinWithBorder("Send", FALSE, 0, LINES/6, COLS, 3);
	// receive window
	wrx = CreateWinWithBorder("Receive", TRUE, 0, LINES/6 + 3, COLS, LINES - LINES/6 - 3);

	// refresh
	RefreshAll();

	// Init Lib Knx Usb
	res = LKU_Init(&pDevice);
	if (res < 0) {
		perror("LKU_Init");
		exit(1);
	}
	LogPrint("LKU_Init", "file descriptor: %p.", pDevice);

	while(!toexit) {
		ch = getch();
		if ((ch == KEY_F(2)) || (ch == 'q')) {
			toexit = TRUE;
		}
		else if (ch == 's') {
			// send msg
			char smsg[LKU_KNX_MSG_LENGTH*2];
			wmove(wtx, 0, 0);
			wattron(wtx, A_BOLD);
			wprintw(wtx, "Write msg: ");
			wattroff(wtx, A_BOLD);
			echo();
			wgetnstr(wtx, smsg, LKU_KNX_MSG_LENGTH*2);
			noecho();
			wclear(wtx);
			if (SendStringMsg(pDevice, smsg) < 0) {
				LogPrint("SendStringMsg", "ERROR: message not sent");
			}
		}
		else {
			wprintw(wlog, "The pressed key is ");
			wattron(wlog, A_BOLD);
			wprintw(wlog, "%c\n", ch);
			wattroff(wlog, A_BOLD);
		}

		RefreshAll();
	}

	res = LKU_Deinit(pDevice);
	if (res < 0) {
		perror("LKU_Deinit");
		exit(1);
	}

	// End curses mode
	endwin();

	printf("Done.\n");

	return 0;
}

//-END----------------------------- Functions --------------------------------//










