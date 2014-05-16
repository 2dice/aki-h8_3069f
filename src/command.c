#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"
#include "xmodem.h"
#include "command.h"

extern int buffer_start;
static long size = -1;
static char *xmodem_recv_buffer_start_address = (char *)(&buffer_start);

long command_load(void)
{
  size = xmodem_recv(xmodem_recv_buffer_start_address);
  xmodem_wait();
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

int command_dump(void)
{
  put_string("size: ");
  put_hex(size, 2);
  put_string("\n");
  dump(xmodem_recv_buffer_start_address, size);

  return 0;
}
