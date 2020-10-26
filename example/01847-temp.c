//******************************************************************************
/// @file 01847-temp.c
/// @brief Example application to monitor temperature using KNX Interface VIMAR 01847
///
/// @note It create a socket server to send the temperature values gets from KNX bus
//******************************************************************************


//-START--------------------------- Definitions ------------------------------//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include "config.h"
#include "libknxusb.h"
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
LOCAL bool toexit = false;
//-END------------------------------ Variables -------------------------------//


//-START------------------------------- ISR ----------------------------------//
//-END--------------------------------- ISR ----------------------------------//


//-START--------------------------- Functions --------------------------------//

/// Print hex message on recevie buffer
LOCAL void PrintReceivedMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	fprintf(stdout, "%s: ", strprefix);
	for (i=0; i<(u8Len-1); i++){
		switch (i) {
			case 1:
			case 3:
			case 5:
			case 6:
			case 7:
			case 8:
				fprintf(stdout, " ");
				break;

			default:
				break;
		}
		fprintf(stdout, "%.2X", pMsg[i]);
	}
	fprintf(stdout, " %.2X", pMsg[i]);
	fprintf(stdout, ".\n");
}

/// Thread function to handle the receiving of messages
LOCAL void* ThreadKnxRx(void *arg) {
	int res;
	uint8_t rxbuf[65];
	hid_device* pDevice = ((ThreadKnxArgs_Type*) arg)->pDevice;
	int socket = ((ThreadKnxArgs_Type*) arg)->socket;

	while(1) {
		res = LKU_ReceiveLBusmonMessage(pDevice, rxbuf, 65);
		if (res < 0) {
			perror("Error receiving data.");
			exit(1);
		}

		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		char sTime[64];
		strftime(sTime, sizeof(sTime), "%c", tm);
		PrintReceivedMsg(sTime, rxbuf, res);

		// Send to socket
		SocketData_Type txbuf;
		bool tosend = false;
		txbuf.value = 0;
		sprintf(txbuf.track, "Unknown");
		strftime(txbuf.time, sizeof(txbuf.time), "%Y-%m-%d %H:%M:%S.0", tm);

#ifdef DEBUG
		// send every message received to the socket
		tosend = true;
#endif

		// Ta zona giorno
		if ((rxbuf[3]==0x21) && (rxbuf[4]==0x77)) {
			sprintf(txbuf.track, "Ta_giorno");
			txbuf.value = DptValueTemp2Float(&rxbuf[8]);
			fprintf(stdout, "*** Zona giorno, Ta=%.1f ***\n", txbuf.value);
			tosend=true;
		}

		// T esterna
		if ((rxbuf[3]==0x0C) && (rxbuf[4]==0x60)) {
			sprintf(txbuf.track, "T_ext");
			txbuf.value = DptValueTemp2Float(&rxbuf[8]);
			fprintf(stdout, "*** Temperatura esterna, T=%.1f ***\n", txbuf.value);
			tosend=true;
		}

		// Ta zona notte
		if ((rxbuf[3]==0x21) && (rxbuf[4]==0x9D)) {
			sprintf(txbuf.track, "Ta_notte");
			txbuf.value = DptValueTemp2Float(&rxbuf[8]);
			fprintf(stdout, "*** Zona notte, Ta=%.1f ***\n", txbuf.value);
			tosend=true;
		}

		// Valvola zona giorno
		if ((rxbuf[3]==0x21) && (rxbuf[4]==0x8C)) {
			sprintf(txbuf.track, "Valvola_giorno");
			if (rxbuf[7]==0x84) {
				txbuf.value = 1;
			}
			else {
				txbuf.value = 0;
			}
			fprintf(stdout, "*** Valvola giorno = %.0f ***\n", txbuf.value);
			tosend=true;
		}

		// Valvola zona notte
		if ((rxbuf[3]==0x21) && (rxbuf[4]==0xB2)) {
			sprintf(txbuf.track, "Valvola_notte");
			if (rxbuf[7]==0x81) {
				txbuf.value = 1;
			}
			else {
				txbuf.value = 0;
			}
			fprintf(stdout, "*** Valvola notte = %.0f ***\n", txbuf.value);
			tosend=true;
		}


		if (tosend) {
			fprintf(stdout, "Sending data: time=\"%s\" track=\"%s\" value=%f\n", txbuf.time, txbuf.track, txbuf.value);
			if ((res = write(socket, &txbuf, sizeof(txbuf))) != sizeof(txbuf)) {
				if (res > 0)
					fprintf(stderr, "partial write\n");
				else {
					perror("write error");
					exit(-1);
				}
			}
		}
	}
	return NULL;
}


/// Signal handler, to exit from main loop
LOCAL void SignalHandler(int signo) {
	switch  (signo) {
		case SIGINT:
		case SIGTERM:
			printf("Catched signal %i.\n", signo);
			toexit=true;
			break;
		case SIGPIPE:
			printf("Exit cause client disconnected.\n");
			toexit=true;
			break;
		default:
			break;
	}

}

