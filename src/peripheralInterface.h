#ifndef _PERIPHERAL_H_INCLUDED_
#define _PERIPHERAL_H_INCLUDED_

#include "defines.h"

void
serial_init (void);
int16
SCI0_receiving (void);
void
put_char (uint8);
void
put_byte_data (uint8);
uint8
get_char (void);
uint8
get_byte_data (void);
void
put_string (int8 *);
int16
get_string (int8 *);
void
put_hex (uint32, int16);
void
put_dec (uint16);

void
timer_init (void);
void
clear_TMR16ch0A (void);
void
wait_ms (uint16);
void
wait_us (uint16);

void
DRAM_init (void);

void
LCD_init (void);
void
send_LCD_E (uint8);
void
send_LCD_RS (uint8);
void
send_LCD_data (uint8);
void
put_LCD (uint8 *);

void
NIC_init (void);
void
IP_address_init (void);
void
read_ARP_packet (void);

#endif
