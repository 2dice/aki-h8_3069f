#include "defines.h"
#include "peripheralInterface.h"
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

/* serial_send_byteで待ってるのに無いと文字化けする，最適化解除でも変わらず． */
static void xmodem_wait()
    {
      volatile long i;
      for(i = 0; i < 300000; i++)
        ;
    }

static int16 xmodem_polling(void)
{
  int32 i = 0;

  while (SCI0_receiving())
  {
    /* TODO：割込に変更 */
    if(++i >= 2000000)
    {
      i = 0;
      put_byte_data(NAK);
    }
  }
  return 0;
}

static int16 xmodem_read_block(uint8 block_number, int8 *store_address)
{
  uint8 c, recv_block_num, check_sum;
  int16 i;

  /* データブロック番号 */
  recv_block_num = get_byte_data();
  if(recv_block_num != block_number)
    return -1;

  /* データブロック番号の1の補数 */
  recv_block_num ^= get_byte_data();
  if(recv_block_num != 0xff)
    return -1;

  /* データ */
  check_sum = 0;
  for(i = 0; i < BLOCK_SIZE; i++)
  {
    c = get_byte_data();
    *(store_address++) = c;
    check_sum += c;
  }

  /* チェックサム */
  check_sum ^= get_byte_data();
  if(check_sum)
    return -1;
  return i;
}

int32 xmodem_recv(int8 *store_address)
{
  int16 read_block_size, receiving = 0;
  int32 total_read_size = 0;
  uint8 c, block_number = 1;

  while(1)
  {
    if(!receiving)
      xmodem_polling();

    c = get_byte_data();

    /* 転送終了 */
    if(c == EOT)
      {
        put_byte_data(ACK);
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
            put_byte_data(NAK);
          }
        else
          {
            block_number++;
            total_read_size += read_block_size;
            store_address += read_block_size;
            put_byte_data(ACK);
          }
      }
    else
      {
        if(receiving)
          return -1;
      }
  }
  xmodem_wait();

  return total_read_size;
}
  
