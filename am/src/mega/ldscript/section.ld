ENTRY(_start)

SECTIONS {
  . = ORIGIN(ram);
  _text_start = .;
  .text : {
    *(entry)
    *(.text)
  }
  . = ALIGN(0x40);
  .etext : {
    _excp = .;
  }
  .rodata : {
    *(.rodata*)
  }
  .data : {
    *(.data)
  }
  edata = .;
  _data = .;
  .bss : {
	_bss_start = .;
    *(.bss*)
    *(.sbss*)
    *(.scommon)
  }
  _bss_end = .;
  _stack_top = ALIGN(0x1000);
  . = _stack_top + 0x8000;
  _stack_pointer = .;
  end = .;
  _end = .;
  _heap_start = ALIGN(0x1000);
  _pmem_start = pmem_base;
  _pmem_end = _pmem_start + LENGTH(ram);
}
