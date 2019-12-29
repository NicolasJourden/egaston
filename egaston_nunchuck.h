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

#define NUNCHUCK_PACKET_SIZE        6
//#define NUNCHUCK_ADDRESS            0x52
#define NUNCHUCK_ADDRESS            0xA4
#define NUNCHUCK_BUTTON_PRESSED     0x00
#define NUNCHUCK_BUTTON_UNPRESSED   0x01

void egaston_nunchuck_init();
void egaston_nunchuck_probe();
void egaston_nunchuck_disinit();
uint8_t egaston_nunchuck_read_data(sNUNCHUCK_DATA * pData);
uint8_t egaston_nunchuck_cheksum(sNUNCHUCK_DATA * pData);

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
#define getPercentX(p1,p2)	   
#define getPercentY(p1,p2)	   

