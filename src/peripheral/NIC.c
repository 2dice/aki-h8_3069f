#include "defines.h"
#include "peripheralInterface.h"
#include "NIC.h"

static uint8 source_IP_address[4] = {};
static uint8 source_MAC_address[6] = {};

/* ld.scrに移動 */
#define NIC_START_ADDRESS (0x200000)
#define MAC_ADDRESS_SIZE (6)
#define NIC_REMOTE_DMA_DONE (NIC_read(NIC_ISR) & 0x40) 

////////////////////NIC_offset_address////////////////////
#define NIC_CR         0x00
#define NIC_RDMAP      0x10
#define NIC_RESET_PORT 0x18

/* page0 */
#define NIC_PSTART     0x01
#define NIC_PSTOP      0x02
#define NIC_BNRY       0x03
#define NIC_TPSR       0x04
#define NIC_TBCR0      0x05
#define NIC_TBCR1      0x06
#define NIC_ISR        0x07
#define NIC_RSAR0      0x08
#define NIC_RSAR1      0x09
#define NIC_RBCR0      0x0A
#define NIC_RBCR1      0x0B
#define NIC_RCR        0x0C
#define NIC_TCR        0x0D
#define NIC_DCR        0x0E
#define NIC_IMR        0x0F

/* page1 */
#define NIC_PAR0       0x01
#define NIC_PAR1       0x02
#define NIC_PAR2       0x03
#define NIC_PAR3       0x04
#define NIC_PAR4       0x05
#define NIC_PAR5       0x06
#define NIC_CURR       0x07
#define NIC_MAR0       0x08
#define NIC_MAR1       0x09
#define NIC_MAR2       0x0A
#define NIC_MAR3       0x0B
#define NIC_MAR4       0x0C
#define NIC_MAR5       0x0D
#define NIC_MAR6       0x0E
#define NIC_MAR7       0x0F


static uint8
NIC_read (uint8 NIC_offset_address)
{
  uint8 data;
  volatile uint8 *pointer;

  pointer = (volatile unsigned char *)NIC_START_ADDRESS;
  data = pointer[NIC_offset_address];
  return data;
}

static void
NIC_write (uint8 NIC_offset_address, uint8 send_data)
{
  volatile uint8 *pointer;

  pointer = (volatile unsigned char *)NIC_START_ADDRESS;
  pointer[NIC_offset_address] = send_data;
}

void
NIC_soft_reset (void)
{
  uint8 reset_status;

  reset_status = NIC_read (NIC_RESET_PORT);
  NIC_write (NIC_RESET_PORT, reset_status);
  wait_ms (10);
}

void
disable_NIC_set_page0 (void)
{
  NIC_write (NIC_CR, 0x21);
}

void
set_NIC_data_mode(void)
{
  NIC_write (NIC_DCR, 0x4A);
}

void
set_NIC_DMA_data_size (void)
{
  NIC_write (NIC_RBCR0, 0x00);
  NIC_write (NIC_RBCR1, 0x00);
}

void
stop_NIC_packet (void)
{
  NIC_write (NIC_RCR, 0x20);
  NIC_write (NIC_TCR, 0x02);
}

void
set_NIC_TxRx_page(void)
{
  NIC_write (NIC_TPSR, 0x40);
  NIC_write (NIC_PSTART, 0x46);
  NIC_write (NIC_BNRY, 0x46);
  NIC_write (NIC_PSTOP, 0x60);
}

void
disable_NIC_interrupt(void)
{
  disable_NIC_set_page0 ();
  NIC_write (NIC_IMR, 0x00);
}

void
enable_NIC_interrupt(void)
{
  start_NIC_page0 ();
  NIC_write (NIC_IMR, 0x01);
}

void
clear_NIC_interrupt_flag(void)
{
  start_NIC_page0 ();
  NIC_write (NIC_ISR, 0xFF);
}

void
set_NIC_MAC_address_size(void)
{
  NIC_write (NIC_RBCR0, 12);
  NIC_write (NIC_RBCR1, 0);
}

