/*
 * gpio.c
 *
 * created: 2024/5/16
 *  author: 
 */
#include "gpio.h"

//PMU_TypeDef *g_pmu;

void gpio_init(unsigned char gpio,unsigned char io)//1_输出
{
    unsigned int bit;
    if(gpio<32)
    {
        bit=(unsigned int)gpio;
        if(io==0) PMU_GPIOA_OE &= ~(0x01<<bit);
        else PMU_GPIOA_OE |= (0x01<<bit);
    }
    else
    {
        bit=(unsigned int)(gpio-32);
        if(io==0) PMU_GPIOB_OE &= ~(0x01<<bit);
        else PMU_GPIOB_OE |= (0x01<<bit);
    }
}
void gpio_write(unsigned char gpio,unsigned char val)//1_高电平
{
    unsigned int bit;
    if(gpio<32)
    {
        bit=(unsigned int)gpio;
        if(val==0) PMU_GPIOA_O &= ~(0x01<<bit);
        else PMU_GPIOA_O |= (0x01<<bit);
    }
    else
    {
        bit=(unsigned int)(gpio-32);
        if(val==0) PMU_GPIOB_O &= ~(0x01<<bit);
        else PMU_GPIOB_O |= (0x01<<bit);
    }
}
unsigned char gpio_read(unsigned char gpio)
{
    unsigned int bit;
    if(gpio<32)
    {
        bit=gpio;
        if((PMU_GPIOA_I&(0x01<<bit))==(0x01<<bit)) return 1;
        else return 0;
    }
    else
    {
        bit=gpio-32;
        if((PMU_GPIOB_I&(0x01<<bit))==(0x01<<bit)) return 1;
        else return 0;
    }
}
//gpio复用函数
/*
void gpio_iosel(unsigned char gpio,unsigned char iosel)
{
    unsigned int bit;
    if(gpio<16)
    {
        bit=gpio;
        g_pmu->IOSEL0 |= iosel<<(bit*2);
    }
    else if(gpio<32)
    {
        bit=gpio;
        g_pmu->IOSEL1 |= iosel<<(bit*2);
    }
    else if(gpio<48)
    {
        bit=gpio;
        g_pmu->IOSEL2 |= iosel<<(bit*2);
    }
    else if(gpio<64)
    {
        bit=gpio;
        g_pmu->IOSEL3 |= iosel<<(bit*2);
    }
}
*/

