#ifndef _PERIPHERAL_H_INCLUDED_
#define _PERIPHERAL_H_INCLUDED_

#include "defines.h"

void serial_init(void);
int16 SCI0_receiving(void);
void put_char(uint8 c);
void put_byte_data(uint8 c);
uint8 get_char(void);
uint8 get_byte_data(void);
void put_string(int8 str[]);
int16 get_string(int8 *store_array);
void put_hex(uint32 value, int16 digit_number);
void put_dec(uint16 value);

void timer_init(void);
void clear_TMR16ch0A(void);

#endif
