
CC=avr-gcc
OBJ=avr-objcopy
DUMP=avr-objdump
CFLAGS=-I.
TARGET=atmega328p
#TARGET=atmega8

CSTANDARD = -std=gnu99
#CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CFLAGS += -Wall -Wstrict-prototypes
#CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(CSTANDARD)
#CFLAGS += -DF_OSC=$(F_OSC)

all:  build

# egaston.out egaston_twi.out egaston_pwm.out
build:
	$(CC) -g $(CFLAGS) -Os -mmcu=$(TARGET) -c egaston_nunchuck.c
	$(CC) -g $(CFLAGS) -Os -mmcu=$(TARGET) -c egaston_serial.c
	$(CC) -g $(CFLAGS) -Os -mmcu=$(TARGET) -c i2c_master.c
	$(CC) -g $(CFLAGS) -Os -mmcu=$(TARGET) -c egaston_pwm.c
	$(CC) -g $(CFLAGS) -Os -mmcu=$(TARGET) -c egaston.c
	$(CC) -g $(CFLAGS) -g -mmcu=$(TARGET) -o egaston.elf \
		egaston.o \
		egaston_nunchuck.o \
		egaston_serial.o \
		i2c_master.o \
		egaston_pwm.o
	$(OBJ) -j .text -j .data -O ihex egaston.elf egaston.hex

dump:
	$(DUMP) -h -S egaston.elf > egaston.lst

clean:
	rm -f *.o
	rm -f egaston.hex
	rm -f *~

flash:
	avrdude -P /dev/ttyUSB0 -b 57600 -p m328p -c AVRISP -F -e -U flash:w:egaston.hex
