.section entry, "ax"
.globl _start
.type _start, @function

_start:
  la.local $sp, _stack_pointer
  bl _trm_init
  
