#include "defines.h"
#include "serial.h"
#include "lib.h"

static int init(void)
{
  extern int data_start_load, data_start, edata, bss_start, ebss;

  memory_data_copy
      (&data_start, &data_start_load, (long)&edata - (long)&data_start);
  set_data_in_memory(&bss_start, 0, (long)&ebss - (long)&bss_start);
  serial_init();

  return 0;
}

int global_data = 0x10;
int global_bss;
char global_bss_c;
static int static_data = 0x20;
static int static_bss;

static void printval(void)
    {
      put_string("global_data = ");
      put_hex(global_data, 4);
      put_string("\n");

      put_string("global_bss = ");
      put_hex(global_bss, 4);
      put_string("\n");

      put_string("global_bss_c = ");
      put_hex(global_bss_c, 4);
      put_string("\n");

      put_string("static_data = ");
      put_hex(static_data, 4);
      put_string("\n");

      put_string("static_bss = ");
      put_hex(static_bss, 4);
      put_string("\n");
    }

int main(void)
{
  init();

  put_string("Hello World!\n");

  printval();
  put_string("overwrite variables.\n");
  global_data = 0x20;
  global_bss = 0x30;
  global_bss_c = 0x31;
  static_data = 0x40;
  static_bss = 0x50;
  printval();
  
  while (1)
    ;

  return 0;
}
