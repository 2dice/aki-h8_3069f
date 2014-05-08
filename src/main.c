#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(void)
{
  serial_init();

  puts("Hello World!\n");

  while (1)
    ;

  return 0;
}
