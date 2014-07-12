#include "defines.h"
#include "serial.h"
#include "timer.h"
#include "dram.h"
#include "port.h"
#include "LCD.h"
#include "NIC.h"
#include "peripheralInterface.h"

////////////////////serial interface////////////////////
void
serial_init (void)
{
  disable_SCI0_TxRx ();
  disable_SCI0_serial_interrupt ();

  set_SCI0_clock_source_and_SCK_port_status ();
  set_SCI0_serial_modes ();
  set_SCI0_bitrate ();

  enable_SCI0_serial_interrept ();
  enable_SCI0_TxRx ();
}

int16
SCI0_receiving (void)
{
  return SCI0_RECEIVING;
}

void
put_char (uint8 c)
{
  if (c == '\n')
    serial_send_byte ('\r');
  serial_send_byte (c);
}

void
put_byte_data (uint8 c)
{
  serial_send_byte (c);
}

uint8
get_char (void)
{
  uint8 c = serial_recv_byte ();
  c = (c == '\r') ? '\n' : c;
  put_char (c);
  return c;
}

uint8
get_byte_data (void)
{
  uint8 c = serial_recv_byte ();
  return c;
}

void
put_string (int8 str[])
{
  while (*str)
    put_char (*(str++));
}

/* TODO：バッファサイズのチェックを追加 */
/* if((int)sizeof(command) < get_string(command)) */
/*   put_string("command too long\n"); */
int16
get_string (int8 *store_array)
{
  int16 i = 0;
  uint8 c;

  do
    {
      c = get_char ();
      if (c == '\n')
        c = '\0';
      store_array[i++] = c;
    }
  while (c);
  return i - 1;
}

void
put_hex (uint32 value, int16 digit_number)
{
  int8 hex_buffer[9];
  int8 *hex_pointer;

  hex_pointer = hex_buffer + sizeof(hex_buffer) - 1;
  *(hex_pointer--) = '\0';
  while (digit_number)
    {
      *(hex_pointer--) = "0123456789abcdef"[value & 0xf];
      value >>= 4;
      digit_number--;
    }
  put_string (hex_pointer + 1);
}

void
put_dec (uint16 value)
{
  int8 dec_buffer[9];
  int8 *dec_pointer;
  int16 value_size = sizeof(value);

  dec_pointer = dec_buffer + sizeof(dec_buffer) - 1;
  *(dec_pointer--) = '\0';
  while (value)
    {
      *(dec_pointer--) = "0123456789"[value % 10];
      value /= 10;
      value_size--;
    }
  put_string (dec_pointer + 1);
}

////////////////////timer interface////////////////////
static void
timer16_0ch_init (void)
{
  disable_TMR16ch0 ();
  disable_TMR16ch0A_interrupt ();

  set_TMR16ch0_clock_source ();
  set_TMR16ch0_counter_reset_condition ();
  set_TIOCA0_pin_function ();
  set_TMR16ch0A_compare_match_register ();

  enable_TMR16ch0A_interrept ();
  enable_TMR16ch0 ();
}

static void
timer16_1ch_init (void)
{
  disable_TMR16ch1 ();
  disable_TMR16ch1A_interrupt ();

  set_TMR16ch1_clock_source ();
  set_TMR16ch1_counter_reset_condition ();
  set_TIOCA1_pin_function ();
}

void
timer_init (void)
{
  timer16_0ch_init ();
  timer16_1ch_init ();
}

void
clear_TMR16ch0A (void)
{
  TMR16_TISRA = TMR16_TISRA & ~0b00000001;
}

void
wait_ms (uint16 time)
{
  int16 i;

  /* 1ms */
  TMR16_GRA1H = 0x4E;
  TMR16_GRA1L = 0x20;
  enable_TMR16ch1 ();

  for (i = 0; i < time; i++)
    {
      wait_count ();
    }
  disable_TMR16ch1 ();

}

void
wait_us (uint16 time)
{
  int16 i;

  /* 1us */
  TMR16_GRA1H = 0x00;
  TMR16_GRA1L = 0x14;
  enable_TMR16ch1 ();

  for (i = 0; i < time; i++)
    {
      wait_count ();
    }
  disable_TMR16ch1 ();
}

