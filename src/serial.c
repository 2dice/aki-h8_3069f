#include "defines.h"
#include "serial.h"

#define SCI0_SSR (*(volatile uint8*)0xffffbc)
#define SCI0_TDR (*(volatile uint8*)0xffffbb)
#define SCI0_SCR (*(volatile uint8*)0xffffba)
#define SCI0_BRR (*(volatile uint8*)0xffffb9)
#define SCI0_SMR (*(volatile uint8*)0xffffb8)

#define SCI0_SENDING (~SCI0_SSR & 0b10000000)

int disable_SCI0_TxRx(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b00110000;
      return 0;
    }

int disable_SCI0_serial_interrupt(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b11001100;
      return 0;
    }

int set_SCI0_clock_source_and_SCK_port_status(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b00000011;
      return 0;
    }

int set_SCI0_serial_modes(void)
    {
      SCI0_SMR = SCI0_SMR & ~0b11111111;
      return 0;
    }

int set_SCI0_bitrate(void)
    {
      SCI0_BRR = 64;
      return 0;
    }

int enable_SCI0_serial_interrept(void)
    {
      SCI0_SCR = SCI0_SCR | 0b11000100;
      return 0;
    }

int enable_SCI0_TxRx(void)
    {
      SCI0_SCR = SCI0_SCR | 0b00110000;
      return 0;
    }

int serial_init(void)
{
  disable_SCI0_TxRx();
  disable_SCI0_serial_interrupt();

  set_SCI0_clock_source_and_SCK_port_status();
  set_SCI0_serial_modes();
  set_SCI0_bitrate();

  enable_SCI0_serial_interrept();
  enable_SCI0_TxRx();
  
  return 0;
}

int set_SCI0_sending(void)
    {
      SCI0_SSR = SCI0_SSR & ~0b10000000;
      return 0;
    }

int serial_send_byte(unsigned char c)
{
  while (SCI0_SENDING)
    ;
  SCI0_TDR = c;
  set_SCI0_sending();
  
  return 0;
}
