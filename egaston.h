/**
 * Nicolas JOURDEN - 13/01/2017
 **/

// Asign ports: UART, I2C, PWM;
// Check config
//

/**
 * PWM:
 *  Left engine: PD5. (-)
 *  Right engine: PD6. (+)
 *  
 *  Left engine: PD3. (-)
 *  Right engine: PB3. (+)
 *
 * I2C:
 *  SDA - PC4 (27 or A4) and SCL PC5 (28 or A5).
 *
 * UART:
 *  PD0 (RX) and PD1 (TX)
 */

#include <inttypes.h>

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define SCL_CLOCK 100000UL
#define BAUD 9600

#define LED PB5
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

#ifndef sNUNCHUCK_DATA_SIZE
#pragma pack(push, 1)
typedef struct {
  uint8_t joystick_x;
  uint8_t joystick_y;
  int8_t accelerometer_x;
  int8_t accelerometer_y;
  int8_t accelerometer_z;
  uint8_t button_c_status : 1;
  uint8_t button_z_status : 1;
  uint8_t accelerometer_x_2 : 2;
  uint8_t accelerometer_y_2 : 2;
  uint8_t accelerometer_z_2 : 2;
} sNUNCHUCK_DATA;
#pragma pack(pop)
#define sNUNCHUCK_DATA_SIZE	sizeof(sNUNCHUCK_DATA)
#endif

#ifndef EGASTON_VERSION
#define EGASTON_VERSION 0.1


extern int16_t requestedA;
extern int16_t requestedB;
extern sNUNCHUCK_DATA data;
extern uint8_t nun_center_x;
extern uint8_t nun_center_y;
extern uint8_t ret;
extern int8_t lut_y[256];
extern int8_t lut_x[256];
extern uint8_t error_counter;

#endif

#include "egaston_serial.h"
#include "egaston_nunchuck.h"
#include "egaston_pwm.h"
#include "egaston_watchdog.h"
