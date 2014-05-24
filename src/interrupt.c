#include "defines.h"
#include "intr.h"
#include "interrupt.h"

/* ソフトウェア割り込みベクタの初期化 */
int16 softvec_init(void)
{
  int16 type;
  for(type = 0; type < SOFTVEC_TYPE_NUM; type++)
    softvec_setintr(type, NULL);
  return 0;
}

/* ソフトウェア割り込みベクタの設定 */
int16 softvec_setintr(softvec_type_t type, softvec_handler_t handler)
{
  SOFTVECS[type] = handler;
  return 0;
}

/* 共通割込みハンドラ．ソフトウェア割り込みベクタを見て各ハンドラに分岐する */
void interrupt(softvec_type_t type, uint32 sp)
{
  softvec_handler_t handler = SOFTVECS[type];
  if(handler)
    handler(type, sp);
}
