#include "defines.h"
#include "timer.h"
#include "peripheralInterface.h"

////////////////////ch0////////////////////
void
disable_TMR16ch0 (void)
{
  TMR16_TSTR = TMR16_TSTR & ~0b00000001;
}

void
disable_TMR16ch0A_interrupt (void)
{
  TMR16_TISRA = TMR16_TISRA & ~0b00010000;
}

void
set_TMR16ch0_clock_source (void)
{
  /* 内部クロック/8 */
  TMR16_16TCR0 = TMR16_16TCR0 | 0b00000011;
  TMR16_16TCR0 = TMR16_16TCR0 & ~0b00000100;
}

void
set_TMR16ch0_counter_reset_condition (void)
{
  TMR16_16TCR0 = TMR16_16TCR0 | 0b00100000;
  TMR16_16TCR0 = TMR16_16TCR0 & ~0b01000000;
}

void
set_TIOCA0_pin_function (void)
{
  /* 出力禁止 */
  TMR16_TIOR0 = TMR16_TIOR0 & ~0b00000111;

}

void
set_TMR16ch0A_compare_match_register (void)
{
  /* 10ms */
  TMR16_GRA0H = 0x61;
  TMR16_GRA0L = 0xa8;
}

void
enable_TMR16ch0A_interrept (void)
{
  TMR16_TISRA = TMR16_TISRA | 0b00010000;
}

void
enable_TMR16ch0 (void)
{
  TMR16_TSTR = TMR16_TSTR | 0b00000001;
}

////////////////////ch1////////////////////
void
disable_TMR16ch1 (void)
{
  TMR16_TSTR = TMR16_TSTR & ~0b00000010;
}

void
disable_TMR16ch1A_interrupt (void)
{
  TMR16_TISRA = TMR16_TISRA & ~0b00100000;
}

void
set_TMR16ch1_clock_source (void)
{
  /* 内部クロック/1 */
  TMR16_16TCR1 = TMR16_16TCR1 & ~0b00000111;
}

void
set_TMR16ch1_counter_reset_condition (void)
{
  TMR16_16TCR1 = TMR16_16TCR1 | 0b00100000;
  TMR16_16TCR1 = TMR16_16TCR1 & ~0b01000000;
}

void
set_TIOCA1_pin_function (void)
{
  /* 出力禁止 */
  TMR16_TIOR1 = TMR16_TIOR1 & ~0b00000111;
}

void
enable_TMR16ch1 (void)
{
  TMR16_TSTR = TMR16_TSTR | 0b00000010;
}

static void
clear_TMR16ch1A (void)
{
  TMR16_TISRA = TMR16_TISRA & ~0b00000010;
}

void
wait_count (void)
{
  while (TMR16CH1_COUNTING)
    ;
  clear_TMR16ch1A ();
}
