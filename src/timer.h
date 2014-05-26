#ifndef _TIMER_H_INCLUDED_
#define _TIMER_H_INCLUDED_

#include "defines.h"

#define TMR16_GRA0L  (*(volatile uint8*)0xffff6d)
#define TMR16_GRA0H  (*(volatile uint8*)0xffff6c)
#define TMR16_TIOR0  (*(volatile uint8*)0xffff69)
#define TMR16_16TCR0 (*(volatile uint8*)0xffff68)
#define TMR16_TISRA  (*(volatile uint8*)0xffff64)
#define TMR16_TSTR   (*(volatile uint8*)0xffff60)

#define TMR16ch0_COMPARE_MATCH (TMR16_TISRA & 0b00000001)

void disable_TMR16ch0(void);
void disable_TMR16ch0A_interrupt(void);
void set_TMR16ch0_clock_source(void);
void set_TMR16ch0_counter_reset_condition(void);
void set_TIOCA0_pin_function(void);
void set_TMR16ch0A_compare_match_register(void);
void enable_TMR16ch0A_interrept(void);
void enable_TMR16ch0(void);

#endif
