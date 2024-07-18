#ifndef __LA32RCSR_H__
#define __LA32RCSR_H__

// csr addr define
// copy from regdef.h
#define csr_crmd      0x0
#define csr_prmd      0x1
#define csr_euen      0x2
#define csr_ectl      0x4 // ecfg
#define csr_estat     0x5
#define csr_era       0x6
#define csr_badv      0x7
#define csr_eentry     0xc
#define csr_tlbidx    0x10
#define csr_tlbehi    0x11
#define csr_tlbelo0    0x12
#define csr_tlbelo1    0x13
#define csr_asid      0x18
#define csr_pgdl      0x19
#define csr_pgdh      0x1a
#define csr_pgd       0x1b
#define csr_cpuid    0x20
#define csr_save0     0x30
#define csr_save1     0x31
#define csr_save2     0x32
#define csr_save3     0x33
#define csr_tid       0x40
#define csr_tcfg      0x41
#define csr_tval      0x42
#define csr_ticlr     0x44
#define csr_llbctl    0x60
#define csr_tlbrentry 0x88
#define csr_dmw0      0x180
#define csr_dmw1      0x181

// csr mask define
#define CRMD_PLV_MASK       0x3
#define CRMD_IE_MASK        0x4
#define CRMD_DA_MASK        0x8
#define CRMD_PG_MASK        0x10
#define CRMD_DATF_MASK      0x60
#define CRMD_DATM_MASK      0x180

#define PRMD_PPLV_MASK      0x3
#define PRMD_PIE_MASK       0x4

#define ECFG_LIE_MASK       0x1bff

#define ESTAT_SWI_MASK      0x3
#define ESTAT_HWI_MASK      0x3fc
#define ESTAT_TI_MASK       0x800
#define ESTAT_IPI_MASK      0x1000
#define ESTAT_ECODE_MASK    0x3f0000
#define ESTAT_ESUBCODE_MASK 0x7fc00000

#define LLBCTL_ROLLB_MASK   0x1
#define LLBCTL_WCLLB_MASK   0x2
#define LLBCTL_KLO_MASK     0x4

#define TLBIDX_INDEX_MASK   0xffff
#define TLBIDX_PS_MASK      0x3f000000
#define TLBIDX_NE_MASK      0x80000000

#define TLBELO_V_MASK       0x1
#define TLBELO_D_MASK       0x2
#define TLBELO_PLV_MASK     0xc
#define TLBELO_MAT_MASK     0x30
#define TLBELO_G_MASK       0x40
#define TLBELO_PPN_MASK     0xfffff00

#define ASID_ASID_MASK      0x3ff

#define DMW_PLV0_MASK       0x1
#define DMW_PLV3_MASK       0x8
#define DMW_MAT_MASK        0x30
#define DMW_PSEG_MASK       0xe000000
#define DMW_VSEG_MASK       0xe0000000

#define TCFG_EN_MASK        0x1
#define TCFG_PERIODIC_MASK  0x2
#define TCFG_INITVAL_MASK   0xfffffffc

#define TICLR_CLR_MASK      0x1

// csr field right bits, Indicates how many bits are on the right side of this field
#define CRMD_IE_OFFSET       2
#define CRMD_DA_OFFSET       3
#define CRMD_PG_OFFSET       4
#define CRMD_DATF_OFFSET     5
#define CRMD_DATM_OFFSET     7

#define PRMD_PIE_OFFSET      2

#define ESTAT_ECODE_OFFSET   16
#define ESTAT_ESUBCODE_OFFSET    22

#define LLBCTL_WCLLB_OFFSET  1
#define LLBCTL_KLO_OFFSET    2

#define TLBIDX_PS_OFFSET     24
#define TLBIDX_NE_OFFSET     31

#define TLBEHI_VPPN_OFFSET   13

#define TLBELO_D_OFFSET      1
#define TLBELO_PLV_OFFSET    2
#define TLBELO_MAT_OFFSET    4
#define TLBELO_G_OFFSET      6
#define TLBELO_PPN_OFFSET    8

#define DMW_PLV3_OFFSET      3
#define DMW_MAT_OFFSET       4
#define DMW_PSEG_OFFSET      25
#define DMW_VSEG_OFFSET      29

