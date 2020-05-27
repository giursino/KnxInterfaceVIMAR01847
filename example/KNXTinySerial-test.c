//******************************************************************************
/// @file KNXTinySerial-test.c
/// @brief Example appplication to test KnxTinyInterface device without library
//******************************************************************************

#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */

void main(void) {
	int fd;/*File Descriptor*/

	printf("\n +----------------------------------+");
	printf("\n |        Serial Port Read          |");
	printf("\n +----------------------------------+");

	/*------------------------------- Opening the Serial Port -------------------------------*/

	/* Change /dev/ttyUSB0 to the one corresponding to your system */

	fd = open("/dev/serial0", O_RDWR | O_NOCTTY );
	/* O_RDWR   - Read/Write access to serial port       */
	/* O_NOCTTY - No terminal will control the process   */
	/* Open in blocking mode,read will wait              */

	if (fd == -1) /* Error Checking */
		printf("\n  Error! in opening serial port ");
	else
		printf("\n  Serial opened successfully ");

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

	if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");
	else
		printf("\n  BaudRate = 19200 \n  StopBits = 1 \n  Parity   = even\n");

	/*------------------------------- Read data from serial port -----------------------------*/

	tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */

	while (1) {


		char read_buffer[32]; /* Buffer to store the data received              */
		int bytes_read = 0; /* Number of bytes read by the read() system call */
		int i = 0;

		bytes_read = read(fd, &read_buffer, 32); /* Read the data                   */

		printf("\n\n  Bytes Rxed: %d\n", bytes_read); /* Print the number of bytes read */

		for (i = 0; i < bytes_read; i++) /*printing only the received characters*/
			printf("%0.2X ", read_buffer[i]);

		printf("\n +----------------------------------+\n");
	}

	close(fd); /* Close the serial port */

}
