/**
 * Nicolas JOURDEN - 13/01/2017
 **/

#include "egaston.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

// PWM:
void gaston_pwm_init (void);
uint8_t gaston_pwm_read (void);
void gaston_pwm_write (int16_t pRequestedValueA, int16_t pRequestedValueB);

#define EGASTON_PWM_MAX		255
#define EGASTON_PWM_MIN		0

