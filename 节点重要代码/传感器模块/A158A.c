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

// 根据实测数据重新校准的参数
#define TEMP_ADC_26_5   2080    // 26.5℃时ADC测量值
#define TEMP_ADC_27_1   2050    // 27.1℃时ADC测量值
#define TEMP_ADC_27_2   2040    // 27.2℃时ADC测量值
#define TEMP_ADC_27_5   2010    // 27.5℃时ADC测量值
#define TEMP_ADC_27_6   1994    // 27.6℃时ADC测量值
#define TEMP_ADC_27_7   1934    // 27.7℃时ADC测量值
#define TEMP_ADC_28_0   1920    // 28.0℃时ADC测量值

#define TEMP_MIN_C       100    // 温度下限10.0℃（单位0.1℃）
#define TEMP_MAX_C       400    // 温度上限40.0℃（单位0.1℃）

int adc_val = 0;
int temperature = 250; // 初始温度设为25.0℃
int T_switch=30;

void A158A_Init() {
    Adc_powerOn(); // ADC电源开启
    Adc_open(ADC_CHANNEL_I1); // 打开通道1
}


int A158A_Getnum() {
    // 滑动窗口滤波（4次采样）
    static int buf[4] = {0};
    static int idx = 0;

    buf[idx] = Adc_Measure(ADC_CHANNEL_I1);
    idx = (idx + 1) % 4;

    adc_val = (buf[0] + buf[1] + buf[2] + buf[3]) / 4;

    // 根据实测数据进行分段线性映射
    if (adc_val < TEMP_ADC_28_0) {
        // 高温区外推：每降低1 ADC单位，温度升高0.15℃
        temperature = 280 + (TEMP_ADC_28_0 - adc_val) * 15 / 100;
        if (temperature > TEMP_MAX_C) temperature = TEMP_MAX_C;
    } else if (adc_val < TEMP_ADC_27_7) {
        // 27.7℃到28.0℃区间
        temperature = 277 + (TEMP_ADC_27_7 - adc_val) * 30 / (TEMP_ADC_27_7 - TEMP_ADC_28_0);
    } else if (adc_val < TEMP_ADC_27_6) {
        // 27.6℃到27.7℃区间
        temperature = 276 + (TEMP_ADC_27_6 - adc_val) * 10 / (TEMP_ADC_27_6 - TEMP_ADC_27_7);
    } else if (adc_val < TEMP_ADC_27_5) {
        // 27.5℃到27.6℃区间
        temperature = 275 + (TEMP_ADC_27_5 - adc_val) * 10 / (TEMP_ADC_27_5 - TEMP_ADC_27_6);
    } else if (adc_val < TEMP_ADC_27_2) {
        // 27.2℃到27.5℃区间
        temperature = 272 + (TEMP_ADC_27_2 - adc_val) * 30 / (TEMP_ADC_27_2 - TEMP_ADC_27_5);
    } else if (adc_val < TEMP_ADC_27_1) {
        // 27.1℃到27.2℃区间
        temperature = 271 + (TEMP_ADC_27_1 - adc_val) * 10 / (TEMP_ADC_27_1 - TEMP_ADC_27_2);
    } else if (adc_val < TEMP_ADC_26_5) {
        // 26.5℃到27.1℃区间
        temperature = 265 + (TEMP_ADC_26_5 - adc_val) * 6 / (TEMP_ADC_26_5 - TEMP_ADC_27_1);
    } else if (adc_val < TEMP_ADC_27_1) {
        // 26.5℃到27.1℃区间（修正后的逻辑）
        temperature = 265 + (TEMP_ADC_27_1 - adc_val) * 6 / (TEMP_ADC_27_1 - TEMP_ADC_26_5);
    } else {
        // 低温区外推：每增加1 ADC单位，温度降低0.10℃
        temperature = 265 - (adc_val - TEMP_ADC_26_5) * 10 / 100;
        if (temperature < TEMP_MIN_C) temperature = TEMP_MIN_C;
    }
    temperature=temperature-2;

    // 打印格式优化
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

