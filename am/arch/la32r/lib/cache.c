#include <arch/la32r-mega.h>
#include <arch/la32r/cacheops.h>
#include <mmio.h>

static inline unsigned long icache_line_size(void) {
	return CONFIG_SYS_ICACHE_LINE_SIZE;
}

static inline unsigned long dcache_line_size(void) {
	return CONFIG_SYS_DCACHE_LINE_SIZE;
}

static inline unsigned long scache_line_size(void) {
	return 0;
}


#define cache_loop(start, end, lsize, ops...)                        \
	do                                                               \
	{                                                                \
		const void *addr = (const void *)(start & ~(lsize - 1));     \
		const void *aend = (const void *)((end - 1) & ~(lsize - 1)); \
		const unsigned int cache_ops[] = {ops};                      \
		unsigned int i;                                              \
                                                                     \
		if (!lsize)                                                  \
			break;                                                   \
                                                                     \
		for (; addr <= aend; addr += lsize)                          \
		{                                                            \
			for (i = 0; i < sizeof(cache_ops) / sizeof(cache_ops[0]); i++)              \
				la32r_cache(cache_ops[i], addr);                      \
		}                                                            \
	} while (0)

void flush_cache(ulong start_addr, ulong size) {
	unsigned long ilsize = icache_line_size();
	unsigned long dlsize = dcache_line_size();
	unsigned long slsize = scache_line_size();

	/* aend will be miscalculated when size is zero, so we return here */
	if (size == 0)
		return;

	if ((ilsize == dlsize) && !slsize)
	{
		/* flush I-cache & D-cache simultaneously */
		cache_loop(start_addr, start_addr + size, ilsize,
				   HIT_WRITEBACK_INV_D, HIT_INVALIDATE_I);
		goto ops_done;
	}

	/* flush D-cache */
	cache_loop(start_addr, start_addr + size, dlsize, HIT_WRITEBACK_INV_D);

	// /* flush L2 cache */
	// cache_loop(start_addr, start_addr + size, slsize, HIT_WRITEBACK_INV_SD);

	/* flush I-cache */
	cache_loop(start_addr, start_addr + size, ilsize, HIT_INVALIDATE_I);

ops_done:
	/* ensure cache ops complete before any further memory accesses */
	sync();

	/* ensure the pipeline doesn't contain now-invalid instructions */
	instruction_hazard_barrier();
}

void flush_dcache_range(ulong start_addr, ulong stop) {
	unsigned long lsize = dcache_line_size();
	// unsigned long slsize = scache_line_size();

	/* aend will be miscalculated when size is zero, so we return here */
	if (start_addr == stop)
		return;

	cache_loop(start_addr, stop, lsize, HIT_WRITEBACK_INV_D);

	// /* flush L2 cache */
	// cache_loop(start_addr, stop, slsize, HIT_WRITEBACK_INV_SD);

	/* ensure cache ops complete before any further memory accesses */
	sync();
}

void invalidate_dcache_range(ulong start_addr, ulong stop) {
	unsigned long lsize = dcache_line_size();
	unsigned long slsize = scache_line_size();

	/* aend will be miscalculated when size is zero, so we return here */
	if (start_addr == stop)
		return;

	// /* invalidate L2 cache */
	// cache_loop(start_addr, stop, slsize, HIT_INVALIDATE_SD);

	cache_loop(start_addr, stop, lsize, HIT_INVALIDATE_D);

	/* ensure cache ops complete before any further memory accesses */
	sync();
}