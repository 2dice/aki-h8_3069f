#include "defines.h"
#include "intr.h"
#include "interrupt.h"
#include "lib.h"
#include "command.h"
#include "peripheralInterface.h"

extern int8 intrvec;
#define INTERRUPT_VECTOR_FIRST_ADDRESS (&intrvec)
/* 割込みハンドラ関数ポインタの定義 */
typedef void (*intr_func_t)(int16 interrupt_subscript, uint32 sp);
/* 割り込みベクタの位置 */
#define INTERRUPT_VECTORS ((intr_func_t *)INTERRUPT_VECTOR_FIRST_ADDRESS)


static void RXI1_interrupt(int16 interrupt_subscript, uint32 sp)
    {
      int16 c;
      int16 i;
      static int8 command[32];
      static int16 len;
      
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
        else if(!string_compare_at_arbitrary_length(command, "echo ", 5))
        {
          command_echo(command, 5);
        }
        else
        {
          put_string("unknown.\n");
        }
      /* command[]をクリア */
        for(i=0; i < len; i++)
          command[i]=0;
        put_string("kzload> ");
        len = 0;
      }
    }

/* 割り込みベクタの設定 */
static int16 set_interrupt_vector
(int16 subscript, intr_func_t handler_function)
{
  INTERRUPT_VECTORS[subscript] = handler_function;
  return 0;
}

/* 割り込みベクタの初期化 */
void interrupt_init(void)
{
  int16 clear_subscript;
  for(clear_subscript = 0; clear_subscript < NUMBER_OF_INTERRUPT_VECTORS;
      clear_subscript++)
    set_interrupt_vector(clear_subscript, NULL);

  set_interrupt_vector(RXI1_VECTOR_SUBSCRIPT, RXI1_interrupt);
}

/* 共通割込みハンドラ．割り込みベクタを見て各ハンドラに分岐する */
void interrupt(int16 interrupt_subscript, uint32 sp)
{
  intr_func_t interrupt_function = INTERRUPT_VECTORS[interrupt_subscript];
  if(interrupt_function)
    interrupt_function(interrupt_subscript, sp);
}
