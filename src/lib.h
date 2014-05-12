#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

void *set_data_in_memory(void *first_address, int set_data, long set_length);
void *memory_data_copy
(void *store_first_address, const void *source_first_address, long copy_length);
int memory_compare
(const void *A_first_address, const void *B_first_address, long compare_length);

int string_length(const char *string_pointer);
char *string_copy(char *copy_destination, const char *copy_source);
int string_compare(const char *A_pointer, const char *B_pointer);
int string_compare_at_arbitrary_length
(const char *A_pointer, const char *B_pointer, int compare_length);

int put_char(unsigned char c);    
int put_string(unsigned char *str); 
int put_hex(unsigned long value, int digit_number);
int put_dec(unsigned int value);

#endif
