/**
 * Nicolas JOURDEN - 13/01/2017
 **/

#include "egaston_nunchuck.h"

void egaston_nunchuck_init()
{
  i2c_init();
}

void egaston_nunchuck_probe()
{
  uint8_t ret = i2c_start(NUNCHUCK_ADDRESS+I2C_WRITE);
  if(ret==1) 
  {
    puts("1st start bad");
  }
  else {
    puts("1st start OK");
  }

  // Init nunchuck:
  i2c_write(0xF0);
  i2c_write(0x55);
  _delay_ms(2);
  i2c_write(0xFB);
  i2c_write(0x00);
  i2c_stop();
}

uint8_t egaston_nunchuck_read_data(sNUNCHUCK_DATA * pData)
{
  uint8_t i = 0;
  uint8_t * nc_data;
  nc_data = (uint8_t *) pData;
  memset(nc_data, 0, sNUNCHUCK_DATA_SIZE);

  // Start Nunchuck read by writing initiating a start then writing 0x00
  if(i2c_start(NUNCHUCK_ADDRESS+I2C_WRITE) == 1) 
  {
    puts("2nd start bad");
    return 1;
  }
#ifdef NUNCHUCK_DEBUG
  else {
    puts("2nd start OK");
  }
#endif

  i2c_write(0x00);
  i2c_stop();
  _delay_us(500);
 
  // begin read request
  i2c_start_wait(NUNCHUCK_ADDRESS+I2C_READ);
  for(i = 0; i < sNUNCHUCK_DATA_SIZE-1; i++)
  {
    nc_data[i]=i2c_readAck();
  }
  nc_data[sNUNCHUCK_DATA_SIZE-1]= i2c_readNak();
  i2c_stop();

  return 0;
}

