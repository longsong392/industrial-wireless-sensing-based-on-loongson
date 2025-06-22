/*
 * A158A.c
 *
 * created: 2025/4/12
 *  author:
 */

#include "config.h"
#include "A158A.h"
#include "ls1c102_adc.h"
#include "ls1x_gpio.h"

// ����ʵ����������У׼�Ĳ���
#define TEMP_ADC_26_5   2080    // 26.5��ʱADC����ֵ
#define TEMP_ADC_27_1   2050    // 27.1��ʱADC����ֵ
#define TEMP_ADC_27_2   2040    // 27.2��ʱADC����ֵ
#define TEMP_ADC_27_5   2010    // 27.5��ʱADC����ֵ
#define TEMP_ADC_27_6   1994    // 27.6��ʱADC����ֵ
#define TEMP_ADC_27_7   1934    // 27.7��ʱADC����ֵ
#define TEMP_ADC_28_0   1920    // 28.0��ʱADC����ֵ

#define TEMP_MIN_C       100    // �¶�����10.0�棨��λ0.1�棩
#define TEMP_MAX_C       400    // �¶�����40.0�棨��λ0.1�棩

int adc_val = 0;
int temperature = 250; // ��ʼ�¶���Ϊ25.0��
int T_switch=30;

void A158A_Init() {
    Adc_powerOn(); // ADC��Դ����
    Adc_open(ADC_CHANNEL_I1); // ��ͨ��1
}


int A158A_Getnum() {
    // ���������˲���4�β�����
    static int buf[4] = {0};
    static int idx = 0;

    buf[idx] = Adc_Measure(ADC_CHANNEL_I1);
    idx = (idx + 1) % 4;

    adc_val = (buf[0] + buf[1] + buf[2] + buf[3]) / 4;

    // ����ʵ�����ݽ��зֶ�����ӳ��
    if (adc_val < TEMP_ADC_28_0) {
        // ���������ƣ�ÿ����1 ADC��λ���¶�����0.15��
        temperature = 280 + (TEMP_ADC_28_0 - adc_val) * 15 / 100;
        if (temperature > TEMP_MAX_C) temperature = TEMP_MAX_C;
    } else if (adc_val < TEMP_ADC_27_7) {
        // 27.7�浽28.0������
        temperature = 277 + (TEMP_ADC_27_7 - adc_val) * 30 / (TEMP_ADC_27_7 - TEMP_ADC_28_0);
    } else if (adc_val < TEMP_ADC_27_6) {
        // 27.6�浽27.7������
        temperature = 276 + (TEMP_ADC_27_6 - adc_val) * 10 / (TEMP_ADC_27_6 - TEMP_ADC_27_7);
    } else if (adc_val < TEMP_ADC_27_5) {
        // 27.5�浽27.6������
        temperature = 275 + (TEMP_ADC_27_5 - adc_val) * 10 / (TEMP_ADC_27_5 - TEMP_ADC_27_6);
    } else if (adc_val < TEMP_ADC_27_2) {
        // 27.2�浽27.5������
        temperature = 272 + (TEMP_ADC_27_2 - adc_val) * 30 / (TEMP_ADC_27_2 - TEMP_ADC_27_5);
    } else if (adc_val < TEMP_ADC_27_1) {
        // 27.1�浽27.2������
        temperature = 271 + (TEMP_ADC_27_1 - adc_val) * 10 / (TEMP_ADC_27_1 - TEMP_ADC_27_2);
    } else if (adc_val < TEMP_ADC_26_5) {
        // 26.5�浽27.1������
        temperature = 265 + (TEMP_ADC_26_5 - adc_val) * 6 / (TEMP_ADC_26_5 - TEMP_ADC_27_1);
    } else if (adc_val < TEMP_ADC_27_1) {
        // 26.5�浽27.1�����䣨��������߼���
        temperature = 265 + (TEMP_ADC_27_1 - adc_val) * 6 / (TEMP_ADC_27_1 - TEMP_ADC_26_5);
    } else {
        // ���������ƣ�ÿ����1 ADC��λ���¶Ƚ���0.10��
        temperature = 265 - (adc_val - TEMP_ADC_26_5) * 10 / 100;
        if (temperature < TEMP_MIN_C) temperature = TEMP_MIN_C;
    }
    temperature=temperature-2;

    // ��ӡ��ʽ�Ż�
    //printf("%d.%d ", temperature/10,temperature%10);
    //printf("66 ");
    if(temperature / 10>=T_switch)
    {
        for(int i=0;i<1;i++)
        {
            gpio_write_pin(GPIO_PIN_15,0);
            delay_ms(500);
            gpio_write_pin(GPIO_PIN_15,1);
            delay_ms(100);
        }
        gpio_write_pin(GPIO_PIN_18,1);
        gpio_write_pin(GPIO_PIN_16,0);
    }
    else
    {
        gpio_write_pin(GPIO_PIN_18,0);
        gpio_write_pin(GPIO_PIN_16,1);
        gpio_write_pin(GPIO_PIN_15,1);
    }
    return temperature;
    //delay_ms(1000);
}

