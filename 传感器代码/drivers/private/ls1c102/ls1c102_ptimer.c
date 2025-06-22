//#include"ls1c101.h"
#include "ls1x.h"
#include "ls1c102_ptimer.h"

/**
  * @brief  Deinitializes the TIMER peripheral registers to their default reset values..
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @param  TIM_InitStruct: Timer parameter structure.
  *   This parameter Contains parameters for TIMER initialization.
  * @retval None
  */
void TIM_Init(TIM_InitTypeDef* TIM_InitStruct)
{
    TIMER_CFG=0x00;
    TIMER_CNT=0x00;
    TIMER_CMP=TIM_InitStruct->TIME_CMP;
    TIMER_STP=TIM_InitStruct->TIME_STP;
    SET_BIT(TIMER_CFG,TIM_InitStruct->TIME_INTEN);
    if (TIM_InitStruct->TIME_INTEN)
    {
        SET_BIT(INT_EN,IRQ_TIMER);
    }
    SET_BIT(TIMER_CFG,TIM_InitStruct->TIME_PERIODIC);
    SET_BIT(TIMER_CFG,TIM_InitStruct->TIME_START);
}

/**
  * @brief  Fills each TIMER_InitStruct member with its default value.
  * @param  TIM_InitStruct: pointer to a TIM_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void TIM_StructInit(TIM_InitTypeDef* TIM_InitStruct)
{
    /* Set the default configuration */
    TIM_InitStruct->Clock_Freq = 8000000;						//系统时钟
    TIM_InitStruct->TIME_START = 0x01;							//启动定时器
    TIM_InitStruct->TIME_INTEN = 0x01 <<1;						//中断使能
    TIM_InitStruct->TIME_PERIODIC = 0x01 << 2;   				//周期触发
    TIM_InitStruct->TIME_CMP =7999;						//初始值1ms中断触发
    TIM_InitStruct->TIME_STP =8000 * 10;						//步进10ms
    TIM_InitStruct->TIME_CNT = 0;								//计数器初始值设为0
}

/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @param  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable the TIM Counter */
        TIMER_CFG |= 0x01;
    }
    else
    {
        /* Disable the TIM Counter */
        TIMER_CFG &= 0xFE;
    }
}

void TIM_ITConfig(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        TIMER_CFG |= 0x02;
        SET_BIT(INT_EN,IRQ_TIMER);
    }
    else
    {
        /* Disable the Interrupt sources */
        TIMER_CFG &= 0xFD;
        CLEAR_BIT(INT_EN,IRQ_TIMER);
    }
}

/**
  * @brief  Gets the TIMx Counter value.
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @retval Counter Register value.
  */
uint32_t TIM_GetCounter(void)
{
    uint32_t data = 0;

    data = TIMER_CNT ;

    return data;
}

/**
  * @brief  Sets the TIMx Counter Register value
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @param  Counter: specifies the Counter register new value.
  * @retval None
  */
void TIM_SetCounter(uint32_t Counter)
{
    /* Set the Counter Register value */
    TIMER_CNT = Counter;
}


/**
  * @brief  Sets the TIMx Capture Compare Register value
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @param  Compare: specifies the Capture Compare register new value.
  * @retval None
  */
void TIM_SetCompare(uint32_t Compare)
{
    /* Set the Capture Compare1 Register value */
    TIMER_CMP = Compare;
}


/**
  * @brief  Gets the TIMx Capture Compare Register value
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @retval Compare register value.
  */
uint32_t TIM_GetCompare(void)
{
    uint32_t data = 0;

    data = TIMER_CMP ;

    return data;

}


/**
  * @brief  Checks whether the TIM interrupt has occurred or not.
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @param  TIM_FLAG: specifies the TIM interrupt source to check.
  *   This parameter can be TIM_FLAG_Trigger
  * @note
  * @retval The new state of the TIM_IT(SET or RESET).
  */
FlagStatus TIM_GetITStatus(uint32_t TIM_FLAG)
{
    ITStatus bitstatus = RESET;
    if ((TIMER_CFG & TIM_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}
/**
  * @brief  Clears the TIMx's interrupt pending bits.
  * @param  TIMx: select the TIMER peripheral.
  *   This parameter can be TIMER.
  * @param  TIM_FLAG: specifies the TIM interrupt source to check.
  *   This parameter can be  TIM_FLAG_Trigger
  * @retval None
  */
void TIM_ClearIT(uint32_t TIM_FLAG)
{
    /* Clear the flags */
    TIMER_CFG |= TIM_FLAG;
    INT_CLR |= IRQ_TIMER;
}
////////////////////////////////////////mycode
TIM_InitTypeDef TIM_InitStruct;
void timer_init(uint32_t msec)
{



    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIME_STP =8000*msec;

    TIM_Init(&TIM_InitStruct);

}




