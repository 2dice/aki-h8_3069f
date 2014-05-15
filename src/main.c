#include "defines.h"
#include "serial.h"
#include "xmodem.h"
#include "lib.h"

static int init(void)
{
  extern int data_start_load, data_start, edata, bss_start, ebss;

  memory_data_copy
      (&data_start, &data_start_load, (long)&edata - (long)&data_start);
  set_data_in_memory(&bss_start, 0, (long)&ebss - (long)&bss_start);
  serial_init();

  return 0;
}

static int dump(unsigned char *buf, long size)
{
  long i;

  if(size < 0)
  {
    put_string("no data.\n");
    return -1;
  }
  for(i = 0; i < size; i++)
  {
    put_hex(buf[i],2);
    if((i & 0xf) == 15)
    {
      put_string("\n");
    }
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
  static char buf[16];
  static long size = -1;
  static unsigned char *loadbuf = NULL;
  extern int buffer_start;
  
  init();

  put_string("kzload (kozos boot loader) started.\n");

  while(1)
    {
      put_string("kzload> ");
      get_string(buf);

      if(!string_compare(buf, "load"))
        {
          loadbuf = (char *)(&buffer_start);
          size = xmodem_recv(loadbuf);
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
      else if(!string_compare(buf, "dump"))
        {
          put_string("size: ");
          put_hex(size, 2);
          put_string("\n");
          dump(loadbuf, size);
        }
      else
        {
          put_string("unknown.\n");
        }
    }

  return 0;
}
