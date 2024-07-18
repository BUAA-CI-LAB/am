#ifndef _ASM_IO_H
#define _ASM_IO_H

#include <addrspace.h>

static inline unsigned long virt_to_phys(volatile const void *address) {
	unsigned long addr = (unsigned long)address;

	/* this corresponds to kernel implementation of __pa() */
	return CPHYSADDR(addr);
}

#endif