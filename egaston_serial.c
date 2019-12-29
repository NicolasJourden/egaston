/**
 * Nicolas JOURDEN - 13/01/2017
 **/

#include "egaston.h"
#include "egaston_serial.h"

void egaston_serial_init()
{
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void egaston_serial_disinit()
{
}

void egaston_serial_putchar(char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
  UDR0 = c;
}

/*
void egaston_serial_putchar(char c)
{
  UDR0 = c;
  loop_until_bit_is_set(UCSR0A, TXC0); // Wait until transmission ready.
}
*/

char egaston_serial_getchar(void)
{
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

