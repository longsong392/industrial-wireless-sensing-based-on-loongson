#include "ls1x_exti.h"
#include "ls1x.h"


void EXTI_DeInit(EXTI_TypeDef* exti)
{
    exti->EXINT_SRC  =  0x00000000;
    exti->EXINT_EDGE =  0x00000000;
    exti->EXINT_POL  =  0x00000000;
    exti->EXINT_EN   =  0x00000000;
}

void EXTI_Init(EXTI_TypeDef* exti, EXTI_InitTypeDef* EXTI_InitStruct)
{
    //INT32U tmp = 0;

    /* Check the parameters */
    assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
    assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
    assert_param(IS_EXTI_GPIO(EXTI_InitStruct->EXTI_GPIO));
    assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

    if (EXTI_InitStruct->EXTI_GpioCmd != DISABLE)
    {

        /* enable external interrupt cpu */
        PMU_CMDSTS &=~((INT32U)(1<<25));
        PMU_CMDSTS |=(INT32U)(1<<25);
        if(EXTI_InitStruct->EXTI_Mode == EXTI_Mode_Level)
        {
            CLEAR_BIT(exti->EXINT_EDGE,EXTI_InitStruct->EXTI_GPIO);
        }
        else
        {
            SET_BIT(exti->EXINT_EDGE,EXTI_InitStruct->EXTI_GPIO);
        }

        SET_BIT(exti->EXINT_EN,EXTI_InitStruct->EXTI_GPIO);

        /* Clear exint pol configuration */
        exti->EXINT_POL &= ~EXTI_InitStruct->EXTI_GPIO;
        /* Select the trigger for the selected external interrupts */
        if (EXTI_InitStruct->EXTI_Trigger != EXTI_Trigger_Rising_High)
        {
            SET_BIT(exti->EXINT_POL,EXTI_InitStruct->EXTI_GPIO);
        }
        else
        {
            CLEAR_BIT(exti->EXINT_POL,EXTI_InitStruct->EXTI_GPIO);
        }

        SET_BIT(exti->EXINT_SRC,EXTI_InitStruct->EXTI_GPIO);
    }
    else
    {
        CLEAR_BIT(exti->EXINT_EN,EXTI_InitStruct->EXTI_GPIO);
    }
}

void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
    EXTI_InitStruct->EXTI_GPIO = EXTI_GPIONONE;
    EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Edge;
    EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Rising_High;
    EXTI_InitStruct->EXTI_GpioCmd = DISABLE;
}

FlagStatus EXTI_GetFlagStatus(EXTI_TypeDef* exti, INT32U EXTI_GPIO)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_EXTI_GPIO(EXTI_GPIO));

    if ((exti->EXINT_SRC & EXTI_GPIO) != (INT32U)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

void EXTI_ClearFlag(EXTI_TypeDef* exti, INT32U EXTI_GPIO)
{
    /* Check the parameters */
    assert_param(IS_EXTI_GPIO(EXTI_GPIO));

    exti->EXINT_SRC |= EXTI_GPIO;
}

ITStatus EXTI_GetITStatus(EXTI_TypeDef* exti, INT32U EXTI_GPIO)
{
    ITStatus bitstatus = RESET;
    INT32U enablestatus = 0;
    /* Check the parameters */
    assert_param(IS_EXTI_GPIO(EXTI_GPIO));

    enablestatus =  exti->EXINT_EN & EXTI_GPIO;

    if (((exti->EXINT_SRC & EXTI_GPIO) != (INT32U)RESET) && (enablestatus != (INT32U)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

void EXTI_ClearITPendingBit(EXTI_TypeDef* exti, INT32U EXTI_GPIO)
{
    /* Check the parameters */
    assert_param(IS_EXTI_GPIO(EXTI_GPIO));
    exti->EXINT_SRC |= EXTI_GPIO;
}