void
set_NIC_MAC_start_address(void)
{
  NIC_write (NIC_RSAR0, 0x00);
  NIC_write (NIC_RSAR1, 0x00);
}

void
start_NIC_page0_remoteDMA(void)
{
  NIC_write (NIC_CR, 0x0A);
}

uint8 *
read_NIC_MAC_address(void)
{
  uint16 i;

  /* read MAC address 2byte cycle loop (little endian) */
  for (i = 0; i < MAC_ADDRESS_SIZE; i += 2)
  {
    source_MAC_address[i + 1] = NIC_read(NIC_RDMAP);
    /* dummy data */
    NIC_read(NIC_RDMAP); 
    source_MAC_address[i] = NIC_read(NIC_RDMAP); 
    /* dummy data */
    NIC_read(NIC_RDMAP); 
  }

  /* wait remote DMA finish */
  while(NIC_REMOTE_DMA_DONE == 0)
    ;

  return source_MAC_address;
}

void
disable_NIC_set_page1 (void)
{
  NIC_write (NIC_CR, 0x61);
}

void
set_NIC_MAC_address (uint8 source_MAC_address[])
{
  NIC_write (NIC_PAR0, source_MAC_address[0]);
  NIC_write (NIC_PAR1, source_MAC_address[1]);
  NIC_write (NIC_PAR2, source_MAC_address[2]);
  NIC_write (NIC_PAR3, source_MAC_address[3]);
  NIC_write (NIC_PAR4, source_MAC_address[4]);
  NIC_write (NIC_PAR5, source_MAC_address[5]);
}

void
set_NIC_receive_data_start_address(void)
{
  NIC_write (NIC_CURR, 0x47);
}

void
set_NIC_multicast_address (void)
{
  NIC_write (NIC_MAR0, 0);
  NIC_write (NIC_MAR1, 0);
  NIC_write (NIC_MAR2, 0);
  NIC_write (NIC_MAR3, 0);
  NIC_write (NIC_MAR4, 0);
  NIC_write (NIC_MAR5, 0);
  NIC_write (NIC_MAR6, 0);
  NIC_write (NIC_MAR7, 0);
}

void
correspond_to_broadcast_packet (void)
{
  NIC_write (NIC_RCR, 0x04);
}

void
start_NIC_page0 (void)
{
  NIC_write (NIC_CR, 0x22);
}

void
set_NIC_normal_send_mode (void)
{
  NIC_write (NIC_TCR, 0x00);
}

uint8 *
set_IP_address ()
{
  source_IP_address[0] = 192;
  source_IP_address[1] = 168;
  source_IP_address[2] = 0;
  source_IP_address[3] = 100;

  return source_IP_address;
}

static void
start_NIC_page1 (void)
{
  NIC_write (NIC_CR, 0x62);
}

