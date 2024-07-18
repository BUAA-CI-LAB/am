#include "trap.h"

//constrain : 
// ps can only be 12 or 21
// vppn range from 0 to 0x7ffff

struct tlb_struct tlb[TLB_ENTRY_NUM];


bool gen_random_bool()
{
    return rand() & 1;
}

int gen_random_plv()
{
    int p = rand();
    if (p % 2) {
        return 3;
    }
    return 0;
}

int gen_random_mat()
{
    return rand() & 1;
}

int gen_random_tlb_ps()
{
    int p = rand();
    if (p % 2) {
        return 12;
    }
    return 21;
}

// sync hardware tlb state to software tlb array using tlbrd
void tlb_state_sync()
{
    uint32_t tmp;
    for(int i = 0; i < TLB_ENTRY_NUM; i++) {
        csr_masked_write(csr_tlbidx, i , TLBIDX_INDEX_MASK);
        tlbrd();

        struct tlb_struct* entry = &tlb[i];
        tmp = csr_read(csr_tlbidx);
        if (((tmp & TLBIDX_NE_MASK) >> TLBIDX_NE_OFFSET) == 1) {
            entry->hi.E = 0;
            continue;
        }

        entry->hi.E = 1;
        entry->hi.PS = (tmp & TLBIDX_PS_MASK) >> TLBIDX_PS_OFFSET;
        
        tmp = csr_read(csr_tlbehi);
        entry->hi.VPPN = (tmp >> TLBEHI_VPPN_OFFSET);

        tmp = csr_read(csr_tlbelo0);
        entry->lo[0].V = tmp & 1;
        entry->lo[0].D = (tmp & TLBELO_D_MASK) >> TLBELO_D_OFFSET;
        entry->lo[0].PLV = (tmp & TLBELO_PLV_MASK) >> TLBELO_PLV_OFFSET;
        entry->lo[0].MAT = (tmp & TLBELO_MAT_MASK) >> TLBELO_MAT_OFFSET;
        entry->hi.G = (tmp & TLBELO_G_MASK) >> TLBELO_G_OFFSET;
        entry->lo[0].PPN = (tmp & TLBELO_PPN_MASK) >> TLBELO_PPN_OFFSET;

        tmp = csr_read(csr_tlbelo1);
        entry->lo[1].V = tmp & 1;
        entry->lo[1].D = (tmp & TLBELO_D_MASK) >> TLBELO_D_OFFSET;
        entry->lo[1].PLV = (tmp & TLBELO_PLV_MASK) >> TLBELO_PLV_OFFSET;
        entry->lo[1].MAT = (tmp & TLBELO_MAT_MASK) >> TLBELO_MAT_OFFSET;
        entry->hi.G &= (tmp & TLBELO_G_MASK) >> TLBELO_G_OFFSET;
        entry->lo[1].PPN = (tmp & TLBELO_PPN_MASK) >> TLBELO_PPN_OFFSET;

        tmp = csr_read(csr_asid);
        entry->hi.ASID = tmp;
    }
}

// when index > TLB_ENTRY_NUM, use tlbfill, otherwise use tlbwr
void write_tlb(uint32_t index, struct tlb_struct entry) 
{
    csr_write(csr_tlbehi, entry.hi.VPPN << TLBEHI_VPPN_OFFSET);

    csr_masked_write(csr_tlbidx, index, TLBIDX_INDEX_MASK);
    csr_masked_write(csr_tlbidx, entry.hi.PS << TLBIDX_PS_OFFSET, TLBIDX_PS_MASK);
    csr_masked_write(csr_tlbidx, (~entry.hi.E) << TLBIDX_NE_OFFSET, TLBIDX_NE_MASK);

    csr_write(csr_asid, entry.hi.ASID);

    csr_masked_write(csr_tlbelo0, entry.lo[0].V, TLBELO_V_MASK);
    csr_masked_write(csr_tlbelo0, entry.lo[0].D << TLBELO_D_OFFSET, TLBELO_D_MASK);
    csr_masked_write(csr_tlbelo0, entry.lo[0].PLV << TLBELO_PLV_OFFSET, TLBELO_V_MASK);
    csr_masked_write(csr_tlbelo0, entry.lo[0].MAT << TLBELO_MAT_OFFSET, TLBELO_MAT_MASK);
    csr_masked_write(csr_tlbelo0, entry.hi.G << TLBELO_G_OFFSET, TLBELO_G_MASK);
    csr_masked_write(csr_tlbelo0, entry.lo[0].PPN << TLBELO_PPN_OFFSET, TLBELO_PPN_MASK);

    csr_masked_write(csr_tlbelo1, entry.lo[1].V, TLBELO_V_MASK);
    csr_masked_write(csr_tlbelo1, entry.lo[1].D << TLBELO_D_OFFSET, TLBELO_D_MASK);
    csr_masked_write(csr_tlbelo1, entry.lo[1].PLV << TLBELO_PLV_OFFSET, TLBELO_V_MASK);
    csr_masked_write(csr_tlbelo1, entry.lo[1].MAT << TLBELO_MAT_OFFSET, TLBELO_MAT_MASK);
    csr_masked_write(csr_tlbelo1, entry.hi.G << TLBELO_G_OFFSET, TLBELO_G_MASK);
    csr_masked_write(csr_tlbelo1, entry.lo[1].PPN << TLBELO_PPN_OFFSET, TLBELO_PPN_MASK);

    if (index < TLB_ENTRY_NUM) {
        tlbwr();
    } else {
        tlbfill();
    }

}

