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

int putxval(unsigned long value, int column)
    {
      char buf[9];
      char *p;

      p = buf + sizeof(buf) -1;
      *(p--) = '\0';
      if (!value && !column)
        column++;
      while (value || column)
        {
          *(p--) = "0123456789abcdef"[value & 0xf];
          value >>= 4;
          if (column) column--;
        }
      put_string(p + 1);

      return 0;
    }
