/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 1996, 99 Ralf Baechle
 * Copyright (C) 2000, 2002  Maciej W. Rozycki
 * Copyright (C) 1990, 1999 by Silicon Graphics, Inc.
 */
#ifndef _ASM_ADDRSPACE_H
#define _ASM_ADDRSPACE_H

/*
 * Returns the kernel segment base of a given address
 */
#define KSEGX(a)		((a) & 0xe0000000)

/*
 * Returns the physical address of a CKSEGx / XKPHYS address
 */
#define CPHYSADDR(a)		((a) & 0x1fffffff)


#define CKSEG0ADDR(a)		(CPHYSADDR(a) | KSEG0)
#define CKSEG1ADDR(a)		(CPHYSADDR(a) | KSEG1)
#define CKSEG2ADDR(a)		(CPHYSADDR(a) | KSEG2)
#define CKSEG3ADDR(a)		(CPHYSADDR(a) | KSEG3)

/*
 * Map an address to a certain kernel segment
 */
#define KSEG0ADDR(a)		(CPHYSADDR(a) | KSEG0)
#define KSEG1ADDR(a)		(CPHYSADDR(a) | KSEG1)
#define KSEG2ADDR(a)		(CPHYSADDR(a) | KSEG2)
#define KSEG3ADDR(a)		(CPHYSADDR(a) | KSEG3)

/*
 * Memory segments (32bit kernel mode addresses)
 * These are the traditional names used in the 32-bit universe.
 */
#define KUSEG			0x00000000
#define KSEG0			0xa0000000
#define KSEG1			0x80000000
#define KSEG2			0xc0000000
#define KSEG3			0xe0000000

#define CKUSEG			0x00000000
#define CKSEG0			0xa0000000
#define CKSEG1			0x80000000
#define CKSEG2			0xc0000000
#define CKSEG3			0xe0000000

#endif /* _ASM_ADDRSPACE_H */
