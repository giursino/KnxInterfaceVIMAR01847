/*
 * Applicativo di test per l'articolo 01847
 *
 * Copyright (c) 2017 Giuseppe Ursino
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include "config.h"
#include "libknxusb.h"
#include "01847-test.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


int main(int argc, char* argv[])
{

	printf("Welcome to %s.\n", PACKAGE_STRING);

	uint8_t buf[3]= {1,2,3};
	uint8_t dest[3]= {0,0,0};

	int res= LKU_LData2cEmi(buf, dest, 3);

	printf("res=%d...dest=%d %d %d\n", res, dest[0], dest[1], dest[2]);


	return 0;
}
