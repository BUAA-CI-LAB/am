#include "../include/trap.h"


// test csrrd, csrwr, csrxchg read write func
// NOTE : doesn't test write crmd.plv, crmd.da crmd.pg, estat.is

#define csr_read_write(csr) \
    rdata += csr_read(csr); \
    csr_write(csr, full); \
    rdata += csr_read(csr); \
    csr_masked_write(csr, full, 0); \
    rdata += csr_read(csr);

#define csr_random_read_write(csr)  \
    rdata += csr_read(csr); \
    csr_write(csr, rand()); \
    rdata += csr_read(csr); \
    csr_masked_write(csr, rand(), rand()); \
    rdata += csr_read(csr);


int main()
{

    uint32_t full = 0xffffffff;
    uint32_t rdata = 0xffffffff;

    printf("begin csr-read-write test, need enable csr diff!\n");

    //crmd disable cache
    rdata += csr_read(csr_crmd);
    csr_masked_write(csr_crmd, full, CRMD_IE_MASK);
    csr_masked_write(csr_crmd, 1 << CRMD_DATF_OFFSET, CRMD_DATF_MASK);
    csr_masked_write(csr_crmd, 1 << CRMD_DATM_OFFSET, CRMD_DATM_MASK);
    csr_write(csr_crmd, 1 << CRMD_DA_OFFSET);

    // flush dcache to memory
    __asm__ __volatile__("ibar 0" : : : "memory");

    // prmd
    rdata += csr_read(csr_prmd);
    csr_write(csr_prmd, full);
    rdata += csr_read(csr_prmd);
    csr_write(csr_prmd, 0);
    csr_masked_write(csr_prmd, full, PRMD_PPLV_MASK);
    rdata += csr_read(csr_prmd);
    csr_masked_write(csr_prmd, full, PRMD_PIE_MASK);
    rdata += csr_read(csr_prmd);

    // euen
    rdata += csr_read(csr_euen);
    csr_write(csr_euen, full);
    rdata += csr_read(csr_prmd);
    csr_write(csr_euen, 0);
    rdata += csr_read(csr_prmd);

    // ecfg
    rdata += csr_read(csr_ectl);
    csr_write(csr_ectl, full);
    rdata += csr_read(csr_prmd);
    csr_write(csr_ectl, 0);
    rdata += csr_read(csr_prmd);
    csr_masked_write(csr_ectl, full, ECFG_LIE_MASK);
    rdata += csr_read(csr_prmd);
    csr_write(csr_ectl, 0);
    rdata += csr_read(csr_prmd);

    //era
    csr_read_write(csr_era);

    // badv
    csr_read_write(csr_badv);

    // eentry
    csr_read_write(csr_eentry);

    // cpuid
    csr_read_write(csr_cpuid);

    // save0 - save3
    csr_read_write(csr_save0);
    csr_read_write(csr_save1);
    csr_read_write(csr_save2);
    csr_read_write(csr_save3);

    // llbctl
    csr_read_write(csr_llbctl);

    // tlbidx
    csr_read_write(csr_tlbidx);

    // tlbehi
    csr_read_write(csr_tlbehi);

    // tlbelo
    csr_read_write(csr_tlbelo0);
    csr_read_write(csr_tlbelo1);

    // asid
    csr_read_write(csr_asid);

    // pgd
    csr_read_write(csr_pgdl);
    csr_read_write(csr_pgdh);
    csr_read_write(csr_pgd);

    // tlbreentry
    csr_read_write(csr_tlbrentry);

    // dmw
    csr_read_write(csr_dmw0);
    csr_read_write(csr_dmw1);

    // tid
    csr_read_write(csr_tid);

    csr_read_write(csr_tcfg);

    csr_read_write(csr_tval);

    csr_read_write(csr_ticlr);

    // csr random read write test
    for (int i = 0; i < 1000; i++) {
        csr_random_read_write(csr_prmd);
        csr_random_read_write(csr_euen);
        csr_random_read_write(csr_ectl);
        csr_random_read_write(csr_era);
        csr_random_read_write(csr_badv);
        csr_random_read_write(csr_eentry);
        csr_random_read_write(csr_cpuid);
        csr_random_read_write(csr_save0);
        csr_random_read_write(csr_save1);
        csr_random_read_write(csr_save2);
        csr_random_read_write(csr_save3);
        csr_random_read_write(csr_llbctl);
        csr_random_read_write(csr_tlbidx);
        csr_random_read_write(csr_tlbehi);
        csr_random_read_write(csr_tlbelo0);
        csr_random_read_write(csr_tlbelo1);
        csr_random_read_write(csr_asid);
        csr_random_read_write(csr_pgdl);
        csr_random_read_write(csr_pgdh);
        csr_random_read_write(csr_pgd);
        csr_random_read_write(csr_tlbrentry);
        csr_random_read_write(csr_dmw0);
        csr_random_read_write(csr_dmw1);
        csr_random_read_write(csr_tid);
        csr_random_read_write(csr_tcfg);
        csr_random_read_write(csr_tval);
        csr_random_read_write(csr_ticlr);
    }


    printf("csr-read-write pass!!!\n");

    return 0;
}