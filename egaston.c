/**
 * Nicolas JOURDEN - 13/01/2017
 **/

#include "egaston.h"


FILE uart_io = FDEV_SETUP_STREAM(egaston_serial_putchar, egaston_serial_getchar, _FDEV_SETUP_RW);

int16_t requestedA = 0.0;
int16_t requestedB = 0.0;
sNUNCHUCK_DATA data;
uint8_t nun_center_x = 0;
uint8_t nun_center_y = 0;
uint8_t ret = 0x01;
int8_t lut_y[256];
int8_t lut_x[256];
uint8_t error_counter = 0;

void loadLUT()
{
  // Prepare the lut for y:
  lut_y[nun_center_y] = 0;
  for (int i = nun_center_y+1; i <= 255; i++)
  {
    lut_y[i] = ( ( i - nun_center_y ) * 100 ) / (NUNCHUCK_MAX_Y - nun_center_y);
  }
  for (int i = nun_center_y-1; i >= 0; i--)
  {
    lut_y[i] = -1 * ( ( i - nun_center_y ) * 100 ) / (NUNCHUCK_MIN_Y - nun_center_y);
  }

  // Prepare the lut for x:
  lut_x[nun_center_x] = 0;
  for (int i = nun_center_x+1; i <= 255; i++)
  {
    lut_x[i] = ( ( i - nun_center_x ) * 100 ) / (NUNCHUCK_MAX_X - nun_center_x);
  }
  for (int i = nun_center_x-1; i >= 0; i--)
  {
    lut_x[i] = -1 * ( ( i - nun_center_x ) * 100 ) / (NUNCHUCK_MIN_X - nun_center_x);
  }
}

int main() {

  // Configure LED GPIO:
  set_output(DDRD, LED);  
  output_high(PORTB, LED);

  // Init IO:
  egaston_serial_init();
  stdout = stdin = &uart_io;
  puts("Serial ready...");
  printf ("Data: %08x, size %d\n", &data, sNUNCHUCK_DATA_SIZE);

  // PWM:
  gaston_pwm_init();
  gaston_pwm_set(0.0, 0.0);
  puts("PWM ready...");

  // Nunchuck:
  do
  {
    puts("Probing the nunchuck...");
    egaston_nunchuck_init();
    puts("Nunchuck I2C ready...");
    egaston_nunchuck_probe();
    puts("Nunchuck probed...");

    // Try to get the value:
    ret = egaston_nunchuck_read_data(&data);
  } while (egaston_nunchuck_cheksum(&data) == 0x00);

  // Ready:
  printf ("Nunchuck init: X: %03d - Y: %03d - %d\n",
      data.joystick_x,
      data.joystick_y,
      ret
  );
  nun_center_x = ( data.joystick_x == 0 ? NUNCHUCK_DEFAULT_IDLE_X : data.joystick_x );
  nun_center_y = ( data.joystick_y == 0 ? NUNCHUCK_DEFAULT_IDLE_Y : data.joystick_y );

  // Load the LUT:
  loadLUT();

  // Enable watchog:
//  init_watchdog();

  // Read value of the controller:
  while (1)
  {
    output_low(PORTB, LED);
    ret = egaston_nunchuck_read_data(&data);

    // No news from nunchuck:
    if (ret == 1)
{
    do
  {
    puts("Probing the nunchuck...");
    egaston_nunchuck_init();
    puts("Nunchuck I2C ready...");
    egaston_nunchuck_probe();
    puts("Nunchuck probed...");

    // Try to get the value:
    ret = egaston_nunchuck_read_data(&data);
  } while (egaston_nunchuck_cheksum(&data) == 0x00);
}

    // On of the button must be pressed:
    if (
      (data.button_c_status == 0x01 && data.button_z_status == 0x00)
      ||
      (data.button_c_status == 0x00 && data.button_z_status == 0x01)
    ) {
      requestedA = lut_y[data.joystick_y];
      requestedB = lut_y[data.joystick_y];

      // To the left forward: reduced speed on left A.
      if (data.joystick_x < nun_center_x && data.joystick_y > nun_center_y)
      {
        requestedA = lut_y[data.joystick_y] + lut_x[data.joystick_x];
      }
      // To the right forward: reduced speed on right B.
      else if (data.joystick_x > nun_center_x && data.joystick_y > nun_center_y)
      {
        requestedB = lut_y[data.joystick_y] - lut_x[data.joystick_x];
      }
      // To the left backward: reduced speed on left A.
      else if (data.joystick_x < nun_center_x && data.joystick_y < nun_center_y)
      {
        requestedA = lut_y[data.joystick_y] - lut_x[data.joystick_x];
      }
      // To the right backward: reduced speed on right B.
      else if (data.joystick_x > nun_center_x && data.joystick_y < nun_center_y)
      {
        requestedB = lut_y[data.joystick_y] + lut_x[data.joystick_x];
      }
      // Rotate left:
      else if (data.joystick_x < nun_center_x && data.joystick_y == nun_center_y)
      {
        requestedA = lut_x[data.joystick_x];
        requestedB = -1 * (lut_x[data.joystick_x]);
      }
      // Rotate right:
      else if (data.joystick_x > nun_center_x && data.joystick_y == nun_center_y)
      {
        requestedA = lut_x[data.joystick_x];
        requestedB = -1 * lut_x[data.joystick_x];
      }

      printf ("throttle - differential: [%d;%d] %d / %d -> A: %d - B: %d\n",
        data.joystick_y,
        data.joystick_x,
        lut_y[data.joystick_y],
        lut_x[data.joystick_x],
        requestedA,
        requestedB
      );
    }
    else
    {
      requestedA = 0.0;
      requestedB = 0.0;
    }

    // Apply PWM:
    gaston_pwm_set(requestedA, requestedB);

    // pause?
    _delay_ms(100);
    output_high(PORTB, LED);
  }

  egaston_nunchuck_disinit();
}

