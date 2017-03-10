/**
 * Nicolas JOURDEN - 13/01/2017
 **/

#include "egaston.h"

#define LED PB5
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

FILE uart_io = FDEV_SETUP_STREAM(egaston_serial_putchar, egaston_serial_getchar, _FDEV_SETUP_RW);

int main() {
  // Value requested:
  int16_t requestedA = 0;
  int16_t requestedB = 0;
  sNUNCHUCK_DATA data;
  uint8_t default_x = 0;
  uint8_t default_y = 0;
  uint8_t ret = 0;
  uint8_t error_counter = 0;

  // Configure LED GPIO:
  set_output(DDRD, LED);  

  // Init IO:
  egaston_serial_init();
  stdout = stdin = &uart_io;
  puts("Serial ready...");
  printf ("Data: %08x, size %d\n", &data, sNUNCHUCK_DATA_SIZE);

  gaston_pwm_init();
  puts("PWM ready...");

  egaston_nunchuck_init();
  puts("Numchuck I2C ready...");

  egaston_nunchuck_probe();
  puts("Nunchuck probed...");

  // For status:
  output_high(PORTB, LED);
  gaston_pwm_write(0, 0);

  // Try to get the value:
  egaston_nunchuck_read_data(&data);
  printf ("Nunchuck init: X:%3d - Y:%03d\n",
      data.joystick_x, data.joystick_y
  );
  default_x = ( data.joystick_x == 0 ? NUNCHUCK_DEFAULT_IDLE_X : data.joystick_x );
  default_y = ( data.joystick_y == 0 ? NUNCHUCK_DEFAULT_IDLE_Y : data.joystick_y );

  // Read value of the controller:
  while (1)
  {
    output_low(PORTB, LED);
    ret = egaston_nunchuck_read_data(&data);
    printf ("Nunchuck says: X:%3d - Y:%03d - C:%01d - X:%01d, ERR: %02d - DX:%03d DY%03d\n",
      data.joystick_x, data.joystick_y,
      data.button_c_status, data.button_z_status,
      error_counter,
      default_x, default_y
    );

    // No news from nunchuck:
    if (ret == 1)
    {
      egaston_nunchuck_probe();
      error_counter++;
    }

    // Can not allow 0: 
    if (data.joystick_y == 0 || data.joystick_x == 0)
    {
      puts ("Values set to 0 ... skipping....");
      continue;
    }

    // Compute requested PWM value:
    // Going backwards:    
    if ( data.joystick_y < default_y )
    {
      requestedA = ( (int32_t) (default_y - data.joystick_y) * EGASTON_PWM_MAX ) / default_y;
      requestedA *= -1;
    }
    else if ( data.joystick_y > default_y )
    {
      requestedA = ( (int32_t) ( data.joystick_y - default_y ) * EGASTON_PWM_MAX ) / default_y;
    }
    else {
      requestedA = 0;
    }

    // No differential yet:
    requestedB = requestedA;

    // Apply PWM:
    gaston_pwm_write(requestedA, requestedB);
    printf ("PWM set to: %d - %d\n", requestedA, requestedB);

    // pause?
    _delay_ms(100);
    output_high(PORTB, LED);
  }

  egaston_nunchuck_disinit();
}