uint8
packet_receive(uint8 *packet)
{
  uint16 i;
  uint16 size;
  uint8 size_H, size_L;
  uint8 boundary_page, start_page, current_page;
  uint8 header[4];

  start_NIC_page0 ();
  boundary_page = NIC_read (NIC_BNRY);
  start_NIC_page1 ();
  current_page = NIC_read (NIC_CURR);
  /* no data received */
  if (current_page == boundary_page + 1)
  {
    return 1;
  }
  /* ring buffer address cycled */
  if (current_page < boundary_page)
  {
    /* CURRがリングバッファの先頭に戻っても連続的にアドレスをシフトする */
    current_page += (0x60 - 0x46);
  }
  start_page = boundary_page + 1;
  if (start_page == 0x60)
  {
    /* start_pageがリングバッファの最後のアドレスを超えた場合 */
    start_page = 0x46;
  }
  start_NIC_page0 ();
  NIC_write (NIC_RBCR0, 4);
  NIC_write (NIC_RBCR1, 0);
  NIC_write (NIC_RSAR0, 0x00);
  NIC_write (NIC_RSAR1, start_page);
  start_NIC_page0_remoteDMA();
  for (i = 0; i < 4; i++)
  {
    header[i] = NIC_read(NIC_RDMAP);
  }
  while(NIC_REMOTE_DMA_DONE == 0)
    ;
  start_NIC_page0 ();
  size_L = header[2];
  size_H = header[3];
  size = ((unsigned short)size_H << 8) + (unsigned short)size_L;
  NIC_write(NIC_RBCR0, size_L); /* 受信パケットのサイズ（下位バイト） */
  NIC_write(NIC_RBCR1, size_H); /* 受信パケットのサイズ（上位バイト） */
  NIC_write(NIC_RSAR0, 0x00); /* 受信パケットの先頭アドレス（下位バイト） */
  NIC_write(NIC_RSAR1, start_page); /* 受信パケットの先頭アドレス（上位バイト） */
  NIC_write(NIC_CR, 0x0A); /* リモートDMA読み取りを許可する */
  for (i = 0; i < 4; i++)
  {
    NIC_read(NIC_RDMAP); /* ダミー・データを読み取る */
  }
  for (i = 0; i < (size - 4); i++)
  {
    packet[i] = NIC_read(NIC_RDMAP); /* 受信バッファからパケットのデータを読み取る */
    if (i >= 256) /* パケットのサイズが256byteよりも大きいとき */
    {
      NIC_read(NIC_RDMAP); /* ダミー・データを読み取る */
    }
  }
  while(NIC_REMOTE_DMA_DONE == 0)
    ;
  start_NIC_page0 ();
  boundary_page = current_page - 1;
  if (boundary_page >= 0x60)
  {
    boundary_page -= (0x60 - 0x46);
  }
  NIC_write(NIC_BNRY, boundary_page); 
   
  return 0;
}

uint8
IP_compare(uint8 *IP_a, uint8 *IP_b)
{
  uint16 i;

  for (i = 0; i < 4; i++)
  {
    if (IP_a[i] != IP_b[i])
    {
      return 1;
    }
  }

  return 0; 
}

uint8 *
get_src_IP (void)
{
  return source_IP_address;
}

void packet_send(uint8 *packet, uint16 size)
{
   uint16 i;
   uint8 data;
   uint8 size_H, size_L;

   size_L = (uint8)(size & 0x00FF); /* 送信パケットのサイズ（下位バイト） */
   size_H = (uint8)(size >> 8); /* 送信パケットのサイズ（上位バイト） */

   start_NIC_page0 ();
   NIC_write(NIC_RBCR0, size_L); /* 送信パケットのサイズ（下位バイト） */
   NIC_write(NIC_RBCR1, size_H); /* 送信パケットのサイズ（上位バイト） */
   NIC_write(NIC_RSAR0, 0x00); /* 送信パケットの先頭アドレス（下位バイト） */
   NIC_write(NIC_RSAR1, 0x40); /* 送信パケットの先頭アドレス（上位バイト） */
   NIC_write(NIC_CR, 0x12); /* リモートDMA書き込みを許可する */

   for (i = 0; i < size; i++)
   {
      NIC_write(NIC_RDMAP, packet[i]); /* 送信バッファにパケットのデータを書き込む */
   }
   while(NIC_REMOTE_DMA_DONE == 0)
     ;
   start_NIC_page0 ();
   NIC_write(NIC_TBCR0, size_L); /* 送信パケットのサイズ（下位バイト） */
   NIC_write(NIC_TBCR1, size_H); /* 送信パケットのサイズ（上位バイト） */
   NIC_write(NIC_TPSR, 0x40); /* 送信バッファの開始ページ */
   NIC_write(NIC_CR, 0x26); /* パケットを送信する */
   do
   {
     data = NIC_read(NIC_CR); /* CRを読み取る */
   } while ((data & 0x04) == 0x04); /* パケットの送信が完了するのを待つ */
}

