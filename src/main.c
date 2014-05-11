#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(void)
{
  serial_init();

  put_string("Hello World!\n");

  put_hex(0x10, 4);
  put_string("\n");
  put_hex(0xffff,2);
  put_string("\n");
  put_dec(9501);
  put_string("\n");
  put_dec(11901);
  
  while (1)
    ;

  return 0;
}
