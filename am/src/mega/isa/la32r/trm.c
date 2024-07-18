#include <am.h>
#include <mmio.h>
#include <klib.h>
#include <la32r-csr.h>
#include <klib-macros.h>
#include <mega.h>
#include <driver/uart-ns16550.h>
#include <driver/xilinx-intc.h>

extern char _heap_start;
extern char _pmem_end;
int main(const char *args);

_Area _heap = {
  .start = &_heap_start,
  .end = &_pmem_end,
};

void _putc(char ch) {
  ns16550_putc((struct ns16550 *)UART_BASE_PADDR, ch);
  if(ch == '\n') {
    ns16550_putc((struct ns16550 *)UART_BASE_PADDR, '\r');
  }
}

char _getc() {
  return ns16550_getc((struct ns16550 *)UART_BASE_PADDR);
}

void _halt(int code) {
  printf("Exit with code = %d\n", code);

  outl(CONFREG_FINISH_PADDR, code);
  // should not reach here during simulation
  while (1);
}

void _cache_init()
{
  // invalid icache & dcache, just need two insts due to eulacore hardware implementation
  __asm__ __volatile__("cacop 0, $r0, 0" : : : "memory");
  __asm__ __volatile__("cacop 1, $r0, 0" : : : "memory");
}

// dmw0 map va : 0x0 - 0x1fffffff -> pa : 0x0 - 0x1fffffff (SUC PLV0 PLV3)  for device access
// dmw1 map va : 0x80000000 - 0x9fffffff -> pa : 0x0 - 0x1fffffff (CC PLV0 PLV3) for memory access
void _dmw_init()
{
  uint32_t dmw0 = 1 | (1 << DMW_PLV3_OFFSET) | (STRONGLY_ORDERED_UNCACHED << DMW_MAT_OFFSET) | (0 << DMW_PSEG_OFFSET) | (0 << DMW_VSEG_OFFSET);
  uint32_t dmw1 = 1 | (1 << DMW_PLV3_OFFSET) | (COHERENT_CACHED << DMW_MAT_OFFSET) | (0 << DMW_PSEG_OFFSET) | (4 << DMW_VSEG_OFFSET);
  csr_write(csr_dmw0, dmw0);
  csr_write(csr_dmw1, dmw1);

  csr_masked_write(csr_crmd, (0 << CRMD_DA_OFFSET) | (1 << CRMD_PG_OFFSET) | (1 << CRMD_DATF_OFFSET) | (1 << CRMD_DATM_OFFSET), CRMD_DA_MASK | CRMD_PG_MASK | CRMD_DATF_MASK | CRMD_DATM_MASK);
}

void _relocate()
{
  extern char _text_start;
  extern char _bss_end;
  extern char _text_start;

  uint32_t* src_begin = (uint32_t*)0x1c000000; // reset pc
  uint32_t* dst_begin = (uint32_t*)(&_text_start);
  uint32_t src_len_bytes = (&_bss_end) - (&_text_start);
  uint32_t src_len_words = (ROUNDUP(src_len_bytes, 8)) >> 2;
  for (int i = 0; i < src_len_words; i++) {
    *(dst_begin + i) = *(src_begin + i);
  }
  __asm__ __volatile__("ibar 0" : : : "memory");
}

void _excp_dispatch() {
  intc_ack(2);
  printf("excp dispatch\n");
  __asm__ __volatile__("ertn" : : :);
}

void _excp_resp_init() {
  void (*p)() = _excp_dispatch;
  csr_write(csr_eentry, ((int)p) & 0xffffffc0);
  csr_write(csr_ectl, csr_read(csr_ectl) | 0xc);
  csr_write(csr_crmd, csr_read(csr_crmd) | 0x4);
}

void _trm_init() {
  ns16550_init((struct ns16550 *)UART_BASE_PADDR, CONFIG_SYS_NS16550_CLK / 16 / CONFIG_BAUDRATE);
  xilinx_intc_of_init();
  _excp_resp_init();
  extern const char __am_mainargs;
  int ret = main(&__am_mainargs); 
  _halt(ret);
}