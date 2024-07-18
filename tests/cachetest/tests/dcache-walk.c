#include "../include/trap.h"
#include <mmio.h>

// default cache size is 1 kbytes, if change cache size, need change TAG_OFFSET
#define CACHE_SIZE 1 // kbytes
#define WAYS 4
#define LINESIZE 32
#define SETS (CACHE_SIZE * 1024 / LINESIZE / WAYS) /* sets = 8 */

#define SET_OFFSET 5
#define TAG_OFFSET 8

#define MEM_BOUND (64 * 1024)
#define TAG_RANGE (MEM_BOUND / (1 << TAG_OFFSET))

unsigned int g_seed;

int fast_rand(void) {
  g_seed = (214013*g_seed+2531011);
  return g_seed;
}

uint32_t accum;

void write()
{
    uintptr_t addr;
    for (int tag = 0; tag < TAG_RANGE; tag++) {
        for (int set = 0; set < SETS; set++) {
            for (int byte = 0; byte < LINESIZE; byte++) {
                addr = (tag << TAG_OFFSET) + (set << SET_OFFSET) + byte;
                outb(addr, fast_rand());
            }
        }
    }
}

void read()
{
    uintptr_t addr;
    // read and accumulate data
    for (int tag = 0; tag < TAG_RANGE; tag++) {
        for (int set = 0; set < SETS; set++) {
            for (int byte = 0; byte < LINESIZE; byte++) {
                addr = (tag << TAG_OFFSET) + (set << SET_OFFSET) + byte;
                accum += inb(addr);
            }
        }
    }
}

int main()
{

    // torment the dcache
    write();

    read();

    for (int i = 0; i < 5; i++) {
        __asm__ __volatile__(" ibar 0 " : : : "memory");

        read();
    }

    for (int i = 0; i < 5; i++) {
        __asm__ __volatile__(" cacop 9, %0, 0 " : "=r"(i) : : "memory");

        read();
    }

    return 0;
}