#include <am.h>
#include <mmio.h>
#include <klib.h>
#include <la32r-csr.h>

extern char _heap_start;
extern char _pmem_end;
int main(const char *args);
void __am_init_uartlite(void);
void __am_uartlite_putchar(char ch);

_Area _heap = {
  .start = &_heap_start,
  .end = &_pmem_end,
};

void _putc(char ch) {
  __am_uartlite_putchar(ch);
}

void _halt(int code) {
  printf("Exit with code = %d\n", code);
  // set $a0 = code
  __asm__ volatile("add.w $r4, %0, $r0" : :"r"(code));
  // syscall 0x11 is nemu trap
  __asm__ volatile("syscall 0x11");

  // should not reach here during simulation

  // should not reach here on FPGA
  while (1);
}

inline void _cache_init()
{
  // invalid icache & dcache, just need two insts due to hardware implementation
  __asm__ __volatile__("cacop 0, $r0, 0" : : : "memory");
  __asm__ __volatile__("cacop 1, $r0, 0" : : : "memory");
}

// dmw0 map va : 0xa0000000 - 0xbfffffff -> pa : 0xa0000000 - 0xbfffffff (SUC PLV0 PLV3)
// dmw1 map va : 0x0 - 0x1fffffff -> pa : 0x0 - 0x1fffffff (CC PLV0 PLV3)
// note that pa : 1fe001e0 - 1fe001d0 has mmio to uart8250 device, so do not use va : 1fe00000 - 1fe0ffff as CC memory!
inline void _dmw_init()
{
  uint32_t dmw0 = 1 | (1 << DMW_PLV3_OFFSET) | (STRONGLY_ORDERED_UNCACHED << DMW_MAT_OFFSET) | (5 << DMW_PSEG_OFFSET) | (5 << DMW_VSEG_OFFSET);
  uint32_t dmw1 = 1 | (1 << DMW_PLV3_OFFSET) | (COHERENT_CACHED << DMW_MAT_OFFSET) | (0 << DMW_PSEG_OFFSET) | (0 << DMW_VSEG_OFFSET);
  csr_write(csr_dmw0, dmw0);
  csr_write(csr_dmw1, dmw1);

  csr_masked_write(csr_crmd, (0 << CRMD_DA_OFFSET) | (1 << CRMD_PG_OFFSET) | (1 << CRMD_DATF_OFFSET) | (1 << CRMD_DATM_OFFSET), CRMD_DA_MASK | CRMD_PG_MASK | CRMD_DATF_MASK | CRMD_DATM_MASK);
}

void _trm_init() {
  __am_init_uartlite();
  _cache_init();
  _dmw_init();
  extern const char __am_mainargs;
  int ret = main(&__am_mainargs);
  _halt(ret);
}
