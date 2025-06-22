#include "ls1c103_pmu.h"
#include "ls1x.h"
#include "Config.h"
extern void cpu_wait(void);

uint32_t pmu_get_time()
{
    uint32_t r;
    read_reg32(r, PMU_BASE, PMU_Count);
    return r;
}
void pmu_wake_timer_start(uint32_t delta)
{
    uint32_t r;
    read_reg32(r, PMU_BASE, PMU_Count);
    r += delta;
    write_reg32(r, PMU_BASE, PMU_Compare);

    read_reg32(r, PMU_BASE, PMU_Command);
    r |= 0x1 << 8;
    write_reg32(r, PMU_BASE, PMU_Command);
}

void pmu_wake_timer_stop()
{
    uint32_t r;
    read_reg32(r, PMU_BASE, PMU_Command);
    r &= ~(0x1 << 8);// clear enable bit will also clear the recorded intr state
    write_reg32(r, PMU_BASE, PMU_Command);
}

void pmu_watchdog_feed()
{
    uint32_t r = 0xa55a55aa;
    write_reg32(r, PMU_BASE, PMU_WdtFeed);
}

void pmu_watchdog_set(uint32_t delay)
{
    uint32_t r = ((~delay) << 16 ) | delay;
    write_reg32(r, PMU_BASE, PMU_WdtCfg);
}

uint32_t wait_int()
{
    //if (cfg_waitop_sleep) cpu_sleep();
    //else                  
    printf("idle 0\n");
    cpu_wait();
    printf("int trig\n");
//    uint32_t intr = pmu_intr;
//    pmu_intr = 0;
//    return intr;
    return 0;
}

uint32_t wait_cpu_int()
{
    cpu_wait();
//    uint32_t intr = cpu_intr;
//    cpu_intr = 0;
//    return intr;
    return 0;
}

void pmu_rst_cmd(uint32_t PMU_RstArr, uint32_t NewState)
{
    if (NewState != DISABLE)
    {
        PMU_SOFTRST |= PMU_RstArr;
    }
    else
    {
        PMU_SOFTRST &= ~PMU_RstArr;
    }
}

void pmu_clk_cmd(uint32_t PMU_ClkArr, uint32_t NewState)
{
    if (NewState != DISABLE)
    {
        PMU_CLKOFF |= PMU_ClkArr;
    }
    else
    {
        PMU_CLKOFF &= ~PMU_ClkArr;
    }
}

/*
void pmu_gpio_bit_write(int idx, uint32_t flag)
{
    write_reg8_v(flag, idx, (GPIOA_REG_BASE+PMU_GPIOBit));
}

uint32_t pmu_gpio_bit_read(int idx)
{
    uint32_t r;
    read_reg8_v(r, idx, (GPIOA_REG_BASE+PMU_GPIOBit));
    return r;
}
*/

