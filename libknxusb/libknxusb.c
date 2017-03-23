#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "libknxusb.h"
#include "libknxusb_priv.h"

uint8_t LKU_LData2cEmi(uint8_t* pData, uint8_t* pCEmi, uint8_t u8len) {
	memcpy(pCEmi, pData, u8len);
	printf("...%s...", PIPPO);
	return 0;
}