void
ARP_reply (uint8 *packet)
{
  uint16 i;
  ARP_PACKET *arp_packet;

  arp_packet = (ARP_PACKET *)packet; /* packetをARP_PACKET構造体に当てはめる */
  
  for (i = 0; i < 6; i++)
  {
    arp_packet -> eth_dst_MAC[i] = arp_packet -> eth_src_MAC[i];
    arp_packet -> eth_src_MAC[i] = source_MAC_address[i];
    /* 宛先MACアドレスと送信元MACアドレスを入れ替える */
  }
  arp_packet -> arp_operation = 2; /* ARPの動作（ARPリプライ） */
  
  for (i = 0; i < 6; i++)
  {
    arp_packet -> arp_dst_MAC[i] = arp_packet -> arp_src_MAC[i];
    arp_packet -> arp_src_MAC[i] = source_MAC_address[i];
    /* 宛先MACアドレスと送信元MACアドレスを入れ替える */
  }
  for (i = 0; i < 4; i++)
  {
    arp_packet -> arp_dst_IP[i] = arp_packet -> arp_src_IP[i];
    arp_packet -> arp_src_IP[i] = source_IP_address[i];
    /* 宛先IPアドレスと送信元IPアドレスを入れ替える */
  }
  packet_send(packet, 60); /* パケットのサイズを60byteに指定して送信する */
}

uint32 ones_complement_sum(uint8 *data, uint16 offset, uint16 size)
{
   uint16 i;
   uint32 sum;
   
   sum = 0;
   for (i = offset; i < (offset + size); i += 2)
   {
      sum += ((uint32)data[i] << 8) + (uint32)data[i + 1];
      /* data[i]を上位バイト，data[i + 1]を下位バイトとして，2byte単位で合計値を計算する */
      
      sum = (sum & 0xFFFF) + (sum >> 16);
      /* sumが0xFFFFよりも大きいときは，最上位の桁を消去し，あらためて1を加算する */
   }
   
   return sum; /* sumを戻り値として，呼び出し元の関数に戻る */
}

void
ping_reply(uint8 *packet)
{
   uint16 i;
   uint32 sum;
   PING_PACKET *ping_packet;
   
   ping_packet = (PING_PACKET *)packet; /* packetをPING_PACKET構造体に当てはめる */
   
   for (i = 0; i < 6; i++)
   {
      ping_packet -> eth_dst_MAC[i] = ping_packet -> eth_src_MAC[i];
      ping_packet -> eth_src_MAC[i] = source_MAC_address[i];
      /* 宛先MACアドレスと送信元MACアドレスを入れ替える */
   }
   for (i = 0; i < 4; i++)
   {
      ping_packet -> ip_dst_IP[i] = ping_packet -> ip_src_IP[i];
      ping_packet -> ip_src_IP[i] = source_IP_address[i];
      /* 宛先IPアドレスと送信元IPアドレスを入れ替える */
   }
   ping_packet -> ping_type = 0; /* メッセージの種類（pingリプライ） */
   
   /* 送信側におけるIPヘッダのチェックサムの計算 */
   ping_packet -> ip_checksum = 0x0000; /* チェックサムの初期値を0x0000にする */
   sum = ones_complement_sum(packet, 14, 20); /* 1の補数和（IPヘッダ） */
   sum = (~sum) & 0xFFFF; /* 計算結果をNOT演算によって反転する */
   ping_packet -> ip_checksum = (uint16)sum; /* 計算結果をセットする */
   
   /* 送信側におけるpingメッセージのチェックサムの計算 */
   ping_packet -> ping_checksum = 0x0000; /* チェックサムの初期値を0x0000にする */
   sum = ones_complement_sum(packet, 34, 40); /* 1の補数和（pingメッセージ） */
   sum = (~sum) & 0xFFFF; /* 計算結果をNOT演算によって反転する */
   ping_packet -> ping_checksum = (uint16)sum; /* 計算結果をセットする */
   
   packet_send(packet, (14 + 20 + 40)); /* パケットを送信する */
}
