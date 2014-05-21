#ifndef _PERIPHERAL_H_INCLUDED_
#define _PERIPHERAL_H_INCLUDED_

#include "defines.h"

int16 serial_init(void);
int16 SCI0_receiving(void);
int16 put_char(uint8 c);
int16 put_byte_data(uint8 c);
uint8 get_char(void);
uint8 get_byte_data(void);
int16 put_string(int8 str[]);
int16 get_string(int8 *store_array);
int16 put_hex(uint32 value, int16 digit_number);
int16 put_dec(uint16 value);

#endif
