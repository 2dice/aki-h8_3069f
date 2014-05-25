#include "defines.h"

extern void start(void);
extern void intr_softerr(void);
extern void intr_syscall(void);
extern void RXI1(void);

void (*vectors[])(void) = {
  start, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
  intr_syscall, intr_softerr, intr_softerr, intr_softerr, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, RXI1, NULL, NULL,
  NULL, NULL, NULL, NULL
};
