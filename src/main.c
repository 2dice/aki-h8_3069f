#include "defines.h"
#include "serial.h"
#include "xmodem.h"
#include "lib.h"

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

static int dump(char *dump_start_address, long size)
{
  long i;

  if(size < 0)
  {
    put_string("no data.\n");
    return -1;
  }
  for(i = 0; i < size; i++)
  {
    put_hex(dump_start_address[i],2);
    /* 端末表示用の改行 */
    if((i & 0xf) == 15)
    {
      put_string("\n");
    }
    /* 端末表示用に1byte毎に1つ・8byte毎に2つスペースを挿入 */
    else
    {
      if((i & 0xf) == 7)
        put_string(" ");
      put_string(" ");
    }
  }
  put_string("\n");
      
  return 0;
}

static void wait()
    {
      volatile long i;
      for(i = 0; i < 300000; i++)
        ;
    }

int main(void)
{
  static char command[16];
  static long size = -1;
  static char *xmodem_recv_buffer_start_address = NULL;
  extern int buffer_start;
  
  init();

  put_string("kzload (kozos boot loader) started.\n");

  while(1)
    {
      put_string("kzload> ");
      /* if((int)sizeof(command) < get_string(command)) */
      /*   put_string("command too long\n"); */
      get_string(command);
      
      if(!string_compare(command, "load"))
        {
          xmodem_recv_buffer_start_address = (char *)(&buffer_start);
          size = xmodem_recv(xmodem_recv_buffer_start_address);
          wait();
          if(size < 0)
            {
              put_string("\nXMODEM receive error!\n");
            }
          else
            {
              put_string("\nXMODEM receive succeeded.\n");
            }
        }
      else if(!string_compare(command, "dump"))
        {
          put_string("size: ");
          put_hex(size, 2);
          put_string("\n");
          dump(xmodem_recv_buffer_start_address, size);
        }
      else
        {
          put_string("unknown.\n");
        }
    }

  return 0;
}
