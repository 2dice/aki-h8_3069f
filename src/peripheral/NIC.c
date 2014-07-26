#include "defines.h"
#include "peripheralInterface.h"
#include "NIC.h"

/* TODO:typedefでサイズごと定義しキャストしやすくする&LAN.cに移動 */
static uint8 source_IP_address[4] = {};
static uint8 source_MAC_address[6] = {};

/* TODO:ld.scrに移動 */
#define NIC_START_ADDRESS (0x200000)
#define MAC_ADDRESS_SIZE (6)
#define NIC_REMOTE_DMA_DONE (NIC_INTERRUPT_STATUS_REGISTER & 0x40) 

////////////////////NIC_address////////////////////
#define NIC_COMMAND_REGISTER (*(volatile uint8*)(NIC_START_ADDRESS + 0x00))
#define NIC_REMOTE_DMA_PORT  (*(volatile uint8*)(NIC_START_ADDRESS + 0x10))
#define NIC_RESET_PORT       (*(volatile uint8*)(NIC_START_ADDRESS + 0x18))

/* page0 */
#define NIC_PAGE_START_REGISTER            (*(volatile uint8*)(NIC_START_ADDRESS + 0x01))
#define NIC_PAGE_STOP_REGISTER             (*(volatile uint8*)(NIC_START_ADDRESS + 0x02))
#define NIC_BOUNDARY_REGISTER              (*(volatile uint8*)(NIC_START_ADDRESS + 0x03))  /* get_boundary_address */
#define NIC_TRANSMIT_PAGE_START_REGISTER   (*(volatile uint8*)(NIC_START_ADDRESS + 0x04))
#define NIC_TRANSMIT_BYTE_COUNT_REGISTER0  (*(volatile uint8*)(NIC_START_ADDRESS + 0x05))  /* set_transmit_packet_size */
#define NIC_TRANSMIT_BYTE_COUNT_REGISTER1  (*(volatile uint8*)(NIC_START_ADDRESS + 0x06))  /* set_transmit_packet_size */
#define NIC_INTERRUPT_STATUS_REGISTER      (*(volatile uint8*)(NIC_START_ADDRESS + 0x07))
#define NIC_REMOTE_START_ADDRESS_REGISTER0 (*(volatile uint8*)(NIC_START_ADDRESS + 0x08))
#define NIC_REMOTE_START_ADDRESS_REGISTER1 (*(volatile uint8*)(NIC_START_ADDRESS + 0x09))  /* set_remote_DMA_start_address */
#define NIC_REMOTE_BYTE_COUNT_REGISTER0    (*(volatile uint8*)(NIC_START_ADDRESS + 0x0A))
#define NIC_REMOTE_BYTE_COUNT_REGISTER1    (*(volatile uint8*)(NIC_START_ADDRESS + 0x0B))
#define NIC_RECEIVE_CONFIGRATION_REGISTER  (*(volatile uint8*)(NIC_START_ADDRESS + 0x0C))
#define NIC_TRANSMIT_CONFIGRATION_REGISTER (*(volatile uint8*)(NIC_START_ADDRESS + 0x0D))
#define NIC_DATA_CONFIGRATION_REGISTER     (*(volatile uint8*)(NIC_START_ADDRESS + 0x0E))
#define NIC_INTERRUPT_MASK_REGISTER        (*(volatile uint8*)(NIC_START_ADDRESS + 0x0F))

