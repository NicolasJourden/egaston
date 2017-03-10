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

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define SCL_CLOCK 100000UL
#define BAUD 9600

#ifndef EGASTON_VERSION
#define EGASTON_VERSION 0.1

#include "egaston_serial.h"
#include "egaston_nunchuck.h"
#include "egaston_pwm.h"

#endif
