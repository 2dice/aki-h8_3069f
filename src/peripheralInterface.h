#ifndef _PERIPHERAL_H_INCLUDED_
#define _PERIPHERAL_H_INCLUDED_

int put_char(unsigned char c);
unsigned char get_char(void);
int put_string(char str[]);
int get_string(char *store_array);
int put_hex(unsigned long value, int digit_number);
int put_dec(unsigned int value);

#endif
