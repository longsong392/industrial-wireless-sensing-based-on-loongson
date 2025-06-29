#include "config.h"
#include "DHT11.h"
#include "ls1c102_adc.h"
#include "ls1x_gpio.h"

// ����DHT11��������
#define DHT11_PIN    5

void DHT11_Rst(void)
{
    gpio_write_pin(DHT11_PIN, 0);    // ����DQ
    delay_ms(20);                    // ��������18~30ms
    gpio_write_pin(DHT11_PIN, 1);    // DQ=1
    delay_us(13);                    // ��������10~35us
}

char DHT11_Check(void)
{
    char retry = 0;
    while (gpio_get_pin(DHT11_PIN) && retry < 100) // DHT11������40~80us
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 100) return 1;
    else retry = 0;
    while (!gpio_get_pin(DHT11_PIN) && retry < 100) // DHT11���ͺ���ٴ�����40~80us
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
    while (gpio_get_pin(DHT11_PIN) && retry < 100) // �ȴ���Ϊ�͵�ƽ
    {
        retry++;
        delay_us(1);
    }
    retry = 0;
    while (!gpio_get_pin(DHT11_PIN) && retry < 100) // �ȴ���ߵ�ƽ
    {
        retry++;
        delay_us(1);
    }
    delay_us(40); // �ȴ�40us
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
        for (i = 0; i < 5; i++) // ��ȡ40λ����
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

