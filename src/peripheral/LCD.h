#ifndef _LCD_H_INCLUDED_
#define _LCD_H_INCLUDED_

#include "defines.h"

void
set_LCD_8bit_mode (void);
void
set_LCD_8bit_mode (void);
void
set_LCD_8bit_mode (void);
void
set_LCD_4bit_mode (void);
void
set_LCD_2line_mode_in_4bit_mode (void);
void
disable_LCD_display (void);
void
enable_LCD_display (void);
void
set_LCD_entry_mode_right (void);
void
LCD_clear (void);
void
LCD_display (uint8 *str);

#endif
