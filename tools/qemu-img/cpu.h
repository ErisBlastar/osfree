/*
 * i386 virtual CPU header
 * 
 *  Copyright (c) 2003 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef CPU_I386_H
#define CPU_I386_H

#include "config.h"

#ifdef TARGET_X86_64
#define TARGET_LONG_BITS 64
#else
#define TARGET_LONG_BITS 32
#endif

/* target supports implicit self modifying code */
#define TARGET_HAS_SMC
/* support for self modifying code even if the modified instruction is
   close to the modifying instruction */
#define TARGET_HAS_PRECISE_SMC

#define TARGET_HAS_ICE 1

#include "cpu-defs.h"

#include "softfloat.h"

#if defined(__i386__) && !defined(CONFIG_SOFTMMU)
#define USE_CODE_COPY
#endif

#define R_EAX 0
#define R_ECX 1
#define R_EDX 2
#define R_EBX 3
#define R_ESP 4
#define R_EBP 5
#define R_ESI 6
#define R_EDI 7

#define R_AL 0
#define R_CL 1
#define R_DL 2
#define R_BL 3
#define R_AH 4
#define R_CH 5
#define R_DH 6
#define R_BH 7

#define R_ES 0
#define R_CS 1
#define R_SS 2
#define R_DS 3
#define R_FS 4
#define R_GS 5

/* segment descriptor fields */
#define DESC_G_MASK     (1 << 23)
#define DESC_B_SHIFT    22
#define DESC_B_MASK     (1 << DESC_B_SHIFT)
#define DESC_L_SHIFT    21 /* x86_64 only : 64 bit code segment */
#define DESC_L_MASK     (1 << DESC_L_SHIFT)
#define DESC_AVL_MASK   (1 << 20)
#define DESC_P_MASK     (1 << 15)
#define DESC_DPL_SHIFT  13
#define DESC_S_MASK     (1 << 12)
#define DESC_TYPE_SHIFT 8
#define DESC_A_MASK     (1 << 8)

#define DESC_CS_MASK    (1 << 11) /* 1=code segment 0=data segment */
#define DESC_C_MASK     (1 << 10) /* code: conforming */
#define DESC_R_MASK     (1 << 9)  /* code: readable */

#define DESC_E_MASK     (1 << 10) /* data: expansion direction */
#define DESC_W_MASK     (1 << 9)  /* data: writable */

#define DESC_TSS_BUSY_MASK (1 << 9)

/* eflags masks */
#define CC_C   	0x0001
#define CC_P 	0x0004
#define CC_A	0x0010
#define CC_Z	0x0040
#define CC_S    0x0080
#define CC_O    0x0800

#define TF_SHIFT   8
#define IOPL_SHIFT 12
#define VM_SHIFT   17

#define TF_MASK 		0x00000100
#define IF_MASK 		0x00000200
#define DF_MASK 		0x00000400
#define IOPL_MASK		0x00003000
#define NT_MASK	         	0x00004000
#define RF_MASK			0x00010000
#define VM_MASK			0x00020000
#define AC_MASK			0x00040000 
#define VIF_MASK                0x00080000
#define VIP_MASK                0x00100000
#define ID_MASK                 0x00200000

/* hidden flags - used internally by qemu to represent additionnal cpu
   states. Only the CPL and INHIBIT_IRQ are not redundant. We avoid
   using the IOPL_MASK, TF_MASK and VM_MASK bit position to ease oring
   with eflags. */
/* current cpl */
#define HF_CPL_SHIFT         0
/* true if soft mmu is being used */
#define HF_SOFTMMU_SHIFT     2
/* true if hardware interrupts must be disabled for next instruction */
#define HF_INHIBIT_IRQ_SHIFT 3
/* 16 or 32 segments */
#define HF_CS32_SHIFT        4
#define HF_SS32_SHIFT        5
/* zero base for DS, ES and SS : can be '0' only in 32 bit CS segment */
#define HF_ADDSEG_SHIFT      6
/* copy of CR0.PE (protected mode) */
#define HF_PE_SHIFT          7
#define HF_TF_SHIFT          8 /* must be same as eflags */
#define HF_MP_SHIFT          9 /* the order must be MP, EM, TS */
#define HF_EM_SHIFT         10
#define HF_TS_SHIFT         11
#define HF_IOPL_SHIFT       12 /* must be same as eflags */
#define HF_LMA_SHIFT        14 /* only used on x86_64: long mode active */
#define HF_CS64_SHIFT       15 /* only used on x86_64: 64 bit code segment  */
#define HF_OSFXSR_SHIFT     16 /* CR4.OSFXSR */
#define HF_VM_SHIFT         17 /* must be same as eflags */

