
#include"ls1x_clock.h"
#include"ls1x_latimer.h"
#include"start.h"

uint32_t		g_SystemFreq = 8000000; // default 8M

#if defined  (LS1C103)
uint32_t CLOCK_IramToDramEn(void)
{
    PMU->CHIPCTRL		|= (1 << 25);				/*iramas-d*/
    return 0;
}
#endif // LS1C103



void CLOCK_checkSelect(unsigned char enable)
{
    unsigned int x;
    
    //如果32k时钟初始化时没有配置成功，则继续配置32时钟
    if ( (PMU_CMDSTS & (1 << 28)) == 0 || (PMU_CMDSTS & (1 << 29)) != 0)
    {
        PMU_CHIPCTRL &= ~(1 << 5);
        for(x = 2; x > 0; x--);  // delay 30us
        PMU_CHIPCTRL |= (1 << 5);
        for(x = 2; x > 0; x--);  // delay 30us
    }
}

/**
  * @brief	Waits for HSE start-up.
  * @param	None
  * @retval An ErrorStatus enumuration value:
  * - SUCCESS: HSE oscillator is  to be use
  * - ERROR: HSE oscillator not yet ready
  */
ErrorStatus CLOCK_WaitForHSEStartUp()
{
    uint32_t		nDelay = 2*8000;        // 2ms
    ErrorStatus 	status = ERROR;

    PMU->CHIPCTRL		|= CLOCK_SYSCLK_HSE_EN;

    /*
     * 不加延时，睡眠起来后会时钟失效? 用loop 计数循环
     */
    while (nDelay-- > 0);

    while (PMU->CMDSTS & CLOCK_HSE_FAIL)
    {
        PMU->CHIPCTRL &= ~CLOCK_SYSCLK_HSE_SELECT;
    }
    PMU->CHIPCTRL	|= CLOCK_SYSCLK_HSE_SELECT;

    status			= SUCCESS;

    return status;
}

ErrorStatus CLOCK_WaitForLSEStartUp(void)
{
    ErrorStatus 	status = ERROR;

    //CLOCK_checkSelect(CLOCK_EXT32k_ENABLE);
    
#if 1
    /* Wait till LSE is ready and if Time out is reached exit */
    while((PMU_CMDSTS & (1 << 28)) == 0 || (PMU_CMDSTS & (1 << 29)) != 0)
    {
        PMU_CHIPCTRL &= ~CLOCK_LSE_SELECT;
        CLOCK_delayUs(30);  // delay 30us
        PMU_CHIPCTRL |= CLOCK_LSE_SELECT;
        CLOCK_delayUs(30);  // delay 30us
        
        //break; // this state will failed all the time.
    }
#endif
    // PMU_CHIPCTRL		|= CLOCK_LSE_SELECT;
    //CLOCK_checkSelect(CLOCK_EXT32k_ENABLE);

    status				= SUCCESS;

    return (status);
}


/******************************************************************************
 *Function Name：	 CLOCK_HSEConfig
 *Brief：Configures the External High Speed oscillator (HSE).
 *
 *Param：
 *		 RCC_HSE: specifies the new state of the HSE.
 *		 RCC_HSE_OFF: HSE oscillator OFF,system use internal 8M clock;
 *		 RCC_HSE_ON: HSE oscillator ON,system use external 8M clock;
 *Return value：
 *		 the state of function;
 *Remarks：
 *
 *
 ******************************************************************************/
ErrorStatus CLOCK_HSEConfig(uint32_t clock)
{
    ErrorStatus status	= ERROR;

    /* Check the parameters */
    assert_param(IS_CLOCK_HSE(clock));

    /* reset the HSE bit  */
    PMU->CHIPCTRL		&= (uint32_t)~clock;
    CLOCK_delayMs(8);

    if (clock == CLOCK_SYSCLK_HSE_SELECT)
    {
        status				= CLOCK_WaitForHSEStartUp();
    }
    else
    {
        PMU->CHIPCTRL		&= ~CLOCK_SYSCLK_HSE_SELECT;
        status				= SUCCESS;
    }

    return status;
}


/******************************************************************************
 *Function Name：	 CLOCK_LSEConfig
 *Brief：Configures the External Low Speed oscillator (LSE 32K).
 *
 *Param：
 *		 RCC_LSE: specifies the new state of the HSE.
 *		 RCC_LSE_OFF: HSE oscillator OFF,system use internal 8M clock;
 *		 RCC_LSE_ON: HSE oscillator ON,system use external 8M clock;
 *Return value：
 *		 the state of function;
 *Remarks：
 *
 *
 ******************************************************************************/
