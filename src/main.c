#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(void)
{
  serial_init();

  put_string("Hello World!\n");

  putxval(0x10, 0);
  put_string("\n");
  putxval(0xffff,0);
  put_string("\n");
  
  while (1)
    ;

  return 0;
}
