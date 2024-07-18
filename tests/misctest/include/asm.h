#ifndef _SYS_ASM_H
#define _SYS_ASM_H

#include "regdef.h"
#include "sysdep.h"

/* Macros to handle different pointer/register sizes for 32/64-bit code.  */
# define PTRLOG 2
# define SZREG	4
# define SZFREG	4
# define REG_L ld.w
# define REG_S st.w
# define FREG_L fld.w
# define FREG_S fst.w

#define TLBREBASE 0xf000
#define DATABASE  0x100000

#define WAY    2
#define OFFSET 4
#define INDEX  8

/* Declare leaf routine.  */
#define	LEAF(symbol)			\
	.text;				\
	.globl	symbol;			\
	.align	3;			\
	cfi_startproc ;			\
	.type	symbol, @function;	\
symbol:

# define ENTRY(symbol) LEAF(symbol)

/* Mark end of function.  */
#undef END
#define END(function)			\
	cfi_endproc ;			\
	.size	function,.-function;

/* Stack alignment.  */
#define ALMASK	~15

#endif /* sys/asm.h */
