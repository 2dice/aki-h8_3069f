#include "defines.h"
#include "serial.h"

int16 disable_SCI0_TxRx(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b00110000;
      return 0;
    }

int16 disable_SCI0_serial_interrupt(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b11001100;
      return 0;
    }

int16 set_SCI0_clock_source_and_SCK_port_status(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b00000011;
      return 0;
    }

int16 set_SCI0_serial_modes(void)
    {
      SCI0_SMR = SCI0_SMR & ~0b11111111;
      return 0;
    }

int16 set_SCI0_bitrate(void)
    {
      SCI0_BRR = 64;
      return 0;
    }

int16 enable_SCI0_serial_interrept(void)
    {
      SCI0_SCR = SCI0_SCR | 0b11000100;
      return 0;
    }

int16 enable_SCI0_TxRx(void)
    {
      SCI0_SCR = SCI0_SCR | 0b00110000;
      return 0;
    }

static int16 set_SCI0_sending(void)
    {
      SCI0_SSR = SCI0_SSR & ~0b10000000;
      return 0;
    }

int16 serial_send_byte(uint8 c)
{
  while (SCI0_SENDING)
    ;
  SCI0_TDR = c;
  set_SCI0_sending();
  
  return 0;
}

static int16 set_SCI0_receiving(void)
    {
      SCI0_SSR = SCI0_SSR & ~0b01000000;
      return 0;
    }

uint8 serial_recv_byte(void)
    {
      uint8 c;

      while(SCI0_RECEIVING)
        ;
      c = SCI0_RDR;
      set_SCI0_receiving();

      return c;
    }
