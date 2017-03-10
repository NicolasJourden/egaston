
#include "egaston.h"

#include "i2c_master.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>
#include <string.h>

#ifndef sNUNCHUCK_DATA_SIZE
#pragma pack(push, 1)
typedef struct {
  uint8_t joystick_x;
  uint8_t joystick_y;
  int8_t accelerometer_x;
  int8_t accelerometer_y;
  int8_t accelerometer_z;
  uint8_t accelerometer_x_2 : 2;
  uint8_t accelerometer_y_2 : 2;
  uint8_t accelerometer_z_2 : 2;
  uint8_t button_c_status : 1;
  uint8_t button_z_status : 1;

} sNUNCHUCK_DATA;
#pragma pack(pop)
#define sNUNCHUCK_DATA_SIZE	sizeof(sNUNCHUCK_DATA)
#endif

#define NUNCHUCK_PACKET_SIZE        6
//#define NUNCHUCK_ADDRESS            0x52
#define NUNCHUCK_ADDRESS            0xA4
#define NUNCHUCK_BUTTON_PRESSED     0x00
#define NUNCHUCK_BUTTON_UNPRESSED   0x01

void egaston_nunchuck_init();
void egaston_nunchuck_probe();
void egaston_nunchuck_disinit();
uint8_t egaston_nunchuck_read_data(sNUNCHUCK_DATA * pData);

// Nunchuck constants:
// MAX is for full speed.
// MIN is for full speed reverse.
// IDLE is for stop.
#define NUNCHUCK_MAX_Y             255
#define NUNCHUCK_MAX_X             255
#define NUNCHUCK_MIN_Y             0
#define NUNCHUCK_MIN_X             0
#define NUNCHUCK_DEFAULT_IDLE_Y    124
#define NUNCHUCK_DEFAULT_IDLE_X    129

