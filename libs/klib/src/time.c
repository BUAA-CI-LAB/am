#include <watchdog.h>
#include <driver/confreg.h>

#define CONFIG_WD_PERIOD (10 * 1000 * 1000)	/* 10 seconds default */
#define CONFIG_SYS_TIMER_RATE (1000000)

ulong get_tbclk() {
	return CONFIG_SYS_TIMER_RATE;
}

uint64_t get_ticks() {
    return read_time();
}

uint64_t usec_to_tick(uint64_t usec)
{
	uint64_t tick = usec;
	return tick;
}

void __udelay(uint64_t usec) {
	uint64_t tmp;
	tmp = get_ticks() + usec_to_tick(usec);	/* get current timestamp */
	while (get_ticks() < tmp + 1)	/* loop till event */
		 /*NOP*/;
}

void udelay(uint64_t usec) {
	uint64_t kv;
	do {
		WATCHDOG_RESET();
		kv = usec > CONFIG_WD_PERIOD ? CONFIG_WD_PERIOD : usec;
		__udelay(kv);
		usec -= kv;
	} while(usec);
}