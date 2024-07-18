#ifndef __CPU_FUNC_H
#define __CPU_FUNC_H

/* arch/$(ARCH)/lib/cache.c */
void flush_cache(unsigned long addr, unsigned long size);
void flush_dcache_range(unsigned long start, unsigned long stop);
void invalidate_dcache_range(unsigned long start, unsigned long stop);

#endif