#define HF_CPL_MASK          (3 << HF_CPL_SHIFT)
#define HF_SOFTMMU_MASK      (1 << HF_SOFTMMU_SHIFT)
#define HF_INHIBIT_IRQ_MASK  (1 << HF_INHIBIT_IRQ_SHIFT)
#define HF_CS32_MASK         (1 << HF_CS32_SHIFT)
#define HF_SS32_MASK         (1 << HF_SS32_SHIFT)
#define HF_ADDSEG_MASK       (1 << HF_ADDSEG_SHIFT)
#define HF_PE_MASK           (1 << HF_PE_SHIFT)
#define HF_TF_MASK           (1 << HF_TF_SHIFT)
#define HF_MP_MASK           (1 << HF_MP_SHIFT)
#define HF_EM_MASK           (1 << HF_EM_SHIFT)
#define HF_TS_MASK           (1 << HF_TS_SHIFT)
#define HF_LMA_MASK          (1 << HF_LMA_SHIFT)
#define HF_CS64_MASK         (1 << HF_CS64_SHIFT)
#define HF_OSFXSR_MASK       (1 << HF_OSFXSR_SHIFT)

#define CR0_PE_MASK  (1 << 0)
#define CR0_MP_MASK  (1 << 1)
#define CR0_EM_MASK  (1 << 2)
#define CR0_TS_MASK  (1 << 3)
#define CR0_ET_MASK  (1 << 4)
#define CR0_NE_MASK  (1 << 5)
#define CR0_WP_MASK  (1 << 16)
#define CR0_AM_MASK  (1 << 18)
#define CR0_PG_MASK  (1 << 31)

#define CR4_VME_MASK  (1 << 0)
#define CR4_PVI_MASK  (1 << 1)
#define CR4_TSD_MASK  (1 << 2)
#define CR4_DE_MASK   (1 << 3)
#define CR4_PSE_MASK  (1 << 4)
#define CR4_PAE_MASK  (1 << 5)
#define CR4_PGE_MASK  (1 << 7)
#define CR4_PCE_MASK  (1 << 8)
#define CR4_OSFXSR_MASK (1 << 9)
#define CR4_OSXMMEXCPT_MASK  (1 << 10)

#define PG_PRESENT_BIT	0
#define PG_RW_BIT	1
#define PG_USER_BIT	2
#define PG_PWT_BIT	3
#define PG_PCD_BIT	4
#define PG_ACCESSED_BIT	5
#define PG_DIRTY_BIT	6
#define PG_PSE_BIT	7
#define PG_GLOBAL_BIT	8

#define PG_PRESENT_MASK  (1 << PG_PRESENT_BIT)
#define PG_RW_MASK	 (1 << PG_RW_BIT)
#define PG_USER_MASK	 (1 << PG_USER_BIT)
#define PG_PWT_MASK	 (1 << PG_PWT_BIT)
#define PG_PCD_MASK	 (1 << PG_PCD_BIT)
#define PG_ACCESSED_MASK (1 << PG_ACCESSED_BIT)
#define PG_DIRTY_MASK	 (1 << PG_DIRTY_BIT)
#define PG_PSE_MASK	 (1 << PG_PSE_BIT)
#define PG_GLOBAL_MASK	 (1 << PG_GLOBAL_BIT)

#define PG_ERROR_W_BIT     1

#define PG_ERROR_P_MASK    0x01
#define PG_ERROR_W_MASK    (1 << PG_ERROR_W_BIT)
#define PG_ERROR_U_MASK    0x04
#define PG_ERROR_RSVD_MASK 0x08

