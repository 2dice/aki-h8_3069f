#include "defines.h"
#include "serial.h"
#include "timer.h"
#include "peripheralInterface.h"

////////////////////serial interface////////////////////
void serial_init(void)
{
  disable_SCI0_TxRx();
  disable_SCI0_serial_interrupt();

  set_SCI0_clock_source_and_SCK_port_status();
  set_SCI0_serial_modes();
  set_SCI0_bitrate();

  enable_SCI0_serial_interrept();
  enable_SCI0_TxRx();
}

int16 SCI0_receiving(void)
    {
      return SCI0_RECEIVING;
    }

void put_char(uint8 c)
{
  if (c == '\n')
    serial_send_byte('\r');
  serial_send_byte(c);
}

void put_byte_data(uint8 c)
{
  serial_send_byte(c);
}

uint8 get_char(void)
{
  uint8 c = serial_recv_byte();
  c = (c == '\r') ? '\n' : c;
  put_char(c);
  return c;
}

uint8 get_byte_data(void)
{
  uint8 c = serial_recv_byte();
  return c;
}

void put_string(int8 str[])
{
  while (*str)
    put_char(*(str++));
}

/* if((int)sizeof(command) < get_string(command)) */
/*   put_string("command too long\n"); */
int16 get_string(int8 *store_array)
{
  int16 i = 0;
  uint8 c;

  do{
    c = get_char();
    if(c == '\n')
      c = '\0';
    store_array[i++] = c;
  }while(c);
  return i - 1;
}

void put_hex(uint32 value, int16 digit_number)
{
  int8 hex_buffer[9];
  int8 *hex_pointer;

  hex_pointer = hex_buffer + sizeof(hex_buffer) -1;
  *(hex_pointer--) = '\0';
  while (digit_number)
  {
    *(hex_pointer--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    digit_number--;
  }
  put_string(hex_pointer + 1);
}

void put_dec(uint16 value)
{
  int8 dec_buffer[9];
  int8 *dec_pointer;
  int16 value_size = sizeof(value);

  dec_pointer = dec_buffer + sizeof(dec_buffer) -1;
  *(dec_pointer--) = '\0';
  while (value)
  {
    *(dec_pointer--) = "0123456789"[value % 10];
    value /= 10;
    value_size--;
  }
  put_string(dec_pointer + 1);
}

////////////////////timer interface////////////////////
void timer_init(void)
{
  disable_TMR16ch0();
  disable_TMR16ch0A_interrupt();

  set_TMR16ch0_clock_source();
  set_TMR16ch0_counter_reset_condition();
  set_TIOCA0_pin_function();
  set_TMR16ch0A_compare_match_register();

  enable_TMR16ch0A_interrept();
  enable_TMR16ch0();
}

void clear_TMR16ch0A(void)
{
  while(!TMR16ch0_COMPARE_MATCH)
    ;
  TMR16_TISRA = TMR16_TISRA & ~0b00000001;
}
