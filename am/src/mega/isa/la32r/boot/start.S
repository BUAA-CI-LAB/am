.section entry, "ax"
.globl _start
.type _start, @function

// start from pc = 0x1c000000
_start:

  // reset cache
  bl _cache_init
  
  // dmw0 map va : 0x0 - 0x1fffffff -> pa : 0x0 - 0x1fffffff (SUC PLV0 PLV3)  for device access
  // dmw1 map va : 0x80000000 - 0x9fffffff -> pa : 0x0 - 0x1fffffff (CC PLV0 PLV3) for memory access
  bl _dmw_init
  
  // set stack pointer to va 0x87000000 (set stack to a higher memory space for _relocate() tempory use)
  ori     $sp, $zero, 0
  lu12i.w $sp, 0x8700
  
  // do relocate, move code and data from spi flash to memory
  bl _relocate

  // jump to memory
  li.w    $t0, 0x80000100
  jirl    $zero, $t0, 0

  .org 0x100

  la.local $sp, _stack_pointer
  bl _trm_init

_excp:
  bl _excp_dispatch
  
