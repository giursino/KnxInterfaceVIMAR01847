//******************************************************************************
/// @file 01847-temp-reader.c
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "config.h"
#include "01847-temp.h"

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
LOCAL char *socket_path = SOCKET_FILE;
//-END------------------------------ Variables -------------------------------//


//-START------------------------------- ISR ----------------------------------//
//-END--------------------------------- ISR ----------------------------------//


//-START--------------------------- Functions --------------------------------//


int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	char buf[100];
	int fd, rc;

	if (argc > 1)
		socket_path = argv[1];
#ifdef CONNECTION_ORIENTED
	printf("Mode: Connection-oriented\n");
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(-1);
	}
#else
	printf("Mode: Connection-less\n");
	if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket error");
		exit(-1);
	}
#endif
	fprintf(stdout, "socket %i\n", fd);

#ifdef CONNECTION_ORIENTED
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	if (*socket_path == '\0') {
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	} else {
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
	}

	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("connect error");
		exit(-1);
	}
#endif


	while (1) {
		{
			SocketData_Type buf;
			rc = read(fd, &buf, sizeof(buf));
			if (rc > 0) {
				fprintf(stdout, "read %i bytes: time: %s, temperature=%f\n", rc, buf.time, buf.temperature);
			}
			else {
				perror("write error");
				exit(-1);
			}
		}
	}

	return 0;
}

//-END----------------------------- Functions --------------------------------//










