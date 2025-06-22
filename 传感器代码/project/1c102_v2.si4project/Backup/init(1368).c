#include "ls1x.h"
#include "ls1x_pmu.h"
#include "ls1x_common.h"
#include "Config.h"
#include "ls1x_wdg.h"
#include "ls1x_uart.h"
#include "ls1x_latimer.h"

extern void main();
extern void cpu_sleep();
extern void wakeup_reset();
extern void GPIOInit();
extern void WdgInit();
extern void UartInit();

extern uint32_t SystemClockInit();

int bsp_init(void)
{
    DisableInt();

    /*Clock Init*/
    SystemClockInit();
    
    /*IoRemap Init*/
    GPIOInit();
    
    /*WDT Init*/
    WdgInit();
    //WDG_SetWatchDog(8); //8s reboot
    
    /*Serial Init*/
    Uart1_init(115200);
	ls1x_logo();

    /*Enable extern intrrupt. */
    PMU_CMDSTS  |= (1 << 25);
    EnableInt();
    
    /*Enable cpu timer. */
    open_count();
    Wake_Set(0);        //定时唤醒喂狗

    WDG_DogFeed();

    PMU_RstSrc RstSrc = PMU_GetRstRrc();
    if (RstSrc == PMU_RSTSRC_ext)
    {
        printf("\r\nKey reset!\r\n");
        if (PMU_GetBootSpiStatus())
        {
            printf("\r\nspi boot");
        }
        /*Intr Init */
	    main();
    }
    else if (RstSrc == PMU_RSTSRC_wdg)
    {
        printf("\r\ndog bark!\r\n");
	    main();
    }
    else if (RstSrc == PMU_RSTSRC_wake)
    {
        printf("\r\nwakeUp!\r\n");
        WDG_DogFeed();
        Wake_Set(6);
        cpu_sleep();
    }

    return 0;
}
