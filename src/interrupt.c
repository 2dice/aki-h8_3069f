#include "defines.h"
#include "intr.h"
#include "interrupt.h"
#include "lib.h"
#include "command.h"
#include "peripheralInterface.h"

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

/* ソフトウェア割り込みベクタの初期化 */
int16 softvec_init(void)
{
  int16 type;
  for(type = 0; type < SOFTVEC_TYPE_NUM; type++)
    softvec_setintr(type, NULL);
  return 0;
}

/* ソフトウェア割り込みベクタの設定 */
int16 softvec_setintr(softvec_type_t type, softvec_handler_t handler)
{
  SOFTVECS[type] = handler;
  return 0;
}

/* 共通割込みハンドラ．ソフトウェア割り込みベクタを見て各ハンドラに分岐する */
void interrupt(softvec_type_t type, uint32 sp)
{
  softvec_handler_t handler = SOFTVECS[type];
  if(handler)
    handler(type, sp);
}
