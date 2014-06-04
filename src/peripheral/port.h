#ifndef _PORT_H_INCLUDED_
#define _PORT_H_INCLUDED_

#include "defines.h"

#define P8DDR (*(volatile uint8*)0xfee007)
#define P2DDR (*(volatile uint8*)0xfee001)
#define P1DDR (*(volatile uint8*)0xfee000)

void set_PORT1_address_output(void);
void set_PORT2_address_output(void);
void set_PORT82_CS_output(void);


#endif
