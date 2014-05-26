#ifndef _INTERRUPT_H_INCLUDED_
#define _INTERRUPT_H_INCLUDED_

#include "defines.h"

/* 割込有効化 */
#define INTR_ENABLE  asm volatile ("andc.b #0x3f,ccr")
/* 割込無効化 */
#define INTR_DISABLE asm volatile ("orc.b  #0xc0,ccr")

#endif
