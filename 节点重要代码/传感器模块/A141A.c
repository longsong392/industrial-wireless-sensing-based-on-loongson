/*
 * A141A.c
 *
 * created: 2025/4/12
 *  author:
 */

#include "config.h"
#include "A141A.h"
#include "ls1c102_adc.h"
#include "ls1x_gpio.h"

//extern void gpio_init(unsigned char gpio,unsigned char io);
//extern void gpio_write(unsigned char gpio,unsigned char val);
//extern unsigned char gpio_read(unsigned char gpio);
//extern void delay_us(unsigned int us);
//extern void delay_ms(unsigned int ms);

int value=0;
int light_data=0;
int L_switch=300;

void A141A_Init()
{
    Adc_powerOn();//adc电源开启
    Adc_open(ADC_CHANNEL_I0);//打开通道0
}

int A141A_Getnum()
{
    value=Adc_Measure(ADC_CHANNEL_I0);
    light_data=(4095 - value) * 300 / 4095;
    //printf("55 ");
    //printf("%d ",light_data);
    if(light_data>=L_switch)
    {
        gpio_write_pin(GPIO_PIN_20,1);
    }
    else
    {
        gpio_write_pin(GPIO_PIN_20,0);
    }
    return light_data;
    //gpio_write_pin(GPIO_PIN_55,1);
    //delay_ms(1000);
}