#define TCFG_PERIODIC_OFFSET 1
#define TCFG_INITVAL_OFFSET  2

// ecode
#define INT_ECODE   0x0
#define PIL_ECODE   0x1
#define PIS_ECODE   0x2
#define PIF_ECODE   0x3
#define PME_ECODE   0x4
#define PPI_ECODE   0x7
#define ADEF_ECODE  0x8
#define ALE_ECODE   0x9
#define SYS_ECODE   0xb
#define BRK_ECODE   0xc
#define INE_ECODE   0xd
#define IPE_ECODE   0xe
#define FPD_ECODE   0xf
#define FPE_ECODE   0x12
#define TLBR_ECODE  0x3f

#define STRONGLY_ORDERED_UNCACHED 0
#define COHERENT_CACHED           1

// csr R/W copy and modify from nexus-am
#define __ASM_STR(x)	#x

// csrrd rd, csr_num
#define csr_read(csr)       \
    ({                      \
        register unsigned int __v;  \
        __asm__ __volatile__("csrrd %0 , " __ASM_STR(csr) \
                            : "=r"(__v)    \
                            :                  \
                            : "memory");       \
        __v;    \
    })

// csrwr rd, csr_num
#define csr_write(csr, val)                                        \
	({                                                         \
		unsigned int __v = (unsigned int)(val);          \
		__asm__ __volatile__("csrwr %0, " __ASM_STR(csr)  \
				     : "+r"(__v)                           \
				     :                    \
				     : "memory");                  \
	})

// csrxchg rd, rj, csr_num
#define csr_masked_write(csr, val, wmask)                                        \
	({                                                         \
		unsigned int __v = (unsigned int)(val);          \
        unsigned int __wmask = (unsigned int)(wmask);          \
		__asm__ __volatile__("csrxchg %0, %1, " __ASM_STR(csr)  \
				     : "+r"(__v)                          \
				     : "r"(__wmask)                   \
				     : "memory");                  \
	})

#define TLB_ENTRY_NUM 32

#define tlbsrch() __asm__ __volatile__("tlbsrch": : : "memory")

#define tlbrd() __asm__ __volatile__("tlbrd" : : : "memory")

#define tlbwr() __asm__ __volatile__("tlbwr" : : : "memory")

#define tlbfill() __asm__ __volatile("tlbfill" : : : "memory")

#define invtlb(op, asid, va)                                        \
	({                                                         \
		unsigned int __asid = (unsigned int)(asid);          \
		unsigned int __va = (unsigned int)(va);          \
		__asm__ __volatile__("invtlb " __ASM_STR(op) ", %0, %1"  \
				     :                            \
				     : "r"(__asid), "r"(__va)                   \
				     : "memory");                  \
	})

#pragma pack(8)
typedef union {
  struct {
    uint32_t E    : 1;
    uint32_t ASID : 10;
    uint32_t G    : 1;
    uint32_t PS   : 6;
    uint32_t VPPN : 19;
    uint32_t      : 27;    
  };
  uint64_t val;
} EntryHi;
#pragma pack()

typedef union {
  struct {
    uint32_t V     : 1;
    uint32_t D     : 1;
    uint32_t MAT   : 2;
    uint32_t PLV   : 2;
    uint32_t PPN   : 24;
    uint32_t pad0  : 2;
  };
  uint32_t val;  
} EntryLo;

struct tlb_struct{
  EntryHi hi;
  EntryLo lo[2];
} ;

// src1 in $t0($r12), src2 in $t1($r13), dest in $t2($r14)
#define newinst(src1, src2, inst)     \
({                                    \
    register unsigned int __dest;        \
    register unsigned int __src1 = (unsigned int)src1; \
    register unsigned int __src2 = (unsigned int)src2; \
    __asm__ __volatile__("add.w $t0, %[_s1], $zero\n\t"        \
                         "add.w $t1, %[_s2], $zero\n\t"        \
                         ".word " __ASM_STR(inst) "\n\t"         \
                         "add.w %[dest], $t2, $zero\n\t"        \
                         : [dest] "=r" (__dest)                   \
                         : [_s1] "r" (__src1), [_s2] "r" (__src2) \
                         : "$t0", "$t1", "$t2"                  \
     );                                                         \
      __dest;                                                     \
})

#endif