/* page1 */
#define NIC_PHYSICAL_ADDRESS_REGISTER0  (*(volatile uint8*)(NIC_START_ADDRESS + 0x01))
#define NIC_PHYSICAL_ADDRESS_REGISTER1  (*(volatile uint8*)(NIC_START_ADDRESS + 0x02))
#define NIC_PHYSICAL_ADDRESS_REGISTER2  (*(volatile uint8*)(NIC_START_ADDRESS + 0x03))
#define NIC_PHYSICAL_ADDRESS_REGISTER3  (*(volatile uint8*)(NIC_START_ADDRESS + 0x04))
#define NIC_PHYSICAL_ADDRESS_REGISTER4  (*(volatile uint8*)(NIC_START_ADDRESS + 0x05))
#define NIC_PHYSICAL_ADDRESS_REGISTER5  (*(volatile uint8*)(NIC_START_ADDRESS + 0x06))
#define NIC_CURRENT_PAGE_REGISTER       (*(volatile uint8*)(NIC_START_ADDRESS + 0x07))
#define NIC_MULTICAST_ADDRESS_REGISTER0 (*(volatile uint8*)(NIC_START_ADDRESS + 0x08))
#define NIC_MULTICAST_ADDRESS_REGISTER1 (*(volatile uint8*)(NIC_START_ADDRESS + 0x09))
#define NIC_MULTICAST_ADDRESS_REGISTER2 (*(volatile uint8*)(NIC_START_ADDRESS + 0x0A))
#define NIC_MULTICAST_ADDRESS_REGISTER3 (*(volatile uint8*)(NIC_START_ADDRESS + 0x0B))
#define NIC_MULTICAST_ADDRESS_REGISTER4 (*(volatile uint8*)(NIC_START_ADDRESS + 0x0C))
#define NIC_MULTICAST_ADDRESS_REGISTER5 (*(volatile uint8*)(NIC_START_ADDRESS + 0x0D))
#define NIC_MULTICAST_ADDRESS_REGISTER6 (*(volatile uint8*)(NIC_START_ADDRESS + 0x0E))
#define NIC_MULTICAST_ADDRESS_REGISTER7 (*(volatile uint8*)(NIC_START_ADDRESS + 0x0F))

////////////////////internal data R/W functions////////////////////
/* set_NIC_DMA_data_size()を作成 */

/* TODO:関数化set_NIC_remote_DMA_start_address */
/* NIC_REMOTE_START_ADDRESS_REGISTER0 = 0x00; */
/* NIC_REMOTE_START_ADDRESS_REGISTER1 = 0x00; */

////////////////////register setting functions////////////////////
static void
stop_NIC (void)
{
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER & ~0b00001110;
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER | 0b00100001;
}

static void
start_NIC (void)
{
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER & ~0b00000001;
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER | 0b00000010;
}

static void
start_NIC_DMA_read (void)
{
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER & ~0b11110100;
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER | 0b00001000;
}

static void
set_NIC_page0 (void)
{
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER & ~0b11000000;
}

static void
set_NIC_page1 (void)
{
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER | 0b01100000;
  NIC_COMMAND_REGISTER = NIC_COMMAND_REGISTER & ~0b10000000;
}

static void
set_NIC_FIFO_threshold_8byte (void)
{
  NIC_DATA_CONFIGRATION_REGISTER = 0b01000000;
}

/* TODO:TCRとセットで設定がMUST．同一関数内にする */
static void
set_NIC_loopback_mode (void)
{
  NIC_DATA_CONFIGRATION_REGISTER = 0b00001000;
}

static void
set_NIC_byte_order_big_endian (void)
{
  NIC_DATA_CONFIGRATION_REGISTER = 0b00000010;
}

static void
set_NIC_receive_monitor_mode(void)
{
  NIC_RECEIVE_CONFIGRATION_REGISTER = 0x21;
}

static void
set_NIC_transmit_loopback_mode(void)
{
  NIC_TRANSMIT_CONFIGRATION_REGISTER = 0x02;
}

static void
set_NIC_transmit_buffer_start_address (void)
{
  NIC_TRANSMIT_PAGE_START_REGISTER = 0x40;
}

static void
set_NIC_receive_buffer_start_address ()
{
  NIC_PAGE_START_REGISTER = 0x46;
}

static void
reset_NIC_receive_boundary_address ()
{
  NIC_BOUNDARY_REGISTER = 0x46;
}

static void
set_NIC_receive_buffer_end_address ()
{
  NIC_PAGE_STOP_REGISTER = 0x60;
}

////////////////////interface functions////////////////////
void
NIC_soft_reset (void)
{
  uint8 reset_status;

  reset_status = NIC_RESET_PORT;
  NIC_RESET_PORT = reset_status;
  wait_ms (10);
}

void
disable_NIC_set_page0 (void)
{
  stop_NIC ();
  set_NIC_page0 ();
}

void
set_NIC_data_mode(void)
{
  set_NIC_FIFO_threshold_8byte ();
  set_NIC_loopback_mode ();
  set_NIC_byte_order_big_endian ();
}

void
clear_NIC_DMA_data_size (void)
    /* TODO:関数名変更(clear_NIC_DMA_data_size()) */
{
/* TODO:上記に加えて引数を持つ関数set_NIC_DMA_data_size()を作成 */
  NIC_REMOTE_BYTE_COUNT_REGISTER0 = 0x00;
  NIC_REMOTE_BYTE_COUNT_REGISTER1 = 0x00;
}

