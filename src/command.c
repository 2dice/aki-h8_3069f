#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"
#include "xmodem.h"
#include "command.h"

extern int16 buffer_start;
static int32 size = -1;
static int8 *xmodem_recv_buffer_start_address = (int8 *)(&buffer_start);

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
