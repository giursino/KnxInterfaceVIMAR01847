#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <hidapi/hidapi.h>

/* Inclusion of C file module to test! This is useful to test local function */
#include "cemi.c"
#include "knxhid.c"
#include "libknxusb.c"


/* A test case that does nothing and succeeds. */
static void null_test_success(void **state) {
    (void) state; /* unused */
}


/* Test of LKU_LData2CEmi */
static void test_LKU_LData2CEmi(void **state) {

	uint8_t buf[LKU_KNX_MSG_LENGTH];
	uint8_t i=0;
	buf[i++] = 0xbc;
	buf[i++] = 0x10;
	buf[i++] = 0x01;
	buf[i++] = 0x0c;
	buf[i++] = 0x0a;
	buf[i++] = 0xE1;
	buf[i++] = 0x00;
	buf[i++] = 0x81;

	uint8_t retbuf[LKU_CEMI_MSG_LENGTH];


	int ret = LKU_LData2CEmi(buf, i, retbuf, LKU_CEMI_MSG_LENGTH);

	assert_int_not_equal(ret, -1);
	assert_int_equal(ret, 14+8);

	i=0;
	// KNX HID Report Header
	assert_int_equal(retbuf[i++], 0x01); //ReportId
	assert_int_equal(retbuf[i++], 0x13); //PacketInfo
	assert_int_equal(retbuf[i++], 0x13); //Datalength

	// KNX HID Report Body
	assert_int_equal(retbuf[i++], 0x00); //ProtocolVersion
	assert_int_equal(retbuf[i++], 0x08); //HeaderLength
	assert_int_equal(retbuf[i++], 0x00); //BodyLength
	assert_int_equal(retbuf[i++], 0x0B); //    "
	assert_int_equal(retbuf[i++], 0x01); //ProtocolId
	assert_int_equal(retbuf[i++], 0x03); //EMIID (cEMI)
	assert_int_equal(retbuf[i++], 0x00); //ManufacturerCode
	assert_int_equal(retbuf[i++], 0x00); //      "

	// KNX USB Transfer Protocol Body
	assert_int_equal(retbuf[i++], 0x11); //EMIMessageCode (29=rx, 11=tx)
	assert_int_equal(retbuf[i++], 0x00); //Data

	assert_int_equal(retbuf[i++], 0xBC);
	assert_int_equal(retbuf[i++], 0xE0);
	assert_int_equal(retbuf[i++], 0x10);
	assert_int_equal(retbuf[i++], 0x01);
	assert_int_equal(retbuf[i++], 0x0C);
	assert_int_equal(retbuf[i++], 0x0A);
	assert_int_equal(retbuf[i++], 0x01);
	assert_int_equal(retbuf[i++], 0x00);
	assert_int_equal(retbuf[i++], 0x81);
}


/* Test of LKU_CEmi2LData */
static void test_LKU_CEmi2LData(void **state) {

	uint8_t buf[LKU_CEMI_MSG_LENGTH];
	uint8_t i=0;

	// KNX HID Report Header
	buf[i++] = 0x01; //ReportId
	buf[i++] = 0x13; //PacketInfo
	buf[i++] = 0x13; //Datalength

	// KNX HID Report Body
	buf[i++] = 0x00; //ProtocolVersion
	buf[i++] = 0x08; //HeaderLength
	buf[i++] = 0x00; //BodyLength
	buf[i++] = 0x0B; //    "
	buf[i++] = 0x01; //ProtocolId
	buf[i++] = 0x03; //EMIID (cEMI)
	buf[i++] = 0x00; //ManufacturerCode
	buf[i++] = 0x00; //      "

	// KNX USB Transfer Protocol Body
	buf[i++] = 0x29; //EMIMessageCode (29=rx, 11=tx)
	buf[i++] = 0x00; //Data
	buf[i++] = 0xBC;
	buf[i++] = 0xE0;
	buf[i++] = 0x10;
	buf[i++] = 0x01;
	buf[i++] = 0x0C;
	buf[i++] = 0x0A;
	buf[i++] = 0x01;
	buf[i++] = 0x00;
	buf[i++] = 0x81;

	uint8_t retbuf[LKU_KNX_MSG_LENGTH];


	int ret = LKU_CEmi2LData(buf, i, retbuf, LKU_KNX_MSG_LENGTH);

	assert_int_not_equal(ret, -1);
	assert_int_equal(ret, 8);

	i=0;
	assert_int_equal(retbuf[i++], 0xBC);
	assert_int_equal(retbuf[i++], 0x10);
	assert_int_equal(retbuf[i++], 0x01);
	assert_int_equal(retbuf[i++], 0x0C);
	assert_int_equal(retbuf[i++], 0x0A);
	assert_int_equal(retbuf[i++], 0xE1);
	assert_int_equal(retbuf[i++], 0x00);
	assert_int_equal(retbuf[i++], 0x81);
}

/* Test of LKU_Decode */
static void test_LKU_Decode(void **state) {

	KNXHID_Frame buf;
	buf.KNX_HID_Report.KNX_HID_Report_Header.ReportID = 0x00;
	buf.KNX_HID_Report.KNX_HID_Report_Header.PacketInfo.sequence_number = 1;
	buf.KNX_HID_Report.KNX_HID_Report_Header.PacketInfo.packet_type = 3;

	uint8_t retbuf[LKU_KNX_MSG_LENGTH];


	LKU_KNXMSG_TYPE rettype;
	int ret = LKU_Decode(&buf, &rettype, retbuf, LKU_KNX_MSG_LENGTH);

	assert_int_not_equal(ret, -1);
	assert_int_equal(ret, 8);

	uint8_t i;
	assert_int_equal(retbuf[i++], 0xBC);
	assert_int_equal(retbuf[i++], 0x10);
	assert_int_equal(retbuf[i++], 0x01);
	assert_int_equal(retbuf[i++], 0x0C);
	assert_int_equal(retbuf[i++], 0x0A);
	assert_int_equal(retbuf[i++], 0xE1);
	assert_int_equal(retbuf[i++], 0x00);
	assert_int_equal(retbuf[i++], 0x81);
}


int main(void) {

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_LKU_LData2CEmi),
        cmocka_unit_test(test_LKU_CEmi2LData),
        cmocka_unit_test(test_LKU_Decode),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