void
stop_NIC_packet (void)
{
  set_NIC_receive_monitor_mode ();
  set_NIC_transmit_loopback_mode ();
}

void
set_NIC_TxRx_buffer_address(void)
{
  set_NIC_transmit_buffer_start_address ();
  set_NIC_receive_buffer_start_address ();
  reset_NIC_receive_boundary_address ();
  set_NIC_receive_buffer_end_address ();
}

void
disable_NIC_interrupt(void)
{
/* TODO:internal functionsに移動 */
  disable_NIC_set_page0 ();
  NIC_INTERRUPT_MASK_REGISTER = 0x00;
}

void
enable_NIC_interrupt(void)
{
/* TODO:internal functionsに移動 */
  start_NIC_page0 ();
  NIC_INTERRUPT_MASK_REGISTER = 0x01;
}

void
clear_NIC_interrupt_flag(void)
{
  start_NIC_page0 ();
  NIC_INTERRUPT_STATUS_REGISTER = 0xFF;
}

void
set_NIC_MAC_address_size(void)
{
/* TODO:set_NIC_DMA_data_size()を呼ぶように変更 */
/* TODO:MACアドレスサイズをdefineに */
  NIC_REMOTE_BYTE_COUNT_REGISTER0 = 12;
  NIC_REMOTE_BYTE_COUNT_REGISTER1 = 0;
}

void
set_NIC_MAC_start_address(void)
{
  /* TODO:関数化set_NIC_remote_DMA_start_address */
  /* TODO:アドレスをdefineに */
  NIC_REMOTE_START_ADDRESS_REGISTER0 = 0x00;
  NIC_REMOTE_START_ADDRESS_REGISTER1 = 0x00;
}

/* TODO:関数名変更start_NIC_page0_remoteDMA_read(void) */
void
start_NIC_page0_remoteDMA(void)
{
  start_NIC ();
  start_NIC_DMA_read ();
}

uint8 *
read_NIC_MAC_address(void)
{
  uint16 i;
  uint8 dummy;

  /* read MAC address 2byte cycle loop (little endian) */
  for (i = 0; i < MAC_ADDRESS_SIZE; i += 2)
  {
    source_MAC_address[i + 1] = NIC_REMOTE_DMA_PORT;
    /* dummy data */
    dummy = NIC_REMOTE_DMA_PORT; 
    source_MAC_address[i] = NIC_REMOTE_DMA_PORT; 
    /* dummy data */
    dummy = NIC_REMOTE_DMA_PORT; 
  }

  /* wait remote DMA finish */
  while(NIC_REMOTE_DMA_DONE == 0)
    ;

  return source_MAC_address;
}

void
disable_NIC_set_page1 (void)
{
  stop_NIC ();
  set_NIC_page1 ();
}

void
set_NIC_MAC_address (uint8 source_MAC_address[])
{
  NIC_PHYSICAL_ADDRESS_REGISTER0 = source_MAC_address[0];
  NIC_PHYSICAL_ADDRESS_REGISTER1 = source_MAC_address[1];
  NIC_PHYSICAL_ADDRESS_REGISTER2 = source_MAC_address[2];
  NIC_PHYSICAL_ADDRESS_REGISTER3 = source_MAC_address[3];
  NIC_PHYSICAL_ADDRESS_REGISTER4 = source_MAC_address[4];
  NIC_PHYSICAL_ADDRESS_REGISTER5 = source_MAC_address[5];
}

void
set_NIC_receive_data_start_address(void)
{
/* TODO:関数名変更reset_receive_current_address */
  NIC_CURRENT_PAGE_REGISTER = 0x47;
}

void
set_NIC_multicast_address (void)
{
  NIC_MULTICAST_ADDRESS_REGISTER0 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER1 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER2 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER3 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER4 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER5 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER6 = 0;
  NIC_MULTICAST_ADDRESS_REGISTER6 = 0;
}

void
correspond_to_broadcast_packet (void)
{
  NIC_RECEIVE_CONFIGRATION_REGISTER = 0x04;
}

void
start_NIC_page0 (void)
{
  start_NIC ();
  set_NIC_page0 ();
}

void
set_NIC_normal_send_mode (void)
{
  NIC_TRANSMIT_CONFIGRATION_REGISTER = 0x00;
}

