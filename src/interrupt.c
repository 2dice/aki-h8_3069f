#include "defines.h"
#include "interrupt.h"
#include "lib.h"
#include "command.h"
#include "peripheralInterface.h"

#pragma interrupt
void RXI1(void)
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

#pragma interrupt
void IMIA0(void)
{
  static int16 timer_count = 0;
  
  clear_TMR16ch0A();
  timer_count++;

  if(timer_count >= 100)
  {
    put_string("1sec\n");
    timer_count = 0;
  }
}
