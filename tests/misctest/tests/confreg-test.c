#include "trap.h"
#include <mmio.h>
#include <eula.h>

int ret = 0;
volatile int array[1];

#define mmiocheck(cond) \
    do { \
      if (!(cond)) { \
        printf("mmiocheck fail at %s:%d\n", __FILE__, __LINE__); \
        ret = 1; \
      } \
    } while (0)

int main()
{

    printf("begin confreg test!\n");
    uint32_t random = 0;
    uint32_t result = 0;

    for (int i = 0; i < 8; i++) {
        random = rand();
        uintptr_t addr = CONFREG_BASE_PADDR + 0x8000 + 0x10 * i;
        outl(addr, random);
        result = inl(addr);
        mmiocheck(random == result);
    }

    uint32_t timer1 = inl(CONFREG_TIMER_PADDR);
    uint32_t timer2 = inl(CONFREG_TIMER_PADDR);
    mmiocheck(timer1 != timer2);

    outl(CONFREG_IO_SIMU_PADDR, 0x12345678);
    result = inl(CONFREG_IO_SIMU_PADDR);
    mmiocheck(result == 0x56781234);

    outl(CONFREG_OPEN_TRACE_PADDR, 0x80000000);
    result = inl(CONFREG_OPEN_TRACE_PADDR);
    mmiocheck(result == 1);
    outl(CONFREG_OPEN_TRACE_PADDR, 0);
    result = inl(CONFREG_OPEN_TRACE_PADDR);
    mmiocheck(result == 0);

    outl(CONFREG_NUM_MONITOR_PADDR, 0xffffffff);
    result = inl(CONFREG_NUM_MONITOR_PADDR);
    mmiocheck(result == 1);
    outl(CONFREG_NUM_MONITOR_PADDR, 0xfffffffe);
    result = inl(CONFREG_NUM_MONITOR_PADDR);
    mmiocheck(result == 0);

    if (ret) {
      printf("confreg test fail!\n");
      array[0] = 0;
      ret = result / array[0]; // nemu should assert fail
    } else {
      printf("confreg test pass!\n");
    }
    
    return ret;
}