#ifndef _DRAM_H_INCLUDED_
#define _DRAM_H_INCLUDED_

#include "defines.h"

#define RTCOR  (*(volatile uint8*)0xfee02a)
#define RTMCSR (*(volatile uint8*)0xfee028)
#define DRCRB  (*(volatile uint8*)0xfee027)
#define DRCRA  (*(volatile uint8*)0xfee026)
#define ABWCR  (*(volatile uint8*)0xfee020)

void
set_area2_access_bit (void);
void
set_DRAM_refresh_cycle (void);
void
set_DRAM_refresh_timer_clock_source (void);
void
set_DRAM_multiplex_controll (void);
void
set_DRAM_CAS_pin_function (void);
void
set_DRAM_refresh_enable (void);
void
set_DRAM_precharge_cycle (void);
void
set_DRAM_WR_wait_state (void);
void
set_DRAM_refresh_wait_state (void);
void
set_DRAM_area (void);
void
set_DRAM_burst_access (void);
void
set_DRAM_self_refresh_mode_in_software_standby (void);
void
set_DRAM_RFSH_pin_function (void);
void
DRAM_wait (void);

#endif
