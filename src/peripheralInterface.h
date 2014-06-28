#ifndef _PERIPHERAL_H_INCLUDED_
#define _PERIPHERAL_H_INCLUDED_

#include "defines.h"

void
serial_init (void);
int16
SCI0_receiving (void);
void
put_char (uint8 c);
void
put_byte_data (uint8 c);
uint8
get_char (void);
uint8
get_byte_data (void);
void
put_string (int8 str[]);
int16
get_string (int8 *store_array);
void
put_hex (uint32 value, int16 digit_number);
void
put_dec (uint16 value);

void
timer_init (void);
void
clear_TMR16ch0A (void);
void
wait_ms (uint16 time);
void
wait_us (uint16 time);

void
DRAM_init (void);

void
LCD_init (void);
void
send_LCD_E (uint8 bit);
void
send_LCD_RS (uint8 bit);
void
send_LCD_data (uint8 data);
void
put_LCD (uint8 *str);

#endif
