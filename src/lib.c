#include "defines.h"
#include "serial.h"
#include "lib.h"

int put_char(unsigned char c)
{
  if (c == '\n')
    serial_send_byte('\r');
  return serial_send_byte(c);
}

int put_string(unsigned char *str)
{
  while (*str)
    put_char(*(str++));
  return 0;
}
