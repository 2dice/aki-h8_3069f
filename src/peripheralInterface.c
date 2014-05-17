#include "defines.h"
#include "serial.h"
#include "peripheralInterface.h"

////////////////////serial interface////////////////////
int SCI0_receiving(void)
    {
      return SCI0_RECEIVING;
    }

int put_char(unsigned char c)
{
  if (c == '\n')
    serial_send_byte('\r');

  return serial_send_byte(c);
}

int put_byte_data(unsigned char c)
{

  return serial_send_byte(c);

}

unsigned char get_char(void)
{
  unsigned char c = serial_recv_byte();
  c = (c == '\r') ? '\n' : c;
  put_char(c);
  return c;
}

unsigned char get_byte_data(void)
{
  unsigned char c = serial_recv_byte();
  return c;
}

int put_string(char str[])
{
  while (*str)
    put_char(*(str++));

  return 0;
}

int get_string(char *store_array)
{
  int i = 0;
  unsigned char c;

  do{
    c = get_char();
    if(c == '\n')
      c = '\0';
    store_array[i++] = c;
  }while(c);
  return i - 1;
}

int put_hex(unsigned long value, int digit_number)
{
  char hex_buffer[9];
  char *hex_pointer;

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

int put_dec(unsigned int value)
{
  char dec_buffer[9];
  char *dec_pointer;
  int value_size = sizeof(value);

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
