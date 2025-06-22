/*
 * AHT20.h
 *
 * created: 2024/5/21
 *  author: 
 */
#ifndef __AHT20_H
#define __AHT20_H
#include "myiic.h"

#define ATH20_SLAVE_ADDRESS		0x38		/* I2C从机地址 */

//****************************************
// 定义 AHT20 内部地址
//****************************************
#define	AHT20_STATUS_REG		0x00	//状态字 寄存器地址
#define	AHT20_INIT_REG			0xBE	//初始化 寄存器地址
#define	AHT20_SoftReset			0xBA	//软复位 单指令
#define	AHT20_TrigMeasure_REG	0xAC	//触发测量 寄存器地址

// 存储AHT20传感器信息的结构体
struct m_AHT20
{
	uint8_t alive;	// 0-器件不存在; 1-器件存在
	uint8_t flag;	// 读取/计算错误标志位。0-读取/计算数据正常; 1-读取/计算设备失败
	uint32_t HT[2];	// 湿度、温度 原始传感器的值，20Bit

	uint32_t RH;		// 湿度，转换单位后的实际值，标准单位%
    uint32_t Temp;		// 温度，转换单位后的实际值，标准单位°C
};


uint8_t AHT20_Init(void);
uint8_t AHT20_ReadHT(uint32_t *HT);
uint8_t StandardUnitCon(struct m_AHT20* aht);
void collect_proc();
#include "config.h"
extern int wendu,shidu;//温湿度
#include "ls1c102_ptimer.h"
#include "ls1x_latimer.h"

#endif
