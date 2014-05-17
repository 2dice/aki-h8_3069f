#include "defines.h"
#include "serial.h"
#include "peripheralInterface.h"

////////////////////serial interface////////////////////
int16 SCI0_receiving(void)
    {
      return SCI0_RECEIVING;
    }

int16 put_char(uint8 c)
{
  if (c == '\n')
    serial_send_byte('\r');

  return serial_send_byte(c);
}

int16 put_byte_data(uint8 c)
{

  return serial_send_byte(c);

}

uint8 get_char(void)
{
  uint8 c = serial_recv_byte();
  c = (c == '\r') ? '\n' : c;
  put_char(c);
  return c;
}

uint8 get_byte_data(void)
{
  uint8 c = serial_recv_byte();
  return c;
}

int16 put_string(int8 str[])
{
  while (*str)
    put_char(*(str++));

  return 0;
}

int16 get_string(int8 *store_array)
{
  int16 i = 0;
  uint8 c;

  do{
    c = get_char();
    if(c == '\n')
      c = '\0';
    store_array[i++] = c;
  }while(c);
  return i - 1;
}

int16 put_hex(uint32 value, int16 digit_number)
{
  int8 hex_buffer[9];
  int8 *hex_pointer;

  hex_pointer = hex_buffer + sizeof(hex_buffer) -1;
  *(hex_pointer--) = '\0';
  while (digit_number)
  {
    *(hex_pointer--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    digit_number--;
  }
  put_string(hex_pointer + 1);

  return 0;
}

int16 put_dec(uint16 value)
{
  int8 dec_buffer[9];
  int8 *dec_pointer;
  int16 value_size = sizeof(value);

  dec_pointer = dec_buffer + sizeof(dec_buffer) -1;
  *(dec_pointer--) = '\0';
  while (value)
  {
    *(dec_pointer--) = "0123456789"[value % 10];
    value /= 10;
    value_size--;
  }
  put_string(dec_pointer + 1);

  return 0;
}