////////////////////bus controller interface////////////////////
void
DRAM_init (void)
{
  set_area2_access_bit ();
  set_DRAM_refresh_cycle ();
  set_DRAM_refresh_timer_clock_source ();
  set_DRAM_multiplex_controll ();
  set_DRAM_CAS_pin_function ();
  set_DRAM_refresh_enable ();
  set_DRAM_precharge_cycle ();
  set_DRAM_WR_wait_state ();
  set_DRAM_refresh_wait_state ();
  set_DRAM_area ();
  set_DRAM_burst_access ();
  set_DRAM_self_refresh_mode_in_software_standby ();
  set_DRAM_RFSH_pin_function ();
  DRAM_wait ();

  set_PORT1_address_output ();
  set_PORT2_address_output ();
  set_PORT82_CS_output ();
}

////////////////////LCD interface////////////////////
void
LCD_init (void)
{
  set_PORTA_output ();

  wait_ms (20);
  set_LCD_8bit_mode ();
  wait_ms (5);
  set_LCD_8bit_mode ();
  wait_ms (5);
  set_LCD_8bit_mode ();
  wait_ms (5);
  set_LCD_4bit_mode ();
  wait_ms (5);
  set_LCD_2line_mode_in_4bit_mode ();
  wait_us (50);
  disable_LCD_display ();
  wait_us (50);
  enable_LCD_display ();
  wait_us (50);
  set_LCD_entry_mode_right ();
  wait_us (50);
}

void
send_LCD_E (uint8 bit)
{
  wait_us (1);

  if (bit == 0)
    PADR = PADR & ~0b00000001;
  else
    PADR = PADR | 0b00000001;
}

void
send_LCD_RS (uint8 bit)
{
  if (bit == 0)
    PADR = PADR & ~0b00000010;
  else
    PADR = PADR | 0b00000010;
}

void
send_LCD_data (uint8 data)
{
  PADR = PADR | (data & 0xF0);
  PADR = PADR & (data | 0x0F);
}

void
put_LCD (uint8 *str)
{
  LCD_clear ();
  LCD_display (str);
}

////////////////////NIC interface////////////////////
void
NIC_init (void)
{
  uint8 *source_MAC_address;

  set_PORT1_address_output ();
  set_PORT2_address_output ();
  /* set_IRQ5_sense_low_level(); */
  /* enable_IRQ5() */

  NIC_soft_reset ();

  /* page0 setting (read MAC address) */
  disable_NIC_set_page0 ();
  set_NIC_data_mode();
  set_NIC_DMA_data_size();
  stop_NIC_packet();
  set_NIC_TxRx_page();
  disable_NIC_interrupt();
  clear_NIC_interrupt_flag();

  set_NIC_MAC_address_size();
  set_NIC_MAC_start_address();
  start_NIC_page0_remoteDMA();
  source_MAC_address = read_NIC_MAC_address();

  /* page1 setting (set MAC address) */
  disable_NIC_set_page1 ();
  set_NIC_MAC_address(source_MAC_address);
  set_NIC_receive_data_start_address();
  set_NIC_multicast_address ();

  disable_NIC_set_page0 ();
  correspond_to_broadcast_packet ();
  start_NIC_page0 ();
  set_NIC_normal_send_mode ();
  disable_NIC_interrupt();
}

void
IP_address_init (void)
{
  set_IP_address ();
}

void
read_ARP_packet (void)
{
  uint8 packet[256];
  ARP_PACKET *arp_packet;
  PING_PACKET *ping_packet;

  while (1) /* 無限ループ */
  {
    if ((int16)packet_receive(packet) != 1) /* パケットを受信したとき */
    {
      arp_packet = (ARP_PACKET *)packet; /* packetをARP_PACKET構造体に当てはめる */
      if ((arp_packet -> eth_ethernet_type == 0x0806) && 
          /* ARPのパケットのとき */
          (IP_compare(arp_packet -> arp_dst_IP, get_src_IP()) == 0) && 
          /* パケットに記述されている宛先IPアドレスが送信元IPアドレスに一致したとき */
          (arp_packet -> arp_operation == 1)
          /* ARPリクエストのとき */
          )
      {
        put_string ("arp");
        ARP_reply(packet); /* ARPリプライ */
      }
         
      ping_packet = (PING_PACKET *)packet; /* packetをPING_PACKET構造体に当てはめる */
      if ((ping_packet -> eth_ethernet_type == 0x0800) &&
          /* IPのパケットのとき */
          (IP_compare(ping_packet -> ip_dst_IP, get_src_IP()) == 0) &&
          /* パケットに記述されている宛先IPアドレスが送信元IPアドレスに一致したとき */
          (ping_packet -> ip_protocol == 1) &&
          /* ICMPのパケットのとき */
          (ping_packet -> ping_type == 8)
          /* pingリクエストのとき */
          )
      {
        put_string ("ping");
        ping_reply(packet); /* pingリプライ */
      }
    }
  }
}
