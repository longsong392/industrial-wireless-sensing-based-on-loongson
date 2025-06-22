#include "regdef.h"
#include "csrdef.h"
#include "tools-asm.h"
#include "start.h"

/* start address for the initialization values of the .data section. defined in ld.script */
.word  _sidata
/* start address for the .data section. defined in ld.script */  
.word  _sdata
/* end address for the .data section. defined in ld.script */
.word  _edata
/* start address for the .bss section. defined in ld.script */
.word  _sbss
/* end address for the .bss section. defined in ld.script */
.word  _ebss

.globl _RAM_BSS
_RAM_BSS = BSS_BASE
.globl _RAM_DATA
_RAM_DATA = DATA_BASE

.section .text._start                   # begin code segment
.weak _start
.type _start, %function
.globl _start
#All Exception Entries
_start:
        move     t1 , zero 
        b    LoopCopyDataInit
CopyDataInit:
        la.abs   t3, _sidata
        add.w    t4, t3, t1  
        ld.w     t3, t4, 0x0
        add.w    t4, t0, t1  
        st.w     t3, t4, 0x0
        addi.w   t1, t1, 0x4
LoopCopyDataInit:
        la.abs   t0, _sdata
        la.abs   t3, _edata
        add.w    t2, t0, t1
        bne      t2, t3, CopyDataInit
		     /* clear bss */

        la.abs  t0, _sbss
        la.abs  t1, _ebss
        beq     t1, t0, cpu_init_start
LoopFillZerobss:
        st.w    zero, t0, 0
        addi.w  t0, t0, 4
        bne t1, t0, LoopFillZerobss

#set up basic system function
        /* cpu init*/
cpu_init_start:
### reset normal exception base and Ex config
        li.w      t0, 0x1c001000
        csrwr     t0, CSR_EBase        
        li.w      t0, (0<<S_CSR_ExConfig_VS)|(0x0000<<S_CSR_ExConfig_IM) # 4 instruction gap
        csrwr     t0, CSR_ExConfig
        #set int masks
        li.w      t0, 0x1fff
        csrxchg t0, t0, CSR_ExConfig
    ##set VSEG4(0x80000000~0x9fffffff) to cached, other VSEG to uncached
        li.w      t0, CACHE_OP
        csrwr     t0, CSR_SEGCA
    ##set VSEG4(0x80000000~0x9fffffff) to PSEG0(0x00000000~0x1fffffff), other VSEG unchanged
        li.w      t0, ADDR_MAP
        csrwr     t0, CSR_SEGPA
    ##set DA=0(change DA-mode to AD-mode)
        li.w      t0, 0x8
        csrxchg   zero, t0, CSR_CRMD


        li.w    sp, SP_BASE

		bl 		bsp_init
		nop
        jirl      zero, ra, 0


        .globl cpu_wait
cpu_wait:
        idle 0
        jr      ra

#if defined(LS1C102)
		.org 0x0800
		.globl m4_flash_4k
.type m4_flash_4k, %function
m4_flash_4k:
		li.w	t0,0x3ff80      		
		li.w	$r13,0xbfe60000
        and	t0,$r4,t0
        ori	$r15,$r13,0x18
        addi.w	$r16,$r0,0x10
        li.w	$r14,0xa0000000
        st.w	$r16,$r15,0
        or	$r14,t0,$r14
        st.w	$r14,$r13,0
        li.w	$r15,0x40000000
        li.w	$r14,0x5aa555aa
        st.w	$r15,$r13,0
        li.w	$r15,0xe0000000
		st.w	$r14,$r4,0
        or	t0,t0,$r15
        st.w	t0,$r13,0

        jirl      zero, ra, 0
#endif 

        .org 0x1000
DEFAULT_INT_HANDLER:

    SAVE_REGS_FAST(REGS_MEM)
        csrrd   t0, CSR_ExStatus
        andi    t1, t0, INT_VECTOR
        beqz    t1, exception_core_check

exception_pmu:
#if defined (LS1C102)
        andi    t1,t0,0x4
        bnez    t1,wake_label

        andi    t1,t0,0x8
        bnez    t1,touch_label

        andi    t1,t0,0x10
        bnez    t1,uart2_label

        andi    t1,t0,0x20
        bnez    t1,bcc_label

        andi    t1,t0,0x80
        bnez    t1,exint_label

        andi    t1,t0,0x800
        bnez    t1,timer_label

wake_label:
        bl      TIMER_WAKE_INT 
        b       exception_exit

touch_label:
        bl      TOUCH
        b       exception_exit

uart2_label:
        bl      UART2_INT
        b       exception_exit

bcc_label:
        bl      BAT_FAIL
        b       exception_exit

exint_label:
        bl      ext_handler
        b       exception_exit

timer_label:
        bl      TIMER_HANDLER
        b       exception_exit
#elif defined (LS1C103)
        andi    t1,t0,0x4
        bnez    t1,atim_label /*EX_EXT_INT0*/

        andi    t1,t0,0x8
        bnez    t1,gtim_label /*EX_EXT_INT1*/

        andi    t1,t0,0x10
        bnez    t1,adc_label  /*EX_EXT_INT2*/

        andi    t1,t0,0x40
        bnez    t1,pmu_label  /*EX_EXT_INT4*/

        andi    t1,t0,0x80
        bnez    t1,exint_label /*EX_EXT_INT5*/

        andi    t1,t0,0x800
        bnez    t1,timer_label

atim_label:
        bl      atim_handler 
        b       exception_exit

gtim_label:
        bl      gtim_handler
        b       exception_exit

adc_label:
        bl      adc_handler
        b       exception_exit

pmu_label:
        bl      pmu_handler
        b       exception_exit

exint_label:
        bl      ext_handler
        b       exception_exit

timer_label:
        bl      TIMER_HANDLER
        b       exception_exit
#endif

exception_core_check:
        andi    t1, t0, INTC_VECTOR 
        beqz    t1, exception_exit
        bl      intc_handler
        b       exception_exit
    
exception_exit:
    RESTORE_REGS_FAST(REGS_MEM)
        ertn



