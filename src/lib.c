#include "defines.h"
#include "lib.h"
#include "peripheralInterface.h"

////////////////////memory library////////////////////

void *set_data_in_memory(void *first_address, int set_data, long set_length)
{
  char *set_address;

  for (set_address = first_address; set_length > 0; set_length--)
    *(set_address++) =set_data;

  return first_address;
}

void *memory_data_copy
(void *store_first_address, const void *source_first_address, long copy_length)
{
  char *store_address        = store_first_address;
  const char *source_address = source_first_address;

  for (; copy_length > 0; copy_length--)
    *(store_address++) = *(source_address++);

  return store_first_address;
}

int memory_compare
(const void *A_first_address, const void *B_first_address, long compare_length)
    //return  0 : A=B
    //return  1 : A>B(lowest address in different data)
    //return -1 : A<B(lowest address in different data)
{
  const char *A_address = A_first_address,
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

int dump(char *dump_start_address, long size)
{
  long i;

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

int string_length(const unsigned char *string_pointer)
{
  int length;

  for (length = 0; *string_pointer; string_pointer++, length++)
    ;

  return length;
}

unsigned char *string_copy
(unsigned char *copy_destination, const unsigned char *copy_source)
{
  unsigned char *destination = copy_destination;

  for (; ; copy_destination++, copy_source++)
  {
    *copy_destination=*copy_source;
    if (!*copy_source) break;
  }

  return destination;
}

int string_compare
(const char *A_pointer, const char *B_pointer)
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

int string_compare_at_arbitrary_length
(const char *A_pointer, const char *B_pointer, int compare_length)
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

////////////////////other////////////////////
void xmodem_wait()
    {
      volatile long i;
      for(i = 0; i < 300000; i++)
        ;
    }