ErrorStatus CLOCK_LSEConfig(uint32_t clock)
{
    ErrorStatus status	= ERROR;

    /* Check the parameters */
    assert_param(IS_CLOCK_LSE(clock));

    if (clock == CLOCK_LSE_SELECT)
    {
        status				= CLOCK_WaitForLSEStartUp();
    }
    else
    {
        /* reset the LSE bit  */
        PMU->CHIPCTRL		&= (uint32_t)~CLOCK_LSE_SELECT;
        status				= SUCCESS;
    }

    return status;
}


/******************************************************************************
 *Function Name：	 CLOCK_StructInit
 *Brief：Fills each CLOCK member with its default value.
 *
 *Param：
 *		 CLOCK: pointer to a CLOCK_InitTypeDef structurewhich will be initialized.
 *
 *Return value：
 *		 None
 *Remarks：
 *
 ******************************************************************************/
void CLOCK_StructInit(CLOCK_InitTypeDef * CLOCK)
{

    CLOCK->OscillatorType = (CLOCK_OSCILLATORTYPE_HSE | CLOCK_OSCILLATORTYPE_HSI |
                             CLOCK_OSCILLATORTYPE_LSE | CLOCK_OSCILLATORTYPE_LSI);
    CLOCK->HSEState = CLOCK_HSE_ON;
    CLOCK->LSEState = CLOCK_LSE_ON;
    CLOCK->LSISpeed = CLOCK_LSI_SPEED_32K;
    CLOCK->StartClkHS = CLOCK_START_CLK_HS_5140US;
    CLOCK->FastEn = CLOCK_FASTEN_OFF;
    CLOCK->TurboEn = CLOCK_TURBO_OFF;
}


/******************************************************************************
 *Function Name：	 CLOCK_Init
 *Brief：Fills each CLOCK member with its default value.
 *
 *Param：
 *		 CLOCK: pointer to a CLOCK_InitTypeDef structure which was initialized.
 *
 *Return value：
 *		 the state of function;
 *Remarks：
 *
 ******************************************************************************/
ErrorStatus CLOCK_Init(CLOCK_InitTypeDef * CLOCK)
{
    ErrorStatus status	= ERROR;

    /* set HSE start delay time  */
    CLEAR_BIT(PMU->CHIPCTRL, CLOCK_START_CLK_HS_MASK);
    SET_BIT(PMU->CHIPCTRL, CLOCK->StartClkHS); // start clock delay.

    /* set LSISpeed parameters*/
    CLEAR_BIT(PMU->CHIPCTRL, CLOCK_LSI_SPEED_MASK);
    SET_BIT(PMU->CHIPCTRL, CLOCK->LSISpeed);   // 32K clock select

    /* set FASTEN parameters*/
    CLEAR_BIT(PMU->CHIPCTRL, CLOCK_FASTEN_MASK);
    SET_BIT(PMU->CHIPCTRL, CLOCK->FastEn);     // fast mode enable
    
    /**Initializes the HSE */
    if (CLOCK_HSEConfig(CLOCK->HSEState) != SUCCESS)
    {
        return status;
    }

    /**Initializes the LSE */
    if (CLOCK_LSEConfig(CLOCK->LSEState) != SUCCESS)
    {
        return status;
    }

    if (CLOCK->HSEState != CLOCK_HSE_ON)
    {
        // internal 32M clock
        if (CLOCK->FastEn != 0x0)
        {
            /* set FASTEN parameters*/
            CLEAR_BIT(PMU->CHIPCTRL, CLOCK_TURBO_MASK);
            SET_BIT(PMU->CHIPCTRL, CLOCK->TurboEn);

            if (ATE_Freq != 0)
            {
                g_SystemFreq			= ATE_Freq * 4 / 3 * 1000;
            }
            else
            {
                g_SystemFreq			= 10667000; // fast mode
            }
        }
        else
        {
            if (ATE_Freq != 0)
            {
                g_SystemFreq			= ATE_Freq * 1000;
            }
            else
            {
                g_SystemFreq			= 8000000;
            }
        }
    }
    else
    {
        // extern 8M clock
        g_SystemFreq		= 8000000;
    }

    status				= SUCCESS;
    return status;
}


