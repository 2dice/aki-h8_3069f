#include "defines.h"
#include "serial.h"
#include "lib.h"

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

////////////////////string library////////////////////

int string_length(const char *string_pointer)
{
  int length;

  for (length = 0; *string_pointer; string_pointer++, length++)
    ;

  return length;
}

char *string_copy(char *copy_destination, const char *copy_source)
{
  char *destination = copy_destination;

  for (; ; copy_destination++, copy_source++)
  {
    *copy_destination=*copy_source;
    if (!*copy_source) break;
  }

  return destination;
}

int string_compare(const char *A_pointer, const char *B_pointer)
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

////////////////////serial output library////////////////////

int put_char(unsigned char c)
{
  if (c == '\n')
    serial_send_byte('\r');

  return serial_send_byte(c);
}

int put_string(char str[])
{
  while (*str)
    put_char(*(str++));

  return 0;
}

int put_hex(unsigned long value, int digit_number)
{
  char hex_buffer[9];
  char *hex_pointer;

  hex_pointer = hex_buffer + sizeof(hex_buffer) -1;
  *(hex_pointer--) = '\0';
  while (digit_number)
  {
    *(hex_pointer--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    digit_number--;
  }
  put_string(hex_pointer + 1);

  return 0;
}

int put_dec(unsigned int value)
    {
      char dec_buffer[9];
      char *dec_pointer;
      int value_size = sizeof(value);

      dec_pointer = dec_buffer + sizeof(dec_buffer) -1;
      *(dec_pointer--) = '\0';
      while (value)
        {
          *(dec_pointer--) = "0123456789"[value % 10];
          value /= 10;
          value_size--;
        }
      put_string(dec_pointer + 1);

      return 0;
    }