uint8 *
set_IP_address ()
{
/* TODO:自動割り当てに変更 */
  source_IP_address[0] = 192;
  source_IP_address[1] = 168;
  source_IP_address[2] = 0;
  source_IP_address[3] = 100;

  return source_IP_address;
}

static void
start_NIC_page1 (void)
{
  start_NIC ();
  set_NIC_page1 ();
}

uint8
packet_receive(uint8 *packet)
{
  uint16 i;
  uint16 size;
  uint8 size_H, size_L;
  uint8 boundary_page, start_page, current_page;
  uint8 header[4];

/* TODO:関数にまとめる(BOUNDARY_REGISTERを読み取る) */
  start_NIC_page0 ();
  boundary_page = NIC_BOUNDARY_REGISTER;
/* TODO:関数にまとめる(CURRENT_PAGE_REGISTERを読み取る) */
  start_NIC_page1 ();
  current_page = NIC_CURRENT_PAGE_REGISTER;
  /* no data received*/
  /* TODO:条件判定を関数orマクロ切り出し*/
  if (current_page == boundary_page + 1)
  {
    return 1;
  }
  /* ring buffer address cycled */
  /* TODO:条件判定を関数orマクロ切り出し*/
  if (current_page < boundary_page)
  {
    /* CURRENT_PAGE_REGISTERがリングバッファの先頭に戻っても連続的にアドレスをシフトする */
    current_page += (0x60 - 0x46);
  }
  start_page = boundary_page + 1;
  /* TODO:条件判定を関数orマクロ切り出し*/
  if (start_page == 0x60)
  {
    /* start_pageがリングバッファの最後のアドレスを超えた場合 */
 /* TODO:関数化reset_start_page() */
    start_page = 0x46;
  }
  start_NIC_page0 ();
/* TODO:関数分離*uint8 read_remote_DMA(start_page,size) */
/* TODO:関数化read_DMA_receive_data_size() */
  NIC_REMOTE_BYTE_COUNT_REGISTER0 = 4;
  NIC_REMOTE_BYTE_COUNT_REGISTER1 = 0;
  NIC_REMOTE_START_ADDRESS_REGISTER0 = 0x00;
  NIC_REMOTE_START_ADDRESS_REGISTER1 = start_page;
  start_NIC_page0_remoteDMA();
  for (i = 0; i < 4; i++)
  {
    header[i] = NIC_REMOTE_DMA_PORT;
  }
  while(NIC_REMOTE_DMA_DONE == 0)
    ;
  start_NIC_page0 ();
  size_L = header[2];
  size_H = header[3];
  size = ((unsigned short)size_H << 8) + (unsigned short)size_L;
  NIC_REMOTE_BYTE_COUNT_REGISTER0 = size_L; /* 受信パケットのサイズ（下位バイト） */
  NIC_REMOTE_BYTE_COUNT_REGISTER1 = size_H; /* 受信パケットのサイズ（上位バイト） */
  NIC_REMOTE_START_ADDRESS_REGISTER0 = 0x00; /* 受信パケットの先頭アドレス（下位バイト） */
  NIC_REMOTE_START_ADDRESS_REGISTER1 = start_page; /* 受信パケットの先頭アドレス（上位バイト） */
  NIC_COMMAND_REGISTER = 0x0A; /* リモートDMA読み取りを許可する */
/* TODO:分けずに連続で格納して頭4byteを無視する作りにする */
  for (i = 0; i < 4; i++)
  {
    NIC_REMOTE_DMA_PORT; /* ダミー・データを読み取る */
  }
  for (i = 0; i < (size - 4); i++)
  {
    packet[i] = NIC_REMOTE_DMA_PORT; /* 受信バッファからパケットのデータを読み取る */
    if (i >= 256) /* パケットのサイズが256byteよりも大きいとき(uint8でデータを扱っているため) */
    {
      NIC_REMOTE_DMA_PORT; /* ダミー・データを読み取る */
    }
  }
  while(NIC_REMOTE_DMA_DONE == 0)
    ;
  start_NIC_page0 ();
/* TODO:関数化inclement_boundary_page() */
  boundary_page = current_page - 1;
  if (boundary_page >= 0x60)
  {
    boundary_page -= (0x60 - 0x46);
  }
  NIC_BOUNDARY_REGISTER = boundary_page; 
   
  return 0;
}

