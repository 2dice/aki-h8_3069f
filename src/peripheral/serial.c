#include "defines.h"
#include "serial.h"

void
disable_SCI0_TxRx (void)
{
  SCI0_SCR = SCI0_SCR & ~0b00110000;
}

void
disable_SCI0_serial_interrupt (void)
{
  SCI0_SCR = SCI0_SCR & ~0b11001100;
}

void
set_SCI0_clock_source_and_SCK_port_status (void)
{
  SCI0_SCR = SCI0_SCR & ~0b00000011;
}

void
set_SCI0_serial_modes (void)
{
  SCI0_SMR = SCI0_SMR & ~0b11111111;
}

void
set_SCI0_bitrate (void)
{
  SCI0_BRR = 64;
}

void
enable_SCI0_serial_interrept (void)
{
  SCI0_SCR = SCI0_SCR | 0b01000000;
}

void
enable_SCI0_TxRx (void)
{
  SCI0_SCR = SCI0_SCR | 0b00110000;
}

static void
set_SCI0_sending (void)
{
  SCI0_SSR = SCI0_SSR & ~0b10000000;
}

void
serial_send_byte (uint8 c)
{
  while (SCI0_SENDING)
    ;
  SCI0_TDR = c;
  set_SCI0_sending ();
}

static void
set_SCI0_receiving (void)
{
  SCI0_SSR = SCI0_SSR & ~0b01000000;
}

uint8
serial_recv_byte (void)
{
  uint8 c;

  while (SCI0_RECEIVING)
    ;
  c = SCI0_RDR;
  set_SCI0_receiving ();

  return c;
}
