#include "defines.h"
#include "serial.h"
#include "lib.h"
#include "xmodem.h"

/* ASCII制御文字 */
#define SOH 0x01 /* start of heading(128byte) */
#define STX 0x02 /* start of text(1024byte) */
#define EOT 0x04 /* end of transmission */
#define ACK 0x06 /* acknowledge */
#define NAK 0x15 /* negative acknowledge */
#define CAN 0x18 /* cancel */
#define EOF 0x1a /* end of file (Ctrl-Z) */

#define BLOCK_SIZE 128

static int xmodem_polling(void)
{
  long i = 0;

  while (SCI0_RECEIVING)
  {
    /* TODO：割込に変更 */
    if(++i >= 2000000)
    {
      i = 0;
      serial_send_byte(NAK);
    }
  }
  return 0;
}

static int xmodem_read_block(unsigned char block_number, char *store_address)
{
  unsigned char c, recv_block_num, check_sum;
  int i;

  /* データブロック番号 */
  recv_block_num = serial_recv_byte();
  if(recv_block_num != block_number)
    return -1;

  /* データブロック番号の1の補数 */
  recv_block_num ^= serial_recv_byte();
  if(recv_block_num != 0xff)
    return -1;

  /* データ */
  check_sum = 0;
  for(i = 0; i < BLOCK_SIZE; i++)
  {
    c = serial_recv_byte();
    *(store_address++) = c;
    check_sum += c;
  }

  /* チェックサム */
  check_sum ^= serial_recv_byte();
  if(check_sum)
    return -1;
  return i;
}

long xmodem_recv(char *store_address)
{
  int read_block_size, receiving = 0;
  long total_read_size = 0;
  unsigned char c, block_number = 1;

  while(1)
  {
    if(!receiving)
      xmodem_polling();

    c = serial_recv_byte();

    /* 転送終了 */
    if(c == EOT)
      {
        serial_send_byte(ACK);
        break;
      }
    /* 転送中断 */
    else if(c == CAN)
      {
        return -1;
      }
    /* 128byteブロックデータのヘッダ */
    else if(c == SOH)
      {
        receiving++;
        read_block_size = xmodem_read_block(block_number, store_address);
        if(read_block_size < 0)
          {
            serial_send_byte(NAK);
          }
        else
          {
            block_number++;
            total_read_size += read_block_size;
            store_address += read_block_size;
            serial_send_byte(ACK);
          }
      }
    else
      {
        if(receiving)
          return -1;
      }
  }
  return total_read_size;
}
  