/* TODO:LAN.cに移動 */
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

/* TODO:LAN.cに移動 */
uint8 *
get_src_IP (void)
{
  return source_IP_address;
}

void
packet_send(uint8 *packet, uint16 size)
{
   uint16 i;
   uint8 data;
   uint8 size_H, size_L;

   size_L = (uint8)(size & 0x00FF); /* 送信パケットのサイズ（下位バイト） */
   size_H = (uint8)(size >> 8); /* 送信パケットのサイズ（上位バイト） */

   start_NIC_page0 ();
/* TODO:関数分離*uint8 send_remote_DMA(start_page,size) */
   NIC_REMOTE_BYTE_COUNT_REGISTER0 = size_L; /* 送信パケットのサイズ（下位バイト） */
   NIC_REMOTE_BYTE_COUNT_REGISTER1 = size_H; /* 送信パケットのサイズ（上位バイト） */
   NIC_REMOTE_START_ADDRESS_REGISTER0 = 0x00; /* 送信パケットの先頭アドレス（下位バイト） */
   NIC_REMOTE_START_ADDRESS_REGISTER1 = 0x40; /* 送信パケットの先頭アドレス（上位バイト） */
   NIC_COMMAND_REGISTER = 0x12; /* リモートDMA書込みを許可する */

   for (i = 0; i < size; i++)
   {
      NIC_REMOTE_DMA_PORT = packet[i]; /* 送信バッファにパケットのデータを書き込む */
   }
   while(NIC_REMOTE_DMA_DONE == 0)
     ;
   start_NIC_page0 ();
/* TODO:関数分離*uint8 send_local_DMA(start_page,size) */
   NIC_TRANSMIT_BYTE_COUNT_REGISTER0 = size_L; /* 送信パケットのサイズ（下位バイト） */
   NIC_TRANSMIT_BYTE_COUNT_REGISTER1 = size_H; /* 送信パケットのサイズ（上位バイト） */
   NIC_TRANSMIT_PAGE_START_REGISTER = 0x40; /* 送信バッファの開始ページ */
   NIC_COMMAND_REGISTER = 0x26; /* パケットを送信する */
   do
   {
     data = NIC_COMMAND_REGISTER;
   } while ((data & 0x04) == 0x04); /* パケットの送信が完了するのを待つ */
}

/* TODO:LAN.cに移動 */
void
ARP_reply (uint8 *packet)
{
  uint16 i;
  ARP_PACKET *arp_packet;

  arp_packet = (ARP_PACKET *)packet; /* packetをARP_PACKET構造体に当てはめる */
  
/* TODO:関数化convert_src-dst_MAC_address() */
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
/* TODO:関数化convert_src-dst_IP_address() */
  for (i = 0; i < 4; i++)
  {
    arp_packet -> arp_dst_IP[i] = arp_packet -> arp_src_IP[i];
    arp_packet -> arp_src_IP[i] = source_IP_address[i];
    /* 宛先IPアドレスと送信元IPアドレスを入れ替える */
  }
  packet_send(packet, 60); /* パケットのサイズを60byteに指定して送信する */
}

/* TODO:lib.cに移動? */
uint32
ones_complement_sum(uint8 *data, uint16 offset, uint16 size)
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

/* TODO:LAN.cに移動 */
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
/* TODO:関数化caluculate_IP_checksum() */
   ping_packet -> ip_checksum = 0x0000; /* チェックサムの初期値を0x0000にする */
   sum = ones_complement_sum(packet, 14, 20); /* 1の補数和（IPヘッダ） */
   sum = (~sum) & 0xFFFF; /* 計算結果をNOT演算によって反転する */
   ping_packet -> ip_checksum = (uint16)sum; /* 計算結果をセットする */
   
   /* 送信側におけるpingメッセージのチェックサムの計算 */
/* TODO:関数化caluculate_ping_checksum() */
   ping_packet -> ping_checksum = 0x0000; /* チェックサムの初期値を0x0000にする */
   sum = ones_complement_sum(packet, 34, 40); /* 1の補数和（pingメッセージ） */
   sum = (~sum) & 0xFFFF; /* 計算結果をNOT演算によって反転する */
   ping_packet -> ping_checksum = (uint16)sum; /* 計算結果をセットする */
   
   packet_send(packet, (14 + 20 + 40)); /* パケットを送信する */
}
