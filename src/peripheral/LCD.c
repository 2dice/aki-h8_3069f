#include "defines.h"
#include "peripheralInterface.h"
#include "LCD.h"

#define RESET_CURSOR 0x80
#define SET_8BIT_MODE 0x30
#define SET_2LINE_MODE_IN_4BIT_MODE 0x28
#define SET_4BIT_MODE 0x20
#define ENABLE_LCD 0x0C
#define DISABLE_LCD 0x08
#define SET_LCD_ENTRY_RIGHT 0x06
#define CLEAR_LCD 0x01

static void
send_LCD_command(uint8 command)
{
  send_LCD_E(0);
  send_LCD_data(command);
  send_LCD_RS(0);
  send_LCD_E(1);
  send_LCD_E(0);
}

static void
send_LCD_char(uint8 chr)
{
  send_LCD_E(0);
  send_LCD_data(chr);
  send_LCD_RS(1);
  send_LCD_E(1);
  send_LCD_E(0);
}

void
set_LCD_8bit_mode(void)
{
  send_LCD_command(SET_8BIT_MODE);
}

void
set_LCD_4bit_mode(void)
{
  send_LCD_command(SET_4BIT_MODE);
}

void
set_LCD_2line_mode_in_4bit_mode(void)
{
  send_LCD_command(SET_2LINE_MODE_IN_4BIT_MODE);
  send_LCD_command((0x0f & SET_2LINE_MODE_IN_4BIT_MODE) << 4);
}

void
disable_LCD_display(void)
{
  send_LCD_command(DISABLE_LCD);
  send_LCD_command((0x0f & DISABLE_LCD) << 4);
}

void
enable_LCD_display(void)
{
  send_LCD_command(ENABLE_LCD);
  send_LCD_command((0x0f & ENABLE_LCD) << 4);
}

void
set_LCD_entry_mode_right(void)
{
  send_LCD_command(SET_LCD_ENTRY_RIGHT);
  send_LCD_command((0x0f & SET_LCD_ENTRY_RIGHT) << 4);
}

void
LCD_clear(void)
{
  send_LCD_command(CLEAR_LCD);
  send_LCD_command((0x0f & CLEAR_LCD) << 4);
  wait_ms(2);
  send_LCD_command(RESET_CURSOR);
  send_LCD_command((0x0f & RESET_CURSOR) << 4);
}

void
LCD_display(uint8 *str)
{
  while (*str != 0x00) {
    send_LCD_char (*str);
    send_LCD_char (*str << 4);
    wait_us(500);
    str++;
  }
}
