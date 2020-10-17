//******************************************************************************
/// @file libknx810.c
/// @brief Library to interface to KNX bus with KNXTiny810 interface
//******************************************************************************

//-START--------------------------- Definitions ------------------------------//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

#include "config.h"
#include "libknx810.h"
#include "libknx810_priv.h"

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


/// Init library
/// @param 	pDevice return pointer to device handler created by library
/// @return	0 on success and -1 on error
///
GLOBAL int LK8_Init(int *pDevice) {
	int res = 0;
	int fd = 0;

	/*------------------------------- Opening the Serial Port -------------------------------*/

	fd = open("/dev/serial0", O_RDWR | O_NOCTTY );
	/* O_RDWR   - Read/Write access to serial port       */
	/* O_NOCTTY - No terminal will control the process   */
	/* Open in blocking mode,read will wait              */

	if (fd == -1) {
		perror("Cannot open serial port.\n");
		res =-1;
	}
	else {
		/*---------- Setting the Attributes of the serial port using termios structure --------- */

		struct termios SerialPortSettings; /* Create the structure                          */

		tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

		/* Setting the Baud rate */
		cfsetispeed(&SerialPortSettings, B19200); /* Set Read  Speed as 9600                       */
		cfsetospeed(&SerialPortSettings, B19200); /* Set Write Speed as 9600                       */

		/* 8N1 Mode */
		SerialPortSettings.c_cflag &= ~PARODD; /* even parity */
		SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
		SerialPortSettings.c_cflag &= ~CSIZE; /* Clears the mask for setting the data size             */
		SerialPortSettings.c_cflag |= CS8; /* Set the data bits = 8                                 */

		SerialPortSettings.c_cflag &= ~CRTSCTS; /* No Hardware flow Control                         */
		SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

		SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); /* Disable XON/XOFF flow control both i/p and o/p */
		SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

		SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

		/* Setting Time outs */
		SerialPortSettings.c_cc[VMIN] = 9; /* Read at least characters */
		SerialPortSettings.c_cc[VTIME] = 5; /* Wait timeout from last byte received */

		if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) { /* Set the attributes to the termios structure*/
			res = -1;
			perror("Cannot set attributes to serial port.\n");
		}
		else {
			tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */
		}
	}

	if (res < 0) {
		return -1;
	}

	*pDevice=fd;
	return 0;
}


/// De-init library
/// @param 	pDevice  pointer to device handler
/// @return	0 on success and -1 on error
///
GLOBAL int LK8_Deinit(int pDevice) {
	int res;
	res = close(pDevice);
	if (res < 0) {
		return -1;
	}
	return 0;
}


/// Send A_GroupValueWrite message to knx bus
/// @param 	pDevice	pointer to device handler
/// @param  addr	dest group address
/// @param  dpt		datapoint type
/// @param  payload	datapoint payload
/// @param  len		payload length
/// @return	0 on success and -1 on error
///
GLOBAL int LK8_SendGroupValueWrite(int pDevice, LK8_ADDR_TYPE addr,
		LK8_DPT_TYPE dpt, const uint8_t* payload, uint8_t len) {

	// TODO: not implemented
	printf("LK8_SendGroupValueWrite(): not implemented\n");
	return -1;
}

/// Send a raw message to knx bus
/// @param 	pDevice	pointer to device handler
/// @param  pMsg	point to msg buffer
/// @return	0 on success and -1 on error
///
GLOBAL int LK8_SendRawMessage(int pDevice, const uint8_t* pMsg, uint8_t u8MsgLen) {

	// TODO: not implemented
	printf("LK8_SendRawMessage(): not implemented\n");
	return -1;
}


/// Receive a raw message from knx bus. Function blocking.
/// @param 	pDevice	pointer to device handler
/// @param  pMsg	pointer to received msg buffer
/// @param  u8MsgLen received msg buffer len
/// @return	number of byte received and -1 on error
///
GLOBAL int LK8_ReceiveRawMessage(int pDevice, uint8_t* pMsg, uint8_t u8MsgLen) {
	uint8_t msg[LK8_MSG_LENGTH];
	int res;

	res = read(pDevice, msg, LK8_MSG_LENGTH);
	if (res < 0) {
		perror("Error on reading raw message");
		return -1;
	}
	return res;
}



LOCAL void DebugPrintMsg(const char* strprefix, const uint8_t* pMsg, uint8_t u8Len) {
	uint8_t i=0;
	printf("%s: ", strprefix);
	for (i=0; i<u8Len; i++){
		printf("%.2X ", pMsg[i]);
	}
	printf(".\n");
}
//-END----------------------------- Functions --------------------------------//