#define MSR_IA32_APICBASE               0x1b
#define MSR_IA32_APICBASE_BSP           (1<<8)
#define MSR_IA32_APICBASE_ENABLE        (1<<11)
#define MSR_IA32_APICBASE_BASE          (0xfffff<<12)

#define MSR_IA32_SYSENTER_CS            0x174
#define MSR_IA32_SYSENTER_ESP           0x175
#define MSR_IA32_SYSENTER_EIP           0x176

#define MSR_MCG_CAP                     0x179
#define MSR_MCG_STATUS                  0x17a
#define MSR_MCG_CTL                     0x17b

#define MSR_PAT                         0x277

#define MSR_EFER                        0xc0000080

#define MSR_EFER_SCE   (1 << 0)
#define MSR_EFER_LME   (1 << 8)
#define MSR_EFER_LMA   (1 << 10)
#define MSR_EFER_NXE   (1 << 11)
#define MSR_EFER_FFXSR (1 << 14)

#define MSR_STAR                        0xc0000081
#define MSR_LSTAR                       0xc0000082
#define MSR_CSTAR                       0xc0000083
#define MSR_FMASK                       0xc0000084
#define MSR_FSBASE                      0xc0000100
#define MSR_GSBASE                      0xc0000101
#define MSR_KERNELGSBASE                0xc0000102

/* cpuid_features bits */
#define CPUID_FP87 (1 << 0)
#define CPUID_VME  (1 << 1)
#define CPUID_DE   (1 << 2)
#define CPUID_PSE  (1 << 3)
#define CPUID_TSC  (1 << 4)
#define CPUID_MSR  (1 << 5)
#define CPUID_PAE  (1 << 6)
#define CPUID_MCE  (1 << 7)
#define CPUID_CX8  (1 << 8)
#define CPUID_APIC (1 << 9)
#define CPUID_SEP  (1 << 11) /* sysenter/sysexit */
#define CPUID_MTRR (1 << 12)
#define CPUID_PGE  (1 << 13)
#define CPUID_MCA  (1 << 14)
#define CPUID_CMOV (1 << 15)
#define CPUID_PAT  (1 << 16)
#define CPUID_CLFLUSH (1 << 19)
/* ... */
#define CPUID_MMX  (1 << 23)
#define CPUID_FXSR (1 << 24)
#define CPUID_SSE  (1 << 25)
#define CPUID_SSE2 (1 << 26)

#define CPUID_EXT_SS3      (1 << 0)
#define CPUID_EXT_MONITOR  (1 << 3)
#define CPUID_EXT_CX16     (1 << 13)

#define CPUID_EXT2_SYSCALL (1 << 11)
#define CPUID_EXT2_NX      (1 << 20)
#define CPUID_EXT2_FFXSR   (1 << 25)
#define CPUID_EXT2_LM      (1 << 29)

#define EXCP00_DIVZ	0
#define EXCP01_SSTP	1
#define EXCP02_NMI	2
#define EXCP03_INT3	3
#define EXCP04_INTO	4
#define EXCP05_BOUND	5
#define EXCP06_ILLOP	6
#define EXCP07_PREX	7
#define EXCP08_DBLE	8
#define EXCP09_XERR	9
#define EXCP0A_TSS	10
#define EXCP0B_NOSEG	11
#define EXCP0C_STACK	12
#define EXCP0D_GPF	13
#define EXCP0E_PAGE	14
#define EXCP10_COPR	16
#define EXCP11_ALGN	17
#define EXCP12_MCHK	18

enum {
    CC_OP_DYNAMIC, /* must use dynamic code to get cc_op */
    CC_OP_EFLAGS,  /* all cc are explicitely computed, CC_SRC = flags */

    CC_OP_MULB, /* modify all flags, C, O = (CC_SRC != 0) */
    CC_OP_MULW,
    CC_OP_MULL,
    CC_OP_MULQ,

    CC_OP_ADDB, /* modify all flags, CC_DST = res, CC_SRC = src1 */
    CC_OP_ADDW,
    CC_OP_ADDL,
    CC_OP_ADDQ,

    CC_OP_ADCB, /* modify all flags, CC_DST = res, CC_SRC = src1 */
    CC_OP_ADCW,
    CC_OP_ADCL,
    CC_OP_ADCQ,

