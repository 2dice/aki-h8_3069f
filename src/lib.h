#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

#include "defines.h"

void *
set_data_in_memory (void *first_address, int16 set_data, int32 set_length);
void *
memory_data_copy (void *store_first_address, const void *source_first_address,
		  int32 copy_length);
int16
memory_compare (const void *A_first_address, const void *B_first_address,
		int32 compare_length);
int16
dump (int8 *dump_start_address, int32 size);

int16
string_length (const uint8 *string_pointer);
uint8 *
string_copy (uint8 *copy_destination, const uint8 *copy_source);
int16
string_compare (const int8 *A_pointer, const int8 *B_pointer);
int16
string_compare_at_arbitrary_length (const int8 *A_pointer,
                                    const int8 *B_pointer, int16 compare_length);

#endif
