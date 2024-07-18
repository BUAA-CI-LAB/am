.section .text, "ax"
.globl _excp_start
.globl _excp_end
.type _excp_start, @function

.align 6 // eentry need 2^6 align
_excp_start:
    csrwr       $r12, 0x31      // save r12 to save1
    addi.w	    $r12, $r0, 1
    csrwr	    $r12, 0x44      // clear ticlr
    csrwr       $zero, 0x5      // clear estat
    csrrd       $r12, 0x30      // read save0 to r12
    addi.w      $r12, $r12, 1
    csrwr       $r12, 0x30      // save0 increase 1
    csrrd       $r12, 0x31      // restore r12
    ertn

_excp_end:
