#ifndef _INTERRUPT_H_INCLUDED_
#define _INTERRUPT_H_INCLUDED_

#include "defines.h"

extern int8 softvec;
#define SOFTVEC_ADDR (&softvec)
/* ソフトウェア割り込みベクタの種別を表す方の定義 */
typedef int16 softvec_type_t;
/* 割込みハンドラの方の定義 */
typedef void (*softvec_handler_t)(softvec_type_t type, uint32 sp);
/* ソフトウェア割り込みベクタの位置 */
#define SOFTVECS ((softvec_handler_t *)SOFTVEC_ADDR)
/* 割込有効化 */
#define INTR_ENABLE  asm volatile ("andc.b #0x3f,ccr")
/* 割込無効化 */
#define INTR_DISABLE asm volatile ("orc.b  #0xc0,ccr")

/* ソフトウェア割り込みベクタの初期化 */
int16 softvec_init(void);
/* ソフトウェア割り込みベクタの設定 */
int16 softvec_setintr(softvec_type_t type, softvec_handler_t handler);
/* 共通割込みハンドラ */
void interrupt(softvec_type_t type, uint32 sp);

#endif
