#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"
#include "interrupt.h"

static int init(void)
{
  extern int data_start_load, data_start, edata, bss_start, ebss;

  /* 割込を無効にする */
  INTR_DISABLE;

  /* PA=VA処理転送 */
  memory_data_copy
      (&data_start, &data_start_load, (long)&edata - (long)&data_start);
  set_data_in_memory(&bss_start, 0, (long)&ebss - (long)&bss_start);

  serial_init();
  timer_init();

  put_string("kzload (kozos boot loader) started.\n");
  put_string("kzload> ");

  /* 割込を有効にする */
  INTR_ENABLE;

  return 0;
}

int main(void)
{
  init();

  while(1)
    {
      asm volatile ("sleep");
    }

  return 0;
}
