pmem_base = 0x1c000000; /* cpu pc reset to 0x1c000000 */

OUTPUT_FORMAT(elf32-loongarch);

MEMORY {
  ram (rwxa) : ORIGIN = 0x1c000000, LENGTH = 128M
}

INCLUDE "section.ld"
