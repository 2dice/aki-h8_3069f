#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(void)
{
  serial_init();

  put_string("Hello World!\n");

  while (1)
    ;

  return 0;
}
