#include "ls1x_pmu.h"

//void POWER_ClockFastModeConfig(FlagStatus FLAG)
//{
//
//    (FLAG != RESET)? (SET_BIT(CHIPCTRL,POWER_CLOCK_SPEED_FAST_MASK)):(CLEAR_BIT(CHIPCTRL,POWER_CLOCK_SPEED_FAST_MASK));
//}
//


PMU_RstSrc PMU_GetRstRrc(void)
{
    PMU_RstSrc state = PMU_RSTSRC_ext;
    if((PMU->CMDSTS & PMU_RSTSRC_MASK) == PMU_RSTSRC_EXT)
    {
        state = PMU_RSTSRC_ext;
    }
    else
    {
        if((PMU->CMDSTS & PMU_RSTSRC_MASK) == PMU_RSTSRC_WAKE)
        {
            state = PMU_RSTSRC_wake;
        }
        else
        {
            state = PMU_RSTSRC_wdg;
        }
    }
    return state;
}

FlagStatus  PMU_GetBootSpiStatus(void)
{
    FlagStatus state = RESET;
    uint32_t tmp = 0;
    tmp = READ_REG(SRPROT);
    if ((tmp & 0x1) == 0x1)
    {
        return SET;
    }
    return state;
}




