#ifndef _ADC_H_
#define _ADC_H_

//ChipCtl中ADC使能控制
//#define ADC_EN_I0 0x01		//ADC_I0使能
//#define	ADC_EN_I4 0x10		//ADC_I4使能
//#define	ADC_EN_I5 0x20		//ADC_I5使能
//#define	ADC_EN_I6 0x40		//ADC_I6使能
//#define	ADC_EN_I7 0x80		//ADC_I7使能
typedef enum {
	ADC_EN_I0 = 0x01,			//ADC_I0使能
	ADC_EN_I4 = 0x10,			//ADC_I4使能
	ADC_EN_I5 = 0x20,			//ADC_I5使能
	ADC_EN_I6 = 0x40,			//ADC_I6使能
	ADC_EN_I7 = 0x80			//ADC_I7使能
}AdcEnFlag;

//AdcCtl中的选择通道
typedef enum {
	ADC_CHANNEL_I0 = 0,			//ADC_I0
	ADC_CHANNEL_I1,				//1c101无效
	ADC_CHANNEL_VCORE,			//MCU输入电源电压
	ADC_CHANNEL_1V,				//1.0V基准电压参考采集通道
	ADC_CHANNEL_I4,				//ADC_I4
	ADC_CHANNEL_I5,				//ADC_I5
	ADC_CHANNEL_I6,				//ADC_I6
	ADC_CHANNEL_I7				//ADC_I7
}AdcChannel;

//adc电源开
void Adc_powerOn(void);
//adc电源关
void Adc_powerOff(void);
//打开adc单个通道
void Adc_open(AdcChannel channel);
//关闭adc单个通道
void Adc_close(AdcChannel channel);
//打开adc通道，可以同时打开多个
void Adc_openGroup(AdcEnFlag enFlags);
//关闭adc通道，可以同时关闭多个
void Adc_closeGroup(AdcEnFlag enFlags);
//单次测量
unsigned short Adc_singleMeasure(AdcChannel channel);
//去抖测量
unsigned short Adc_Measure(AdcChannel channel);
//测量电压转换,单位：豪伏
unsigned short Adc_getVoltage(AdcChannel channel);
#include "config.h"

#endif

