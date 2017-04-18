#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "libknxusb.h"
#include "libknxusb_priv.h"


/* A test case that does nothing and succeeds. */
static void null_test_success(void **state) {
    (void) state; /* unused */
}


/* Test della funzione LKU_LData2CEmi */
static void test_LKU_LData2CEmi(void **state) {

	uint8_t buf[8];
	uint8_t i=0;
	buf[i++] = 0xbc;
	buf[i++] = 0x10;
	buf[i++] = 0x01;
	buf[i++] = 0x0c;
	buf[i++] = 0x0a;
	buf[i++] = 0xE1;
	buf[i++] = 0x00;
	buf[i++] = 0x81;

	uint8_t retbuf[64];


	int len = LKU_LData2CEmi(buf, 8, retbuf, 64);

	assert_int_not_equal(len, -1);
	assert_int_equal(buf[8], retbuf[len]);
}


int main(void) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_LKU_LData2CEmi),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

