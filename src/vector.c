#include "defines.h"

extern void
start (void);
extern void
RXI1 (void);
extern void
IMIA0 (void);
extern void
IRQ5 (void);

void
(*vectors[]) (void) =
  {
    start,NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, IRQ5, NULL, NULL, NULL, NULL, NULL, NULL,
    IMIA0,NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, RXI1, NULL, NULL,
    NULL, NULL, NULL, NULL
  };
