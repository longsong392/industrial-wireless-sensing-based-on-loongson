#include "start.h"
#include "regdef.h"
#include "csrdef.h"
#include "tools-asm.h"
#if defined (LS1C102)
    	.global serial_out
serial_out:
    //v0 > a4
10: 	li.w    a4, UART0_BASEADDR
    	ld.bu   a4, a4, 0x5
    	andi    a4, a4, 0x20
    	beqz    a4, 10b 
    	li.w    t0, UART0_BASEADDR
    	st.b    a0, t0, 0x0
    	jr      ra

        .global outputaddr
outputaddr:
        addi.w  sp, sp, -4
        st.w    ra, sp, 0

        la      a2, hexdecarr
        li.w    t1, 0xf0000000
        li.w    t2, 8
        li.w    t3, 28
        ori     a1, a0, 0
1:
        beq     t2, zero, 1f
        and     t0, a1, t1
        srl.w   t0, t0, t3
        add.w   t0, t0, a2
        ld.bu   t4, t0, 0
        ori     a0, t4, 0
        bl      serial_out

        srli.w  t1, t1, 4
        addi.w  t3, t3, -4
        addi.w  t2, t2, -1
        b       1b
1:  
        li.w    a0, 0xa
        bl      serial_out
        ld.w    ra, sp, 0
        addi.w  sp, sp, 4
        jr ra

        .global outputstring
outputstring:
        // v1 > a5
        // v0 > a4
        move    a5, a0
1:
        ld.bu   t0, a5, 0
        beq     t0, zero, 1f
10:
        li.w    a4, UART_BASEADDR
        ld.bu   a4, a4, 0x5
        andi    a4, a4, 0x20
        beqz    a4, 10b
        li.w      a3, UART_BASEADDR
        st.b    t0, a3, 0
        
        addi.w  a5, a5, 1
        b       1b
1:
        li.w    a4, UART_BASEADDR
        ld.bu   a4, a4, 0x5
        andi    a4, a4, 0x40
        beqz    a4, 1b
        jr      ra

#elif defined (LS1C103)
    	.global serial_out
serial_out:
    //v0 > a4
10: 	li.w    a4, UART0_BASEADDR
    	ld.w    a4, a4, 0x14
    	andi    a4, a4, 0x20
    	beqz    a4, 10b 
   	li.w    t0, UART0_BASEADDR
    	st.w    a0, t0, 0x0
   	jr      ra
        .global outputaddr
outputaddr:
        addi.w  sp, sp, -4
        st.w    ra, sp, 0

        la      a2, hexdecarr
        li.w    t1, 0xf0000000
        li.w    t2, 8
        li.w    t3, 28
        ori     a1, a0, 0
1:
        beq     t2, zero, 1f
        and     t0, a1, t1
        srl.w   t0, t0, t3
        add.w   t0, t0, a2
        ld.bu   t4, t0, 0
        ori     a0, t4, 0
        bl      serial_out

        srli.w  t1, t1, 4
        addi.w  t3, t3, -4
        addi.w  t2, t2, -1
        b       1b
1:  
        li.w    a0, 0xa
        bl      serial_out
        ld.w    ra, sp, 0
        addi.w  sp, sp, 4
        jr ra

        .global outputstring
outputstring:
        // v1 > a5
        // v0 > a4
        move    a5, a0
1:
        ld.bu   t0, a5, 0
        beq     t0, zero, 1f
10:
        li.w    a4, UART_BASEADDR
        ld.w    a4, a4, 0x14
        andi    a4, a4, 0x20
        beqz    a4, 10b
        li.w    a3, UART_BASEADDR
        st.w    t0, a3, 0
        
        addi.w  a5, a5, 1
        b       1b
1:
        li.w    a4, UART_BASEADDR
        ld.w    a4, a4, 0x14
        andi    a4, a4, 0x40
        beqz    a4, 1b
        jr      ra
#endif
        .section .rodata
        .align 5
        .global msg_wakeup
msg_wakeup:
        .asciz "wakeup!\n"
hexdecarr:
        .asciz "0123456789abcdef"
