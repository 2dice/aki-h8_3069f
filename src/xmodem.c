#include "defines.h"
#include "serial.h"
#include "lib.h"
#include "xmodem.h"

/* ASCII制御文字 */
#define XMODEM_SOH 0x01 /* start of heading */
#define XMODEM_STX 0x02 /* start of text */
#define XMODEM_EOT 0x04 /* end of transmission */
#define XMODEM_ACK 0x06 /* acknowledge */
#define XMODEM_NAK 0x15 /* negative acknowledge */
#define XMODEM_CAN 0x18 /* cancel */
#define XMODEM_EOF 0x1a /* end of file (Ctrl-Z) */

#define XMODEM_BLOCK_SIZE 128

static int xmodem_polling(void)
{
  long cnt = 0;

  while (SCI0_RECEIVING)
  {
    /* ここで受信するとNAKを返してしまい受け取れない */
    /* TODO：割込に変更 */
    if(++cnt >= 2000000)
    {
      cnt = 0;
      serial_send_byte(XMODEM_NAK);
    }
  }
  return 0;
}

static int xmodem_read_block(unsigned char block_number, char *buf)
{
  unsigned char c, block_num, check_sum;
  int i;

  block_num = serial_recv_byte();
  if(block_num != block_number)
    return -1;

  block_num ^= serial_recv_byte();
  if(block_num != 0xff)
    return -1;

  check_sum = 0;
  for(i = 0; i < XMODEM_BLOCK_SIZE; i++)
  {
    c = serial_recv_byte();
    *(buf++) = c;
    check_sum += c;
  }

  check_sum ^= serial_recv_byte();
  if(check_sum)
    return -1;
  return i;
}

long xmodem_recv(char *buf)
{
  int r, receiving = 0;
  long size = 0;
  unsigned char c, block_number = 1;

  while(1)
  {
    if(!receiving)
      xmodem_polling();

    c = serial_recv_byte();

    if(c == XMODEM_EOT)
      {
        serial_send_byte(XMODEM_ACK);
        break;
      }
    else if(c == XMODEM_CAN)
      {
        return -1;
      }
    else if(c == XMODEM_SOH)
      {
        receiving++;
        r = xmodem_read_block(block_number, buf);
        if(r < 0)
          {
            serial_send_byte(XMODEM_NAK);
          }
        else
          {
            block_number++;
            size += r;
            buf += r;
            serial_send_byte(XMODEM_ACK);
          }
      }
    else
      {
        if(receiving)
          return -1;
      }
  }
  return size;
}
  
