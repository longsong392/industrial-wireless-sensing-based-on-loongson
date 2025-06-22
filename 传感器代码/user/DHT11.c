#include "config.h"
#include "DHT11.h"
#include "ls1c102_adc.h"
#include "ls1x_gpio.h"

// 定义DHT11数据引脚
#define DHT11_PIN    5

void DHT11_Rst(void)
{
    gpio_write_pin(DHT11_PIN, 0);    // 拉低DQ
    delay_ms(20);                    // 主机拉低18~30ms
    gpio_write_pin(DHT11_PIN, 1);    // DQ=1
    delay_us(13);                    // 主机拉高10~35us
}

char DHT11_Check(void)
{
    char retry = 0;
    while (gpio_get_pin(DHT11_PIN) && retry < 100) // DHT11会拉低40~80us
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 1;
    else retry = 0;
    while (!gpio_get_pin(DHT11_PIN) && retry < 100) // DHT11拉低后会再次拉高40~80us
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 1;
    return 0;
}

char DHT11_Read_Bit(void)
{
    char retry = 0;
    while (gpio_get_pin(DHT11_PIN) && retry < 100) // 等待变为低电平
    {
        retry++;
        delay_us(1);
    }
    retry = 0;
    while (!gpio_get_pin(DHT11_PIN) && retry < 100) // 等待变高电平
    {
        retry++;
        delay_us(1);
    }
    delay_us(40); // 等待40us
    if (gpio_get_pin(DHT11_PIN)) return 1;
    else return 0;
}

char DHT11_Read_Byte(void)
{
    char i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

char DHT11_Read_Data(char *temp, char *humi)
{
    char buf[5];
    char i;
    DHT11_Rst();
    if (DHT11_Check() == 0) // Check
    {
        for (i = 0; i < 5; i++) // 读取40位数据
        {
            buf[i] = DHT11_Read_Byte();
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
    }
    else return 1;
    return 0;
}

