#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

// PWM:
void gaston_pwm_init (void);
void gaston_pwm_set (int16_t pRequestedValueA, int16_t pRequestedValueB);

#define EGASTON_PWM_MAX		255
#define EGASTON_PWM_MIN		0
#define get_pwm_from_per(p1)    ( ( abs(p1) * EGASTON_PWM_MAX) / 100)
