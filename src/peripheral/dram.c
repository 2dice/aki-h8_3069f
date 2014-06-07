#include "defines.h"
#include "dram.h"
#include "peripheralInterface.h"

void
set_area2_access_bit (void)
{
  ABWCR = ABWCR | 0b00000100;
}

void
set_DRAM_refresh_cycle (void)
{
  RTCOR = 150;
}

void
set_DRAM_refresh_timer_clock_source (void)
{
  RTMCSR = RTMCSR | 0b00001000;
  RTMCSR = RTMCSR & ~0b00110000;
}

void
set_DRAM_multiplex_controll (void)
{
  DRCRB = DRCRB | 0b10000000;
  DRCRB = DRCRB & ~0b01000000;
}

void
set_DRAM_CAS_pin_function (void)
{
  DRCRB = DRCRB & ~0b00100000;
}

void
set_DRAM_refresh_enable (void)
{
  DRCRB = DRCRB | 0b00010000;
}

void
set_DRAM_precharge_cycle (void)
{
  DRCRB = DRCRB & ~0b00000100;
}

void
set_DRAM_WR_wait_state (void)
{
  DRCRB = DRCRB & ~0b00000010;
}

void
set_DRAM_refresh_wait_state (void)
{
  DRCRB = DRCRB & ~0b00000001;
}

void
set_DRAM_area (void)
{
  DRCRA = DRCRA | 0b00100000;
  DRCRA = DRCRA & ~0b11000000;
}

void
set_DRAM_burst_access (void)
{
  DRCRA = DRCRA & ~0b00001000;
}

void
set_DRAM_self_refresh_mode_in_software_standby (void)
{
  /* このDRAMはセルフリフレッシュ非対応 */
  DRCRA = DRCRA & ~0b00000010;
}

void
set_DRAM_RFSH_pin_function (void)
{
  DRCRA = DRCRA & ~0b00000001;
}

void
DRAM_wait (void)
{
  int16 i;

  for (i = 0; i < 15; i++)
    {
      while (~RTMCSR & 0b10000000)
        ;
      RTMCSR = RTMCSR & ~0b10000000;
    }
}
