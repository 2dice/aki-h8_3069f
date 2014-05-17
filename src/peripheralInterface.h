#ifndef _PERIPHERAL_H_INCLUDED_
#define _PERIPHERAL_H_INCLUDED_

int SCI0_receiving(void);
int put_char(unsigned char c);
int put_byte_data(unsigned char c);
unsigned char get_char(void);
unsigned char get_byte_data(void);
int put_string(char str[]);
int get_string(char *store_array);
int put_hex(unsigned long value, int digit_number);
int put_dec(unsigned int value);

#endif
