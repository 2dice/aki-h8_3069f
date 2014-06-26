#include "defines.h"
#include "port.h"
#include "peripheralInterface.h"

void
set_PORT1_address_output (void)
{
  P1DDR = P1DDR | 0b11111111;
}

void
set_PORT2_address_output (void)
{
  P2DDR = P2DDR | 0b00000111;
}

void
set_PORT82_CS_output (void)
{
  P8DDR = P8DDR | 0b00000100;
}

void
set_PORTA_output (void)
{
  PADDR = PADDR | 0b11111111;
  PADR = PADR & ~0b11111111;
}