    CC_OP_SUBB, /* modify all flags, CC_DST = res, CC_SRC = src1 */
    CC_OP_SUBW,
    CC_OP_SUBL,
    CC_OP_SUBQ,

    CC_OP_SBBB, /* modify all flags, CC_DST = res, CC_SRC = src1 */
    CC_OP_SBBW,
    CC_OP_SBBL,
    CC_OP_SBBQ,

    CC_OP_LOGICB, /* modify all flags, CC_DST = res */
    CC_OP_LOGICW,
    CC_OP_LOGICL,
    CC_OP_LOGICQ,

    CC_OP_INCB, /* modify all flags except, CC_DST = res, CC_SRC = C */
    CC_OP_INCW,
    CC_OP_INCL,
    CC_OP_INCQ,

    CC_OP_DECB, /* modify all flags except, CC_DST = res, CC_SRC = C  */
    CC_OP_DECW,
    CC_OP_DECL,
    CC_OP_DECQ,

    CC_OP_SHLB, /* modify all flags, CC_DST = res, CC_SRC.msb = C */
    CC_OP_SHLW,
    CC_OP_SHLL,
    CC_OP_SHLQ,

    CC_OP_SARB, /* modify all flags, CC_DST = res, CC_SRC.lsb = C */
    CC_OP_SARW,
    CC_OP_SARL,
    CC_OP_SARQ,

    CC_OP_NB,
};

#ifdef FLOATX80
#define USE_X86LDOUBLE
#endif

#ifdef USE_X86LDOUBLE
typedef floatx80 CPU86_LDouble;
#else
typedef float64 CPU86_LDouble;
#endif

typedef struct SegmentCache {
    uint32_t selector;
    target_ulong base;
    uint32_t limit;
    uint32_t flags;
} SegmentCache;

typedef union {
    uint8_t _b[16];
    uint16_t _w[8];
    uint32_t _l[4];
    uint64_t _q[2];
    float32 _s[4];
    float64 _d[2];
} XMMReg;

typedef union {
    uint8_t _b[8];
    uint16_t _w[2];
    uint32_t _l[1];
    uint64_t q;
} MMXReg;

#ifdef WORDS_BIGENDIAN
#define XMM_B(n) _b[15 - (n)]
#define XMM_W(n) _w[7 - (n)]
#define XMM_L(n) _l[3 - (n)]
#define XMM_S(n) _s[3 - (n)]
#define XMM_Q(n) _q[1 - (n)]
#define XMM_D(n) _d[1 - (n)]

#define MMX_B(n) _b[7 - (n)]
#define MMX_W(n) _w[3 - (n)]
#define MMX_L(n) _l[1 - (n)]
#else
#define XMM_B(n) _b[n]
#define XMM_W(n) _w[n]
#define XMM_L(n) _l[n]
#define XMM_S(n) _s[n]
#define XMM_Q(n) _q[n]
#define XMM_D(n) _d[n]

#define MMX_B(n) _b[n]
#define MMX_W(n) _w[n]
#define MMX_L(n) _l[n]
#endif
#define MMX_Q(n) q

#ifdef TARGET_X86_64
#define CPU_NB_REGS 16
#else
#define CPU_NB_REGS 8
#endif

