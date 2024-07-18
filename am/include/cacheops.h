#ifndef __CACHEOPS_H
#define __CACHEOPS_H

#define Cache_I				0x00
#define Cache_D				0x01
#define Cache_V				0x02
#define Cache_S				0x03

#define INDEX_INVALIDATE		0x08
#define INDEX_WRITEBACK_INV		0x08
#define HIT_INVALIDATE			0x10
#define HIT_WRITEBACK_INV		0x10

#define INDEX_INVALIDATE_I		(Cache_I | INDEX_INVALIDATE)
#define INDEX_WRITEBACK_INV_D	(Cache_D | INDEX_WRITEBACK_INV)
#define HIT_INVALIDATE_I		(Cache_I | HIT_INVALIDATE)
#define HIT_INVALIDATE_D		(Cache_D | HIT_INVALIDATE)
#define HIT_WRITEBACK_INV_D		(Cache_D | HIT_WRITEBACK_INV)

static inline void la32r_cache(int op, const volatile void *addr) {
	__asm__ __volatile__("cacop	%0, %1" : : "i"(op), "R" (*(unsigned char *)(addr)));
}

#endif