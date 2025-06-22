#include "ls1x_gpio.h"
#include "Config.h"

void AFIO_RemapConfig(AFIO_TypeDef* AFIOx, uint16_t GPIO_Pin, GPIOFunc_TypeDef GPIO_Func)
{
    uint32_t currentpinpos = 0x00;
    uint32_t tmpreg = 0x00, pinmask = 0x00;

    tmpreg = AFIOx->IOSEL;
    for (uint8_t i=0; i < 16; i++)
    {
        if ((GPIO_Pin & (0x01 << i)) != 0x0)
        {
            currentpinpos = i << 1;
            pinmask =((uint32_t)0x03 << currentpinpos);
            tmpreg &= ~pinmask;
            tmpreg |= (GPIO_Func << currentpinpos);
        }
    }

    AFIOx->IOSEL = tmpreg;
}


#if defined(LS1C102) /*******************LS1C102************************/

void GPIO_Init(GPIO_InitTypeDef* GPIO_InitStruct)
{
    gpio_pin_remap(GPIO_InitStruct->GPIO_Pin,GPIO_InitStruct->GPIO_Func);   //设置GPIO
    gpio_set_direction(GPIO_InitStruct->GPIO_Pin,GPIO_InitStruct->GPIO_Mode);  //设置为输入
}

/***********************************************************************
函数功能:		GPIO配置为输入
@param:
			pin:	GPIO管脚
@other:     无
@return:	无
***********************************************************************/
INT8U gpio_get_pin(INT8U pin)
{
    PMU_GPIOBit(pin) = 0x00;
    return PMU_GPIOBit(pin);
}

/***********************************************************************
函数功能:		配置GPIO输出
@param:
			pin:	GPIO管脚
			mode: 	输出电平高低
@other:     无
@return:	无
**********************************************************************/
void gpio_write_pin(INT8U pin,INT8U mode)
{
    if(GPIO_HIGH == mode)
    {
        PMU_GPIOBit(pin) = 0x3;
    }
    else
    {
        PMU_GPIOBit(pin) = 0x2;
    }
}
//GPIO管脚复用
void gpio_pin_remap(INT8U pin,INT8U func)
{
    INT32U gpio_group = pin/16;
    INT32U gpio_offset = (pin%16)*2;

    if(pin>=GPIO_PIN_MAX)
        return;

    switch(gpio_group)
    {
        case 0:
            AFIO_PORTA &= ~(0x03<<gpio_offset);
            AFIO_PORTA |= (func<<gpio_offset);
            break;
        case 1:
            AFIO_PORTB &= ~(0x03<<gpio_offset);
            AFIO_PORTB |= (func<<gpio_offset);
            break;
        case 2:
            AFIO_PORTC &= ~(0x03<<gpio_offset);
            AFIO_PORTC |= (func<<gpio_offset);
            break;
        case 3:
            AFIO_PORTD &= ~(0x03<<gpio_offset);
            AFIO_PORTD |= (func<<gpio_offset);
            break;
        default:
            break;
    }
}
//GPIO管脚输出与输入配置
void gpio_set_direction(INT8U pin,INT8U func)
{
    INT32U gpio_offset = pin;
    if(pin>=GPIO_PIN_0 && pin<GPIO_PIN_32)
    {
        PMU_GPIOA_OE &= ~(0x01<<gpio_offset);
        PMU_GPIOA_OE |= (func<<gpio_offset);
    }
    else if(pin>GPIO_PIN_31&&pin<=GPIO_PIN_MAX)
    {
        gpio_offset-=32;
        PMU_GPIOB_OE &= ~(0x01<<gpio_offset);
        PMU_GPIOB_OE |= (func<<gpio_offset);
    }
    else
    {
        return ;
    }
}
void gpio_set_InputStatus(INT8U pin , INT8U func)
{
    INT32U gpio_offset = pin;
    if (pin >= GPIO_PIN_0 && pin < GPIO_PIN_32)
    {
        PMU_GPIOA_O &= ~(0x01 << gpio_offset);
        PMU_GPIOA_O |= (func << gpio_offset);
    }
    else if (pin > GPIO_PIN_31 && pin <= GPIO_PIN_MAX)
    {
        gpio_offset -= 32;
        PMU_GPIOA_O &= ~(0x01 << gpio_offset);
        PMU_GPIOA_O |= (func << gpio_offset);
    }
    else
    {
        return;
    }
}
#elif defined(LS1C103) /******************LS1C103*************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
    uint32_t tmpreg = 0x00, pinmask = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));

    /*---------------------------- GPIO Mode Configuration -----------------------*/
    currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);

    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
    {
        /* Check the parameters */
        assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
        /* Output mode */
        currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
    }

    /*---------------------------- GPIO CRL Configuration ------------------------*/
    /* Configure the eight low port pins */
    if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
    {
        tmpreg = GPIOx->CRL;

        for (pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = ((uint32_t)0x01) << pinpos;
            /* Get the port pins position */
            currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

            if (currentpin == pos)
            {
                pos = pinpos << 2;
                /* Clear the corresponding low control register bits */
                pinmask = ((uint32_t)0x0F) << pos;
                tmpreg &= ~pinmask;

                /* Write the mode configuration in the corresponding bits */
                tmpreg |= (currentmode << pos);

                /* Reset the corresponding ODR bit */
                if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
                {
                    GPIOx->BRR = (((uint32_t)0x01) << pinpos);
                }
                else
                {
                    /* Set the corresponding ODR bit */
                    if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
                    {
                        GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
                    }
                }
            }
        }
        GPIOx->CRL = tmpreg;
    }

    /*---------------------------- GPIO CRH Configuration ------------------------*/
    /* Configure the eight high port pins */
    if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
    {
        tmpreg = GPIOx->CRH;
        for (pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = (((uint32_t)0x01) << (pinpos + 0x08));
            /* Get the port pins position */
            currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
            if (currentpin == pos)
            {
                pos = pinpos << 2;
                /* Clear the corresponding high control register bits */
                pinmask = ((uint32_t)0x0F) << pos;
                tmpreg &= ~pinmask;

                /* Write the mode configuration in the corresponding bits */
                tmpreg |= (currentmode << pos);

                /* Reset the corresponding ODR bit */
                if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
                {
                    GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
                }
                /* Set the corresponding ODR bit */
                if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
                {
                    GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
                }
            }
        }
        GPIOx->CRH = tmpreg;
    }
}

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}


void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->BSRR = GPIO_Pin;
}

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->BRR = GPIO_Pin;
}

#endif

