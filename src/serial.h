#ifndef _SERIAL_H_INCLUDED_
#define _SERIAL_H_INCLUDED_

#include "defines.h"

#define SCI0_SSR (*(volatile uint8*)0xffffbc)
#define SCI0_RDR (*(volatile uint8*)0xffffbd)
#define SCI0_TDR (*(volatile uint8*)0xffffbb)
#define SCI0_SCR (*(volatile uint8*)0xffffba)
#define SCI0_BRR (*(volatile uint8*)0xffffb9)
#define SCI0_SMR (*(volatile uint8*)0xffffb8)

#define SCI0_SENDING (~SCI0_SSR & 0b10000000)
#define SCI0_RECEIVING (~SCI0_SSR & 0b01000000)

int16 serial_init(void); 
int16 serial_send_byte(uint8 c); 
uint8 serial_recv_byte(void);

#endif
