#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <hidapi/hidapi.h>

/* Inclusion of C file module to test! This is useful to test local function */
#include "01847-temp.c"


/* A test case that does nothing and succeeds. */
static void null_test_success(void **state) {
    (void) state; /* unused */
}


/* Test of LKU_LData2CEmi */
static void test_DPTFloat_01(void **state) {

	uint8_t t_in[2] = {0x0C, 0x1A};
	float t_out = DptValueTemp2Float(t_in);
	float t_expected = 21.0f;
	assert_memory_equal(&t_out, &t_expected, sizeof(float));
}


int main(void) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_DPTFloat_01),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