int tlb_find(uint32_t asid, uint32_t vaddr, uint32_t G)
{
    bool find = false;
    int index = -1;
    asid = asid & ASID_ASID_MASK;

    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        if (tlb[i].hi.E) {
            if (G == 1 || tlb[i].hi.G == 1 || tlb[i].hi.ASID == asid) {
                // TODO : use more narrow condition
                // if (tlb[i].hi.PS == 12 && (vaddr >> 13) == tlb[i].hi.VPPN) {
                //     if (find == true) {
                //         printf("tlb multi hit!\n");
                //         _halt(1);
                //     }
                //     find = true;
                //     index = i;
                // } else if (tlb[i].hi.PS == 21 && (vaddr >> 22) == (tlb[i].hi.VPPN >> 9)) {
                //     if (find == true) {
                //         printf("tlb multi hit!\n");
                //         _halt(i);
                //     }
                //     find = true;
                //     index = i;
                // } 
                if ((vaddr >> 13) == tlb[i].hi.VPPN) {
                    if (find == true) {
                        printf("tlb multi hit!\n");
                        _halt(i);
                    }
                    find = true;
                    index = i;
                }
                else if (tlb[i].hi.PS != 21 && tlb[i].hi.PS != 12) {
                    printf("tlb ps should be 12 or 21!\n");
                    _halt(3);
                }
            }
        }
    }
    return index;
}


struct tlb_struct gen_rand_tlb_entry()
{
    struct tlb_struct entry;
    uint32_t asid = rand();
    uint32_t vaddr = rand();
    entry.hi.G = gen_random_bool();
    while (tlb_find(asid, vaddr, entry.hi.G) != -1) {
        asid = rand();
        vaddr = rand();
        entry.hi.G = gen_random_bool();
    }
    entry.hi.E = gen_random_bool();
    entry.hi.ASID = asid;
    entry.hi.PS = gen_random_tlb_ps();
    entry.hi.VPPN = vaddr >> 13;

    for (int i = 0; i < 2; i++) {
        entry.lo[i].V = gen_random_bool();
        entry.lo[i].D = gen_random_bool();
        entry.lo[i].MAT = gen_random_mat();
        entry.lo[i].PLV = gen_random_plv();
        entry.lo[i].PPN = rand();
    }
    return entry;
}

void gen_rand_tlb()
{
    struct tlb_struct entry;
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        entry = gen_rand_tlb_entry();
        entry.hi.E = 1;
        write_tlb(i, entry);
        tlb_state_sync();
    }
}

int main()
{
    struct tlb_struct entry;
    uint32_t asid, vaddr;

    invtlb(0, 0, 0);

    tlb_state_sync();

    // random tlb read write test use tlbrd, tlbwr, tlbfill
    for (int i = 0 ; i < 1000; i++) {
        entry = gen_rand_tlb_entry();
        write_tlb(i % (TLB_ENTRY_NUM * 2), entry);
        tlb_state_sync();
    }

    // test invtlb
    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        asid = tlb[i].hi.ASID;
        vaddr = tlb[i].hi.VPPN << 13;
        invtlb(6, asid, vaddr);
    }

    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        asid = tlb[i].hi.ASID;
        vaddr = tlb[i].hi.VPPN << 13;
        invtlb(5, asid, vaddr);
    }

    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        asid = tlb[i].hi.ASID;
        invtlb(4, asid, 0);
    }

    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        invtlb(3, 0, 0);
    }

    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        invtlb(2, 0, 0);
    }

    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        invtlb(1, 0, 0);
    }

    gen_rand_tlb();
    for (int i = 0; i < TLB_ENTRY_NUM; i++) {
        invtlb(0, 0, 0);
    }

    // test tlbsrch
    for (int i = 0 ; i < 1000; i++) {
        gen_rand_tlb();
        for (int j = 0 ; j < TLB_ENTRY_NUM; j++) {
            asid = tlb[j].hi.ASID;
            vaddr = tlb[j].hi.VPPN << 13;
            csr_masked_write(csr_asid, asid, ASID_ASID_MASK);
            csr_write(csr_tlbehi, vaddr);
            tlbsrch();
        }
    }

    return 0;
}