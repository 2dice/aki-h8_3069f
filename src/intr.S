#include "intr.h"
	.h8300h
	.section .text

.MACRO	intr_reg_save
	mov.l er6,@-er7
	mov.l er5,@-er7
	mov.l er4,@-er7
	mov.l er3,@-er7
	mov.l er2,@-er7
	mov.l er1,@-er7
	mov.l er0,@-er7
.ENDM
	
.MACRO	intr_reg_restore
	mov.l @er7+,er0
	mov.l @er7+,er1
	mov.l @er7+,er2
	mov.l @er7+,er3
	mov.l @er7+,er4
	mov.l @er7+,er5
	mov.l @er7+,er6
.ENDM
	
	.global _intr_softerr
	.type   _intr_softerr,@function
_intr_softerr:
	intr_reg_save
	mov.l er7,er1
	mov.w #SOFTVEC_TYPE_SOFTERR,r0
	jsr   @_interrupt
	intr_reg_restore
	rte

	.global _intr_syscall
	.type   _intr_syscall,@function
_intr_syscall:
	intr_reg_save
	mov.l er7,er1
	mov.w #SOFTVEC_TYPE_SYSCALL,r0
	jsr   @_interrupt
	intr_reg_restore
	rte

	.global _RXI1
	.type   _RXI1,@function
_RXI1:
	intr_reg_save
	mov.l er7,er1
	mov.w #SOFTVEC_TYPE_SERINTR,r0
	jsr   @_interrupt
	intr_reg_restore
	rte
