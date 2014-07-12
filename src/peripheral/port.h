#ifndef _PORT_H_INCLUDED_
#define _PORT_H_INCLUDED_

#include "defines.h"

#define PADR  (*(volatile uint8*)0xffffd9)
#define IPRA  (*(volatile uint8*)0xfee018)
#define IER   (*(volatile uint8*)0xfee015)
#define ISCR  (*(volatile uint8*)0xfee014)
#define PADDR (*(volatile uint8*)0xfee009)
#define P8DDR (*(volatile uint8*)0xfee007)
#define P2DDR (*(volatile uint8*)0xfee001)
#define P1DDR (*(volatile uint8*)0xfee000)

void
set_PORT1_address_output (void);
void
set_PORT2_address_output (void);
void
set_PORT82_CS_output (void);
void
set_PORT83_CS_output (void);
void
set_IRQ5_sense_low_level (void);
void
enable_IRQ5 (void);
void
set_PORTA_output (void);

#endif