typedef struct CPUX86State {
#if TARGET_LONG_BITS > HOST_LONG_BITS
    /* temporaries if we cannot store them in host registers */
    target_ulong t0, t1, t2;
#endif

    /* standard registers */
    target_ulong regs[CPU_NB_REGS];
    target_ulong eip;
    target_ulong eflags; /* eflags register. During CPU emulation, CC
                        flags and DF are set to zero because they are
                        stored elsewhere */

    /* emulator internal eflags handling */
    target_ulong cc_src;
    target_ulong cc_dst;
    uint32_t cc_op;
    int32_t df; /* D flag : 1 if D = 0, -1 if D = 1 */
    uint32_t hflags; /* hidden flags, see HF_xxx constants */

    /* segments */
    SegmentCache segs[6]; /* selector values */
    SegmentCache ldt;
    SegmentCache tr;
    SegmentCache gdt; /* only base and limit are used */
    SegmentCache idt; /* only base and limit are used */

    target_ulong cr[5]; /* NOTE: cr1 is unused */
    uint32_t a20_mask;

    /* FPU state */
    unsigned int fpstt; /* top of stack index */
    unsigned int fpus;
    unsigned int fpuc;
    uint8_t fptags[8];   /* 0 = valid, 1 = empty */
    union {
#ifdef USE_X86LDOUBLE
        CPU86_LDouble d __attribute__((aligned(16)));
#else
        CPU86_LDouble d;
#endif
        MMXReg mmx;
    } fpregs[8];

    /* emulator internal variables */
    float_status fp_status;
    CPU86_LDouble ft0;
    union {
	float f;
        double d;
	int i32;
        int64_t i64;
    } fp_convert;
    
    float_status sse_status;
    uint32_t mxcsr;
    XMMReg xmm_regs[CPU_NB_REGS];
    XMMReg xmm_t0;
    MMXReg mmx_t0;

    /* sysenter registers */
    uint32_t sysenter_cs;
    uint32_t sysenter_esp;
    uint32_t sysenter_eip;
    uint64_t efer;
    uint64_t star;
#ifdef TARGET_X86_64
    target_ulong lstar;
    target_ulong cstar;
    target_ulong fmask;
    target_ulong kernelgsbase;
#endif

    uint64_t pat;

    /* temporary data for USE_CODE_COPY mode */
#ifdef USE_CODE_COPY
    uint32_t tmp0;
    uint32_t saved_esp;
    int native_fp_regs; /* if true, the FPU state is in the native CPU regs */
#endif
    
    /* exception/interrupt handling */
    jmp_buf jmp_env;
    int exception_index;
    int error_code;
    int exception_is_int;
    target_ulong exception_next_eip;
    struct TranslationBlock *current_tb; /* currently executing TB */
    target_ulong dr[8]; /* debug registers */
    int interrupt_request; 
    int user_mode_only; /* user mode only simulation */

    /* soft mmu support */
    /* in order to avoid passing too many arguments to the memory
       write helpers, we store some rarely used information in the CPU
       context) */
    unsigned long mem_write_pc; /* host pc at which the memory was
                                   written */
    target_ulong mem_write_vaddr; /* target virtual addr at which the
                                     memory was written */
    /* 0 = kernel, 1 = user */
    CPUTLBEntry tlb_read[2][CPU_TLB_SIZE];
    CPUTLBEntry tlb_write[2][CPU_TLB_SIZE];
    
    /* from this point: preserved by CPU reset */
    /* ice debug support */
    target_ulong breakpoints[MAX_BREAKPOINTS];
    int nb_breakpoints;
    int singlestep_enabled;

    /* processor features (e.g. for CPUID insn) */
    uint32_t cpuid_level;
    uint32_t cpuid_vendor1;
    uint32_t cpuid_vendor2;
    uint32_t cpuid_vendor3;
    uint32_t cpuid_version;
    uint32_t cpuid_features;
    uint32_t cpuid_ext_features;
    uint32_t cpuid_xlevel;
    uint32_t cpuid_model[12];
    uint32_t cpuid_ext2_features;
    
#ifdef USE_KQEMU
    int kqemu_enabled;
#endif
    /* in order to simplify APIC support, we leave this pointer to the
       user */
    struct APICState *apic_state;
    /* user data */
    void *opaque;
} CPUX86State;

CPUX86State *cpu_x86_init(void);
int cpu_x86_exec(CPUX86State *s);
void cpu_x86_close(CPUX86State *s);
int cpu_get_pic_interrupt(CPUX86State *s);
/* MSDOS compatibility mode FPU exception support */
void cpu_set_ferr(CPUX86State *s);

/* this function must always be used to load data in the segment
   cache: it synchronizes the hflags with the segment cache values */