/// Signal handler, to exit from main loop
LOCAL void SignalHandlerBeforeConnection(int signo) {
	switch  (signo) {
		case SIGINT:
		case SIGTERM:
			printf("Catched signal %i before client connection.\n", signo);
			unlink(socket_path);
			printf("Done.\n");
			exit(0);
		default:
			break;
	}

}

/// Main function
///
int main(int argc, char* argv[]) {

	printf("Welcome to %s.\n", PACKAGE_STRING);

	hid_device* pDevice;
	int res;
	int ch;


	// Init Lib Knx Usb
	res = LKU_Init(&pDevice, LKU_MODE_BUSMONITOR);
	if (res < 0) {
		perror("LKU_Init error");
		exit(1);
	}


	// Create socket to comunicate with other modules
	int fd;
#ifdef CONNECTION_ORIENTED
	printf("Mode: Connection-oriented\n");
	if ( (fd = socket(AF_UNIX, SOCK_STREAM , 0)) == -1) {
		perror("Socket error");
		exit(1);
	}
#else
	printf("Mode: Connection-less\n");
	if ( (fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("Socket error");
		exit(1);
	}
#endif

	// Binding address to socket
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	if (*socket_path == '\0') {
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	} else {
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
		unlink(socket_path);
	}
	if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("Bind error");
		exit(1);
	}


	// Register signal handler
	if (signal(SIGINT, SignalHandlerBeforeConnection) == SIG_ERR) {
		perror("Cannot catch SIGINT");
		exit(1);
	}
	if (signal(SIGTERM, SignalHandlerBeforeConnection) == SIG_ERR) {
		perror("Cannot catch SIGTERM");
		exit(1);
	}

#ifdef CONNECTION_ORIENTED
	// Listening to new connections
	printf("Waiting connection from client...\n");
	if (listen(fd, 5) == -1) {
		perror("listen error");
		exit(1);
	}

	// Accepting new connections
	int cl;
	if ((cl = accept(fd, NULL, NULL)) == -1) {
		perror("accept error");
		exit(1);
	}
	printf("Connected.\n");
#else
	int cl = fd;
#endif

	// Creating thread to handle the received message
	pthread_t threadRx;
	ThreadKnxArgs_Type threadRxArgs;
	threadRxArgs.pDevice = pDevice;
	threadRxArgs.socket = cl;
	if (pthread_create(&threadRx, NULL, ThreadKnxRx, &threadRxArgs)) {
		perror("Error creating thread");
		exit(1);
	}

	// Register signal handler
	if (signal(SIGINT, SignalHandler) == SIG_ERR) {
		perror("Cannot catch SIGINT");
		exit(1);
	}
	if (signal(SIGTERM, SignalHandler) == SIG_ERR) {
		perror("Cannot catch SIGTERM");
		exit(1);
	}
	if (signal(SIGPIPE, SignalHandler) == SIG_ERR) {
		perror("Cannot catch SIGPIPE");
		exit(1);
	}

	printf("Monitoring and sleeping...\n");

#ifdef NO_DAEMON
	printf("Press 'q' or Ctrl-C or kill to to quit...\n");
#endif

	while(!toexit) {

#ifdef NO_DAEMON
		ch = getchar();
		if (ch == 'q') {
			toexit = true;
		}

		{
			// Press ENTER to send random data (20-25) to client on 'Test' track
			SocketData_Type buf;
			int rc = sizeof(buf);
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);
			sprintf(buf.track, "Test");
			strftime(buf.time, sizeof(buf.time), "%Y-%m-%d %H:%M:%S.0", tm);
			buf.value=20 + (rand()%5);
			printf("send data [%p, %i bytes] to socket [%i] \n", &buf, rc, cl);

			if (write(cl, &buf, rc) != rc) {
				if (rc > 0)
					fprintf(stderr, "partial write\n");
				else {
					perror("write error");
					exit(-1);
				}
			}
		}

#else
		sleep(1);

		// keep-alive
		static int keealive_tmo = 0;
		if ((keealive_tmo--) == 0) {
			keealive_tmo = 10;
			if (write(cl, "", 1) != 1) {
				fprintf(stderr, "client disconnected\n");
				toexit = true;
			}
		}
#endif

	}

	// End thread
	if (pthread_cancel(threadRx)) {
		perror("Error ending thread");
		exit(1);
	}
	if (pthread_join(threadRx, NULL)) {
		perror("Error waiting thread");
		exit(1);
	}

	// Destroy socket
	close(cl);
	close(fd);
	unlink(socket_path);

	// Deinit Lib Knx Usb
	res = LKU_Deinit(pDevice);
	if (res < 0) {
		perror("LKU_Deinit error");
		exit(1);
	}

	printf("Done.\n");

	return 0;
}

//-END----------------------------- Functions --------------------------------//










