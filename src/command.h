#ifndef _COMMAND_H_INCLUDED_
#define _COMMAND_H_INCLUDED_

#include "defines.h"

int32
command_load (void);
int16
command_dump (void);
int16
command_run (void);
void
command_echo (int8 command[], int8 offset);

#endif
