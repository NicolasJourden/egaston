#include <stdlib.h>
#include <avr/io.h>
#include <util/setbaud.h>

void egaston_serial_init();

void egaston_serial_disinit();

void egaston_serial_putchar(char c);

char egaston_serial_getchar(void);


