#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"
#include "command.h"
#include "interrupt.h"
#include "intr.h"
#include "serial.h"

static void intr(softvec_type_t type, uint32 sp)
    {
      int16 c;
      static int8 command[32];
      static int16 len;
      
      /* TODO:シリアル受信割込みハンドラへ登録 */
      /* if((int)sizeof(command) < get_string(command)) */
      /*   put_string("command too long\n"); */
      c = get_char();
      if(c != '\n')
        {
          command[len++] = c;
        }
      else{
        if(!string_compare_at_arbitrary_length(command, "load", 4))
        {
          command_load();
        }
        else if(!string_compare_at_arbitrary_length(command, "dump", 4))
        {
          command_dump();
        }
        else if(!string_compare_at_arbitrary_length(command, "run", 3))
        {
          command_run();
        }
        else if(!string_compare_at_arbitrary_length(command, "echo", 4))
        {
          put_string(command + 4);
          put_string("\n");
        }
        else
        {
          put_string("unknown.\n");
        }
        put_string("kzload> ");
        len = 0;
      }
    }

static int init(void)
{
  extern int data_start_load, data_start, edata, bss_start, ebss;

  /* PA=VA処理転送 */
  memory_data_copy
      (&data_start, &data_start_load, (long)&edata - (long)&data_start);
  set_data_in_memory(&bss_start, 0, (long)&ebss - (long)&bss_start);

  /* ソフトウェア割り込みベクタの初期化 */
  softvec_init();

  serial_init();

  return 0;
}

int main(void)
{
  /* 割込を無効にする */
  INTR_DISABLE;

  init();

  put_string("kzload (kozos boot loader) started.\n");

  softvec_setintr(SOFTVEC_TYPE_SERINTR, intr);
  serial_intr_recv_enable();
  
  put_string("kzload> ");

  /* 割込を有効にする */
  INTR_ENABLE;
  while(1)
    {
      asm volatile ("sleep");
    }

  return 0;
}
