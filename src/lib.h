#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

void *set_data_in_memory(void *first_address, int set_data, long set_length);
void *memory_data_copy
(void *store_first_address, const void *source_first_address, long copy_length);
int memory_compare
(const void *A_first_address, const void *B_first_address, long compare_length);

int string_length(const unsigned char *string_pointer);
unsigned char *string_copy
(unsigned char *copy_destination, const unsigned char *copy_source);
int string_compare
(const unsigned char *A_pointer, const unsigned char *B_pointer);
int string_compare_at_arbitrary_length
(const unsigned char *A_pointer, const unsigned char *B_pointer, int compare_length);

int put_char(unsigned char c);
unsigned char get_char(void);
int put_string(unsigned char str[]);
int get_string(unsigned char *buf);
int put_hex(unsigned long value, int digit_number);
int put_dec(unsigned int value);

#endif
