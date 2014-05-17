#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"

////////////////////memory library////////////////////

void *set_data_in_memory(void *first_address, int16 set_data, int32 set_length)
{
  int8 *set_address;

  for (set_address = first_address; set_length > 0; set_length--)
    *(set_address++) =set_data;

  return first_address;
}

void *memory_data_copy
(void *store_first_address, const void *source_first_address, int32 copy_length)
{
  int8 *store_address        = store_first_address;
  const int8 *source_address = source_first_address;

  for (; copy_length > 0; copy_length--)
    *(store_address++) = *(source_address++);

  return store_first_address;
}

int16 memory_compare
(const void *A_first_address, const void *B_first_address, int32 compare_length)
    //return  0 : A=B
    //return  1 : A>B(lowest address in different data)
    //return -1 : A<B(lowest address in different data)
{
  const int8 *A_address = A_first_address,
             *B_address = B_first_address;

  for (; compare_length > 0; compare_length--)
  {
    if(*A_address != *B_address)
      return (*A_address > *B_address) ? 1 : -1;
    A_address++;
    B_address++;
  }

  return 0;
}

int16 dump(int8 *dump_start_address, int32 size)
{
  int32 i;

  if(size < 0)
  {
    put_string("no data.\n");
    return -1;
  }
  for(i = 0; i < size; i++)
  {
    put_hex(dump_start_address[i],2);
    /* 端末表示用の改行 */
    if((i & 0xf) == 15)
    {
      put_string("\n");
    }
    /* 端末表示用に1byte毎に1つ・8byte毎に2つスペースを挿入 */
    else
    {
      if((i & 0xf) == 7)
        put_string(" ");
      put_string(" ");
    }
  }
  put_string("\n");
      
  return 0;
}


////////////////////string library////////////////////

int16 string_length(const uint8 *string_pointer)
{
  int16 length;

  for (length = 0; *string_pointer; string_pointer++, length++)
    ;

  return length;
}

uint8 *string_copy
(uint8 *copy_destination, const uint8 *copy_source)
{
  uint8 *destination = copy_destination;

  for (; ; copy_destination++, copy_source++)
  {
    *copy_destination=*copy_source;
    if (!*copy_source) break;
  }

  return destination;
}

int16 string_compare
(const int8 *A_pointer, const int8 *B_pointer)
    //return  0 : A=B
    //return  1 : A>B(lowest address in different character by character-code)
    //return -1 : A<B(lowest address in different character by character-code)
{
  while (*A_pointer || *B_pointer)
  {
    if (*A_pointer != *B_pointer)
      return (*A_pointer > *B_pointer) ? 1 : -1;
    A_pointer++;
    B_pointer++;
  }

  return 0;
}

int16 string_compare_at_arbitrary_length
(const int8 *A_pointer, const int8 *B_pointer, int16 compare_length)
    //return  0 : A=B
    //return  1 : A>B(lowest address in different character by character-code)
    //return -1 : A<B(lowest address in different character by character-code)
{
  while ((*A_pointer || *B_pointer) && (compare_length > 0))
  {
    if (*A_pointer != *B_pointer)
      return (*A_pointer > *B_pointer) ? 1 : -1;
    A_pointer++;
    B_pointer++;
    compare_length--;
  }

  return 0;
}

