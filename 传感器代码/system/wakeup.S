#include "regdef.h"
#include "csrdef.h"
#include "tools-asm.h"
#include "start.h"
        .global cpu_sleep
cpu_sleep:
#if defined (LS1C102)
        SAVE_REGS_ALL(REGS_MEM)
        dbar 0
	li.w    t1, PMU_BASE
1:	ld.w    t0, t1,PMU_CommandW
	andi    t0, t0, 0x1
	beqz    t0, 1b
	st.w    t0, t1,PMU_CommandW
        idle 0
#elif defined (LS1C103)
        dbar 0
	li.w    t1, PMU_BASE
1:	ld.w    t0, t1,PMU_CommandW
	andi    t0, t0, 0x1
	beqz    t0, 1b
	st.w    t0, t1,PMU_CommandW
        idle 0
#endif

        .global wakeup_reset
wakeup_reset:
#if defined (LS1C102)
        la      a0, msg_wakeup
        bl      outputstring
        RESTORE_REGS_ALL(REGS_MEM)

        csrwr   t0, CSR_KScratch1
        /*enable interrupt*/
        li.w    t0, 0x4
        csrxchg t0, t0, CSR_CRMD //enter pmu_handler

        csrrd   t0, CSR_KScratch1
        jirl zero, ra, 0
#elif defined (LS1C103)
        csrwr   t0, CSR_KScratch1
        /*enable interrupt*/
        li.w    t0, 0x4
        csrxchg t0, t0, CSR_CRMD //enter pmu_handler

        csrrd   t0, CSR_KScratch1
	b cmdline
#endif
