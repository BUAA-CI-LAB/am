pmem_base = 0x80000000;

OUTPUT_FORMAT(elf32-loongarch);

MEMORY {
  ram (rwxa) : ORIGIN = 0x80000000, LENGTH = 128M
}

INCLUDE "section.ld"
