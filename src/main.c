#include "defines.h"
#include "serial.h"
#include "xmodem.h"
#include "lib.h"
#include "peripheralInterface.h"
#include "command.h"

static int init(void)
{
  extern int data_start_load, data_start, edata, bss_start, ebss;

  /* PA=VA処理転送 */
  memory_data_copy
      (&data_start, &data_start_load, (long)&edata - (long)&data_start);
  set_data_in_memory(&bss_start, 0, (long)&ebss - (long)&bss_start);

  serial_init();

  return 0;
}

int main(void)
{
  static char command[16];
  
  init();

  put_string("kzload (kozos boot loader) started.\n");

  while(1)
    {
      put_string("kzload> ");
      /* TODO:シリアル受信割込みハンドラへ登録 */
      /* if((int)sizeof(command) < get_string(command)) */
      /*   put_string("command too long\n"); */
      get_string(command);
      
      if(!string_compare(command, "load"))
      {
        command_load();
      }
      else if(!string_compare(command, "dump"))
      {
        command_dump();
      }
      else
      {
        put_string("unknown.\n");
      }
    }

  return 0;
}
