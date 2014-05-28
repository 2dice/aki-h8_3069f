#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"
#include "xmodem.h"
#include "command.h"
#include "elf.h"

extern int16 dram_start;
static int32 size = -1;
static int8 *xmodem_recv_buffer_start_address = (int8 *)(&dram_start);

int32 command_load(void)
{
  size = xmodem_recv(xmodem_recv_buffer_start_address);
  if(size < 0)
  {
    put_string("\nXMODEM receive error!\n");
  }
  else
  {
    put_string("\nXMODEM receive succeeded.\n");
  }
  return size;
}

int16 command_dump(void)
{
  put_string("size: ");
  put_hex(size, 2);
  put_string("\n");
  dump(xmodem_recv_buffer_start_address, size);

  return 0;
}

int16 command_run(void)
{
  int8 *entry_point;
  void (*f)(void);
  
  entry_point = elf_load(xmodem_recv_buffer_start_address);
  if(!entry_point)
    {
      put_string("run error!\n");
    }
  else
    {
      put_string("starting from entry point:");
      put_hex((uint32)entry_point, 6);
      put_string("\n");
      put_string("\n");
      f = (void (*)(void))entry_point;
      f();
    } 
  return 0;
}

void command_echo(int8 command[], int8 offset)
{
  put_string(command + offset);
  put_string("\n");
}
