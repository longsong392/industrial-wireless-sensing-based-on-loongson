
#include "ls1x.h"
#include "ls1c102_adc.h"
#include "ls1x_common.h"

//adc电源开
void Adc_powerOn(void)
{
	WriteKeyReg(spPMU_ChipCtrl->adc_on,1);    //打开adc电源
}

//adc电源关
void Adc_powerOff(void)
{
	WriteKeyReg(spPMU_ChipCtrl->adc_on,0);    //关闭adc电源
}

//打开adc通道，可以同时打开多个
void Adc_open(AdcChannel channel)
{
	AdcEnFlag flag = 1 << channel;

	if((flag & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,0);    //关闭ADCI0为模拟信号
	}
	else
    {
		//ADC_I4-7
		WriteKeyReg(spPMU_ChipCtrl->adc_en,flag >> 4);    //关闭ADCI4-7对应通道为模拟信号
	}
}

//关闭adc通道，可以同时关闭多个
void Adc_close(AdcChannel channel)
{
	AdcEnFlag flag = 1 << channel;

	if((flag & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,1);    //打开ADCI0为数字输入
	}
	else
     {
		//ADC_I4-7
		unsigned char en = spPMU_ChipCtrl->adc_en;
		en &= ~(flag >> 4);
		WriteKeyReg(spPMU_ChipCtrl->adc_en, en);    //打开ADCI4-7对应通道为数字输入
	}
}

//打开adc通道，可以同时打开多个
void Adc_openGroup(AdcEnFlag enFlags)
{
	if((enFlags & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,0);    //关闭ADCI0为模拟信号
	}
	else {
		//ADC_I4-7
		WriteKeyReg(spPMU_ChipCtrl->adc_en,enFlags >> 4);    //关闭ADCI4-7对应通道为模拟信号
	}
}

//关闭adc通道，可以同时关闭多个
void Adc_closeGroup(AdcEnFlag enFlags)
{
	if((enFlags & ADC_EN_I0) == ADC_EN_I0)
    {
		//ADC_I0
		WriteKeyReg(spPMU_ChipCtrl->adci0_ien,1);    //打开ADCI0为数字输入
	}
	else
    {
		//ADC_I4-7
		unsigned char en = spPMU_ChipCtrl->adc_en;
		en &= ~(enFlags >> 4);
		WriteKeyReg(spPMU_ChipCtrl->adc_en, en);    //打开ADCI4-7对应通道为数字输入
	}
}


//单次测量
unsigned short Adc_singleMeasure(AdcChannel channel)
{
	unsigned short v;

	DisableInt();
	PMU_CHIPCTRL |= 1 << 16;
    PMU_AdcCtrl = 0x100 | channel;               //开始测量与选择测量端口,2分频
	while((PMU_AdcCtrl & (1<<8)));               //判断测量是否结束
	//返回测量结果整合
	v = PMU_AdcDat & 0xFFF;
	EnableInt();
	return v;
}

/*
//去抖测量
unsigned short Adc_Measure(AdcChannel channel)
{
	unsigned short i, rs, max = 0, min = 0xFFF, sum = 0;

	//测量5次，去掉一个最高和最低，中间三个取平均值
	for(i = 0; i<5; i++)
    {
	    PMU_AdcCtrl = 0x100 | channel;               //开始测量与选择测量端口,2分频
		while((PMU_AdcCtrl & (1<<8)));               //判断测量是否结束

		rs =  PMU_AdcDat & 0xFFF;
		sum += rs;
		if( rs > max)
			max = rs;
		if(rs < min)
			min = rs;
	}

	return (sum - max - min) / 3;
}
*/

//去抖测量
unsigned short Adc_Measure(AdcChannel channel)
{
	unsigned short i, rs, max = 0, sum = 0;

	//测量7次，去掉前三次和一个最大值，剩下3次平均
	for(i = 0; i<7; i++)
    {
//		DisableInt();
	    PMU_AdcCtrl = 0x100 | channel;               //开始测量与选择测量端口,2分频
		while((PMU_AdcCtrl & (1<<8)));               //判断测量是否结束
		rs =  PMU_AdcDat & 0xFFF;
//		EnableInt();

		if(i >= 3){
			sum += rs;
			if(rs > max)
				max = rs;
		}
	}

	return (sum - max) / 3;
}

//测量电压转换,单位：豪伏
unsigned short Adc_getVoltage(AdcChannel channel)
{
	unsigned int v = Adc_Measure(channel);
	unsigned int b = Adc_Measure(ADC_CHANNEL_1V);
	return v * 1000 / b;
}


