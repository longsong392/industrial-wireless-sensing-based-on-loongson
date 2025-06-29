/*
 * MQ2.c
 *
 * created: 2025/4/21
 *  author:
 */


#include "config.h"
#include "MQ2.h"
#include "ls1c102_adc.h"
#include "ls1x_gpio.h"

int yanwu=0;
int smog_data=0;
int M_switch=160;

void MQ2_Init()
{
    Adc_powerOn();
    Adc_open(ADC_CHANNEL_I4);
}

int MQ2_Getnum()
{
    yanwu=Adc_Measure(ADC_CHANNEL_I4);
    smog_data=yanwu * 330 / 2000;
    if(smog_data<=M_switch)
    {
        gpio_write_pin(GPIO_PIN_63,1);
        //delay_ms(100);
    }
    else
    {
        for(int i=0;i<2;i++)
        {
            gpio_write_pin(GPIO_PIN_63,0);
            delay_ms(250);
            gpio_write_pin(GPIO_PIN_63,1);
            delay_ms(200);
        }
        //gpio_write_pin(GPIO_PIN_63,0);
    }
    return smog_data;
    //delay_ms(1000);
}





