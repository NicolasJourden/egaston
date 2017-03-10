/**
 * Nicolas JOURDEN - 13/01/2017
 **/

#include "egaston_pwm.h"

void gaston_pwm_init (void)
{
  // PD5 and PD6:
  DDRD |= (1 << DDD6) | (1 << DDD5);

  // Stop engine:
  OCR0A = 0;
  OCR0B = 0;

  // set none-inverting mode, fast PWM, prescaler to 8 and starts PWM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS01);

  // PD3 and PB3:
  DDRD |= (1 << DDD3);
  DDRB |= (1 << DDD3);

  // Stop engines:
  OCR2A = 0;
  OCR2B = 0;

  // Set none-inverting mode, fast PWM Mode:
  TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2B |= (1 << CS21);
}

uint8_t gaston_pwm_read (void)
{

  return 0;
}

void gaston_pwm_write (int16_t pRequestedValueA, int16_t pRequestedValueB)
{
  // Motor A:
  if (pRequestedValueA > 0)
  {
    OCR0A = (pRequestedValueA > EGASTON_PWM_MAX ? EGASTON_PWM_MAX : pRequestedValueA);
    OCR0B = 0x00;
  }
  else if (pRequestedValueA < 0)
  {
    OCR0A = 0x00;
    OCR0B = (pRequestedValueA < -EGASTON_PWM_MAX ? EGASTON_PWM_MAX : -pRequestedValueA);
  }
  else
  {
    OCR0A = 0x00;
    OCR0B = 0x00;
  }

  // Motor B:
  if (pRequestedValueB > 0)
  {
    OCR2A = (pRequestedValueB > EGASTON_PWM_MAX ? EGASTON_PWM_MAX : pRequestedValueB);
    OCR2B = 0x00;
  }
  else if (pRequestedValueB < 0)
  {
    OCR2A = 0x00;
    OCR2B = (pRequestedValueB < -EGASTON_PWM_MAX ? EGASTON_PWM_MAX : -pRequestedValueB);
  }
  else
  {
    OCR2A = 0x00;
    OCR2B = 0x00;
  }
}