static inline void cpu_x86_load_seg_cache(CPUX86State *env, 
                                          int seg_reg, unsigned int selector,
                                          uint32_t base, unsigned int limit, 
                                          unsigned int flags)
{
    SegmentCache *sc;
    unsigned int new_hflags;
    
    sc = &env->segs[seg_reg];
    sc->selector = selector;
    sc->base = base;
    sc->limit = limit;
    sc->flags = flags;

    /* update the hidden flags */
    {
        if (seg_reg == R_CS) {
#ifdef TARGET_X86_64
            if ((env->hflags & HF_LMA_MASK) && (flags & DESC_L_MASK)) {
                /* long mode */
                env->hflags |= HF_CS32_MASK | HF_SS32_MASK | HF_CS64_MASK;
                env->hflags &= ~(HF_ADDSEG_MASK);
            } else 
#endif
            {
                /* legacy / compatibility case */
                new_hflags = (env->segs[R_CS].flags & DESC_B_MASK)
                    >> (DESC_B_SHIFT - HF_CS32_SHIFT);
                env->hflags = (env->hflags & ~(HF_CS32_MASK | HF_CS64_MASK)) |
                    new_hflags;
            }
        }
        new_hflags = (env->segs[R_SS].flags & DESC_B_MASK)
            >> (DESC_B_SHIFT - HF_SS32_SHIFT);
        if (env->hflags & HF_CS64_MASK) {
            /* zero base assumed for DS, ES and SS in long mode */
        } else if (!(env->cr[0] & CR0_PE_MASK) || 
                   (env->eflags & VM_MASK) ||
                   !(env->hflags & HF_CS32_MASK)) {
            /* XXX: try to avoid this test. The problem comes from the
               fact that is real mode or vm86 mode we only modify the
               'base' and 'selector' fields of the segment cache to go
               faster. A solution may be to force addseg to one in
               translate-i386.c. */
            new_hflags |= HF_ADDSEG_MASK;
        } else {
            new_hflags |= ((env->segs[R_DS].base | 
                            env->segs[R_ES].base |
                            env->segs[R_SS].base) != 0) << 
                HF_ADDSEG_SHIFT;
        }
        env->hflags = (env->hflags & 
                       ~(HF_SS32_MASK | HF_ADDSEG_MASK)) | new_hflags;
    }
}

/* wrapper, just in case memory mappings must be changed */
static inline void cpu_x86_set_cpl(CPUX86State *s, int cpl)
{
#if HF_CPL_MASK == 3
    s->hflags = (s->hflags & ~HF_CPL_MASK) | cpl;
#else
#error HF_CPL_MASK is hardcoded
#endif
}

/* used for debug or cpu save/restore */
void cpu_get_fp80(uint64_t *pmant, uint16_t *pexp, CPU86_LDouble f);
CPU86_LDouble cpu_set_fp80(uint64_t mant, uint16_t upper);

/* the following helpers are only usable in user mode simulation as
   they can trigger unexpected exceptions */
void cpu_x86_load_seg(CPUX86State *s, int seg_reg, int selector);
void cpu_x86_fsave(CPUX86State *s, uint8_t *ptr, int data32);
void cpu_x86_frstor(CPUX86State *s, uint8_t *ptr, int data32);

/* you can call this signal handler from your SIGBUS and SIGSEGV
   signal handlers to inform the virtual CPU of exceptions. non zero
   is returned if the signal was handled by the virtual CPU.  */
struct siginfo;
int cpu_x86_signal_handler(int host_signum, struct siginfo *info, 
                           void *puc);
void cpu_x86_set_a20(CPUX86State *env, int a20_state);

uint64_t cpu_get_tsc(CPUX86State *env);

void cpu_set_apic_base(CPUX86State *env, uint64_t val);
uint64_t cpu_get_apic_base(CPUX86State *env);
void cpu_set_apic_tpr(CPUX86State *env, uint8_t val);
#ifndef NO_CPU_IO_DEFS
uint8_t cpu_get_apic_tpr(CPUX86State *env);
#endif

/* will be suppressed */
void cpu_x86_update_cr0(CPUX86State *env, uint32_t new_cr0);

/* used to debug */
#define X86_DUMP_FPU  0x0001 /* dump FPU state too */
#define X86_DUMP_CCOP 0x0002 /* dump qemu flag cache */

#define TARGET_PAGE_BITS 12
#include "cpu-all.h"

#endif /* CPU_I386_H */
