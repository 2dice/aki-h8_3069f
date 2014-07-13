#ifndef _NIC_H_INCLUDED_
#define _NIC_H_INCLUDED_

#include "defines.h"

typedef struct
{
   /* イーサネットヘッダ（14byte） */
   unsigned char eth_dst_MAC[6];
   unsigned char eth_src_MAC[6];
   unsigned short eth_ethernet_type;
   
   /* ARPメッセージ（28byte） */
   unsigned short arp_hardware_type;
   unsigned short arp_protocol_type;
   unsigned char arp_hardware_length;
   unsigned char arp_protocol_length;
   unsigned short arp_operation;
   unsigned char arp_src_MAC[6];
   unsigned char arp_src_IP[4];
   unsigned char arp_dst_MAC[6];
   unsigned char arp_dst_IP[4];
} ARP_PACKET;

typedef struct
{
   /* イーサネット・ヘッダ（14byte） */
   unsigned char eth_dst_MAC[6];
   unsigned char eth_src_MAC[6];
   unsigned short eth_ethernet_type;
   
   /* IPヘッダ（20byte） */
   unsigned char ip_version_length;
   unsigned char ip_service_type;
   unsigned short ip_total_length;
   unsigned short ip_id;
   unsigned short ip_flags_fragment_offset;
   unsigned char ip_time_to_live;
   unsigned char ip_protocol;
   unsigned short ip_checksum;
   unsigned char ip_src_IP[4];
   unsigned char ip_dst_IP[4];
   
   /* pingメッセージ（40byte） */
   unsigned char ping_type;
   unsigned char ping_code;
   unsigned short ping_checksum;
   unsigned short ping_id;
   unsigned short ping_sequence_number;
   unsigned char ping_data[32];
} PING_PACKET;

void
NIC_soft_reset (void);
void
disable_NIC_set_page0 (void);
void
set_NIC_data_mode (void);
void
set_NIC_DMA_data_size (void);
void
stop_NIC_packet(void);
void
set_NIC_TxRx_page(void);
void
disable_NIC_interrupt(void);
void
enable_NIC_interrupt(void);
void
clear_NIC_interrupt_flag(void);

void
set_NIC_MAC_address_size(void);
void
set_NIC_MAC_start_address(void);
void
start_NIC_page0_remoteDMA (void);
uint8 *
read_NIC_MAC_address(void);

void
disable_NIC_set_page1 (void);
void
set_NIC_MAC_address(uint8 source_MAC_address[]);
void
set_NIC_receive_data_start_address(void);
void
set_NIC_multicast_address (void);
void
correspond_to_broadcast_packet (void);
void
start_NIC_page0 (void);
void
set_NIC_normal_send_mode (void);
uint8 *
set_IP_address (void);
uint8
packet_receive(uint8 *);
uint8
IP_compare(uint8 *, uint8 *);
uint8 *
get_src_IP (void);
void
ARP_reply (uint8 *);
void
ping_reply(uint8 *);

#endif
