#include "defines.h"
#include "port.h"
#include "peripheralInterface.h"

////////////////////PORT1////////////////////
void
set_PORT1_address_output (void)
{
  P1DDR = P1DDR | 0b11111111;
}

////////////////////PORT2////////////////////
void
set_PORT2_address_output (void)
{
  P2DDR = P2DDR | 0b00000111;
}

////////////////////PORT8////////////////////
void
set_PORT82_CS_output (void)
{
  P8DDR = P8DDR | 0b00000100;
}

void
set_PORT83_CS_output (void)
{
  P8DDR = P8DDR | 0b00001000;
}

////////////////////PORT9////////////////////
void
set_IRQ5_sense_low_level (void)
{
  ISCR = ISCR & ~0b00100000;
}

void
enable_IRQ5 (void)
{
  IER = IER | 0b00100000;
}

////////////////////PORTA////////////////////
void
set_PORTA_output (void)
{
  PADDR = PADDR | 0b11111111;
  PADR = PADR & ~0b11111111;
}
