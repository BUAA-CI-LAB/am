#include "trap.h"

#define EENTRY      (0x87000000)
extern char _excp_start, _excp_end;
extern int dhrystone();

int main()
{

    csr_write(csr_eentry, &_excp_start);

    // set save0 to zero, increased when enter interrupt
    csr_write(csr_save0, 0);
    
    // global interrupt enable
    csr_masked_write(csr_crmd, 1 << CRMD_IE_OFFSET, CRMD_IE_MASK);

    // timer & software interrupt enable
    csr_write(csr_ectl, (1 << 11) | 1);

    // config timer
    csr_masked_write(csr_tcfg, 1000 << TCFG_INITVAL_OFFSET, TCFG_INITVAL_MASK);
    csr_masked_write(csr_tcfg, 1 << TCFG_PERIODIC_OFFSET, TCFG_PERIODIC_MASK);
    csr_masked_write(csr_tcfg, 1 , TCFG_EN_MASK);

    dhrystone();

    // disable interrupt
    csr_write(csr_ectl, 0);

    uint32_t intnum = csr_read(csr_save0);

    printf("interrupt num : %d\n", intnum);
    
    return 0;
}