/******************************************************************************
 *Function Name：	 CLOCK_GetFlagStatus
 *Brief：Checks which Clock is set or not.
 *
 *Param： CLCOK_FLAG: specifies the flag to check.
 *		 CLOCK_FLAG_HSERDY: HSE oscillator clock ready
 *		 CLOCK_FLAG_HSEFAIL: HSE oscillator clock fail
 *		 CLOCK_FLAG_LSERDY: LSE oscillator clock ready
 *		 CLOCK_FLAG_LSEFAIL: LSE oscillator clock fail
 *
 *Return value：
 *		 The new state of CLCOK_FLAG (SET or RESET).
 *Remarks：
 *
 ******************************************************************************/
FlagStatus CLOCK_GetFlagStatus(uint32_t CLCOK_FLAG)
{
    FlagStatus status	= RESET;
    assert_param(IS_CLOCK_FLAG(CLCOK_FLAG));
    status				= PMU->CMDSTS & CLCOK_FLAG;
    return status;
}


/******************************************************************************
 *Function Name：	 CLOCK_GetClocksFreq
 *Brief：get the inner clock frequency  value.
 *
 *Param：
 *		 Clocks : the inner clock frequency  value .
 *
 *Return value：
 *		 none
 *Remarks：
 *
 ******************************************************************************/
uint32_t CLOCK_GetClocksFreq(void)
{
    open_count();
    uint32_t tmp		= 0;
    asm volatile(\
                 /* "nop;nop;nop;\n"	*/ \
                 "li.w	$r13, %1;\n" \
                 "li.w	$r16, 0xfffff;\n" \
                 "rdtimel.w $r14, $r0;\n" \
                 "ld.w	$r17, $r13, 0x08;\n" \
                 "addi.w $r17, $r17, 0x40;\n" \
                 "and	$r16, $r16, $r17;\n" \
                 "1: \n" \
                 "ld.w	$r15, $r13, 0x08;\n" \
                 "bne 	$r15, $r16, 1b ;\n" \
                 "nop;\n" \
                 "rdtimel.w $r18, $r0;\n" \
                 "sub.w $r18, $r18, $r14;\n" \
                 "slli.w %0, $r18, 0x02;\n" \
                 : "=r" (tmp) \
                 : "i" (PMU_BASE) \
                 : "$r18", "$r13", "$r14", "$r15", "$r16", "$r17" \
                );
    close_count();
    return tmp;
}


uint32_t SystemClockInit(void)
{
    CLOCK_InitTypeDef CLOCK =
    {
        0
    };

    CLOCK_StructInit(&CLOCK);

#if defined 						(LS1C102)
    CLOCK.OscillatorType = (CLOCK_OSCILLATORTYPE_HSI | CLOCK_OSCILLATORTYPE_HSE \
                            | CLOCK_OSCILLATORTYPE_LSI | CLOCK_OSCILLATORTYPE_LSE); //ls1c102
    CLOCK.HSEState = CLOCK_HSE_ON;  // use extern 8M clock
    CLOCK.LSEState = CLOCK_LSE_ON;   // use extern 32K clock
    CLOCK.LSISpeed = CLOCK_LSI_SPEED_32K;           // LSI clock set to 32K
    CLOCK.StartClkHS = CLOCK_START_CLK_HS_480US;   // clock start delay 5.14ms
    CLOCK.FastEn = CLOCK_FASTEN_OFF;  // internal 10M speed on
    CLOCK.TurboEn = CLOCK_TURBO_OFF;  // RAM 32M speed on
#elif defined						(LS1C103)
    CLOCK.OscillatorType = (CLOCK_OSCILLATORTYPE_HSI | CLOCK_OSCILLATORTYPE_LSI); //ls1c103
    CLOCK.HSEState = CLOCK_HSE_OFF;
    CLOCK.LSEState = CLOCK_LSE_OFF;
    CLOCK.LSISpeed = CLOCK_LSI_SPEED_32K;
    CLOCK.StartClkHS = CLOCK_START_CLK_HS_480US;
    CLOCK.FastEn = CLOCK_FASTEN_ON; // internal 10M speed on
    CLOCK.TurboEn = CLOCK_TURBO_ON; // RAM 32M speed on
    CLOCK_IramToDramEn();
#endif
    //CLOCK_delayMs(1000);
    if (CLOCK_Init(&CLOCK) != SUCCESS)
    {
        return - 1;
    }

    return 0;
}



