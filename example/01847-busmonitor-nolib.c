/*
 * Example application to test the KNX Interface VIMAR 01847
 * It do not use libknxusb
 * It only uses libhid
 *
 * Copyright (c) 2017 Giuseppe Ursino
 */

#include "config.h"



#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <hidapi/hidapi.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define CEMI_M_CODE_POS 11
#define CEMI_PROP_DATA_POS 18

#define CEMI_M_PropRead_req  0xFC
#define CEMI_M_PropRead_con  0xFB

#define CEMI_M_PropWrite_req 0xF6
#define CEMI_M_PropWrite_con 0xF5


#define MAX_STR 255

static bool toexit = false;

/// Signal handler, to exit from main loop
static void SignalHandler(int signo) {
  switch  (signo) {
    case SIGINT:
    case SIGTERM:
      printf("Catched signal %i.\n", signo);
      toexit=true;
      break;
    default:
      break;
  }

}

int main(int argc, char* argv[])
{
  int res;
  unsigned char buf[65];
  wchar_t wstr[MAX_STR];
  hid_device *handle;
  int i;

  printf("Welcome to %s.\n", PACKAGE_STRING);

  printf("hid_init...\n");

  // Initialize the hidapi library
  res = hid_init();

  // Open the device using the VID, PID,
  // and optionally the Serial number.
  printf("hid_open...");
  handle=NULL;
  if (handle==NULL) {
    handle = hid_open(0xc251, 0x1101, NULL);  // 01847
  }
  if (handle==NULL) {
    handle = hid_open(0x24a0, 0x1101, NULL);  // 01847
  }
  if (handle==NULL) {
    perror("Cannot open the device");
    exit(1);
  }
  printf("[handle=%p]\n", handle);


  // Read the Manufacturer String
  printf("hid_get_manufacturer_string...");
  res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
  printf("%i\n", res);
  wprintf(L"Manufacturer String: %s\n", wstr);

  // Read the Product String
  printf("hid_get_product_string...");
  res = hid_get_product_string(handle, wstr, MAX_STR);
  printf("%i\n", res);
  wprintf(L"Product String: %s\n", wstr);

  // Read the Serial Number String
  printf("hid_get_serial_number_string...");
  res = hid_get_serial_number_string(handle, wstr, MAX_STR);
  printf("%i\n", res);
  wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

  // Read Indexed String 1
  printf("hid_get_indexed_string...");
  res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
  printf("%i\n", res);
  wprintf(L"Indexed String 1: %s\n", wstr);

  // Flush buffer
  printf("Flush buffer...\n");
  do {
    res = hid_read_timeout(handle, buf, 65, 1000);
    printf(" flush %i bytes\n", res);
  } while (res > 0);

  // Enable bus-monitor mode
  printf("Enable bus-monitor mode...");
  const uint8_t msg_busmonitor_enable[] = {
    0x01, 0x13, 0x10, 0x00, 0x08, 0x00, 0x08, 0x01, 0x03, 0x00, 0x00,
    CEMI_M_PropWrite_req,
    0x00, 0x08, 0x01, 0x34, 0x10, 0x01, 0x01
  };
  res = hid_write(handle, msg_busmonitor_enable, sizeof(msg_busmonitor_enable));
  printf("%i\n", res);
  printf("Waiting response...");
  res = hid_read_timeout(handle, buf, 65, 1000);
  printf("%i\n", res);
  for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
  if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropWrite_con)) {
    printf("property written\n");
  }

  printf("Check bus-monitor mode...");
  const uint8_t msg_busmonitor_get[] = {
    0x01, 0x13, 0x0F, 0x00, 0x08, 0x00, 0x07, 0x01, 0x03, 0x00, 0x00,
    CEMI_M_PropRead_req,
    0x00, 0x08, 0x01, 0x34, 0x10, 0x01
  };
  res = hid_write(handle, msg_busmonitor_get, sizeof(msg_busmonitor_get));
  printf("%i\n", res);
  printf("Waiting response...");
  res = hid_read_timeout(handle, buf, 65, 1000);
  printf("%i\n", res);
  for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
  if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropRead_con) && (buf[CEMI_PROP_DATA_POS]==0x01)) {
    printf("bus-monitor enabled!\n");
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

  int len;

  // Read requested state
  printf("hid_read...\n");
  while(!toexit) {
    res = hid_read_timeout(handle, buf, 65, 1000);

    // Print out the returned buffer.
    if (res < 0) {
      perror("read");
    }
    else if (res == 0) {
      continue;
    }
    else {
      time_t timer;
      char timebuffer[26];
      struct tm* tm_info;

      time(&timer);
      tm_info = localtime(&timer);
      strftime(timebuffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
      printf("%s \t", timebuffer);
      int len=buf[2]+3;
      const int start = 20;
      for (i = start; i < len; i++) {
        if (i == start+0) {
            // ctrl
            printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if ((i == start+1) || (i == start+2)) {
            // src addr
            printf(ANSI_COLOR_GREEN "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if ((i == start+3) || (i == start+4)) {
            // dst addr
            printf(ANSI_COLOR_RED "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if (i == start+5) {
            // len
            printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if ((i > start+7) && (i < len-1)) {
            // data
            printf(ANSI_COLOR_MAGENTA "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else if (i == (len-1)) {
            // crc
            printf(ANSI_COLOR_YELLOW "%.2hhx " ANSI_COLOR_RESET, buf[i]);
        }
        else {
            printf("%.2hhx ", buf[i]);
        }
      }
      puts("");
    }
  }

  // Disable bus-monitor mode
  printf("Disable bus-monitor mode...");
  const uint8_t msg_busmonitor_disable[] = {
    0x01, 0x13, 0x10, 0x00, 0x08, 0x00, 0x08, 0x01, 0x03, 0x00, 0x00,
    CEMI_M_PropWrite_req,
    0x00, 0x08, 0x01, 0x34, 0x10, 0x01, 0x00
  };
  res = hid_write(handle, msg_busmonitor_disable, sizeof(msg_busmonitor_disable));
  printf("%i\n", res);
  printf("Waiting response...");
  res = hid_read_timeout(handle, buf, 65, 1000);
  printf("%i\n", res);
  for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
  if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropWrite_con)) {
    printf("property written\n");
  }

  printf("Check bus-monitor mode...");
  res = hid_write(handle, msg_busmonitor_get, sizeof(msg_busmonitor_get));
  printf("%i\n", res);
  printf("Waiting response...");
  res = hid_read_timeout(handle, buf, 65, 1000);
  printf("%i\n", res);
  for (i = 0; i < res; i++) {printf("%.2hhx ", buf[i]); if (i==(res-1)) { printf("\n");};}
  if ((res==64) && (buf[CEMI_M_CODE_POS]==CEMI_M_PropRead_con) && (buf[CEMI_PROP_DATA_POS]==0x00)) {
    printf("bus-monitor disabled!\n");
  }

  // Finalize the hidapi library
  printf("hid_exit...\n");
  res = hid_exit();

  return 0;
}
