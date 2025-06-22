/*
    rtc_test
    实验内容设置时间，上位机显示时间

*/
#include "ls1x.h"
#include "config.h"
#include "ls1x_gpio.h"
#include "ls1x_rtc.h"
#include "ls1x_latimer.h"
#include "ls1c102_ptimer.h"
#include "ls1c102_interrupt.h"
#include "rtc_test.h"
#include "ls1x_printf.h"
#include "ls1x_uart.h"
#include "DHT11.h"
#include "gpio.h"
#include "PM25.h"
#include "ADC.h"
#include "UserGpio.h"
#include "ls1x_string.h"


extern void WDG_DogFeed();
extern tsRtcTime rtc_param;
unsigned int PM25_dat=0;
unsigned char receive_flag=0;

extern unsigned int receive_index;
extern unsigned char receive_buf[];


unsigned int humi, temp;

void DHT11_collect()
{
    DHT11_receive(&humi, &temp);
    printf("shidu=%d\r\n",humi);
    printf("wendu=%d\r\n",temp);
}
void PM25_collect()
{
    PM25_dat=PM25_get_value(ADC_CHANNEL_I1);
    //printf("PM25=%d\r\n",PM25_dat);
}
void uart_jieshou_proc()
{
    if(receive_flag==1)
    {
        receive_flag=0;
        receive_index=0;
        if(receive_buf[0]==1)
        {
            gpio_write_pin(20,1);
        }
        if(receive_buf[0]==2)
        {
            gpio_write_pin(20,0);
        }
    }
}
int main(void)
{
    rtc_set_time(24, 1, 25, 16, 30, 0);//设置年月日加时间
    //Adc_powerOn();
    //Adc_open(ADC_CHANNEL_I1);
    adc_init();
    

    timer_init(1000);//定时器初始化  1000--1s
    
    
    for (;;)
    {
        WDG_DogFeed();
        uint8_t new_time=rtc_param.sec;
        RtcUpdateData(1, &rtc_param);
        if(rtc_param.sec!=new_time)
        {
            printf("date is: %u/%u/%u--%u:%u:%u\r\n",rtc_param.year+2000,rtc_param.mon,rtc_param.day,rtc_param.hour,rtc_param.min,rtc_param.sec);
        }
        //delay_s(2);
        //DHT11_collect();
        
        
    }
    return 0;
}


