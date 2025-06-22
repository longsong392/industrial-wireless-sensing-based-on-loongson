/*
 * AHT20.c
 *
 * created: 2024/5/21
 *  author:
 */

 //SDA=gpio5;
 //SCL=gpio4;

#include "AHT20.h"

/**
  * @brief  读AHT20 设备状态字
  * @param  void
  * @retval uint8_t 设备状态字
  */
static uint8_t AHT20_ReadStatusCmd(void)
{
	uint8_t tmp[1];
	Soft_I2C_Read(ATH20_SLAVE_ADDRESS, AHT20_STATUS_REG, 1, tmp);
	return tmp[0];
}

/**
  * @brief  读AHT20 设备状态字 中的Bit3: 校准使能位
  * @param  void
  * @retval uint8_t 校准使能位：1 - 已校准; 0 - 未校准
  */
static uint8_t AHT20_ReadCalEnableCmd(void)
{
	uint8_t tmp;
	tmp = AHT20_ReadStatusCmd();
	return (tmp>>3)&0x01;
}

/**
  * @brief  读AHT20 设备状态字 中的Bit7: 忙标志
  * @param  void
  * @retval uint8_t 忙标志：1 - 设备忙; 0 - 设备空闲
  */
static uint8_t AHT20_ReadBusyCmd(void)
{
	uint8_t tmp;
	tmp = AHT20_ReadStatusCmd();
	return (tmp>>8)&0x01;                                ///////////修改了
}

/**
  * @brief  AHT20 芯片初始化命令
  * @param  void
  * @retval void
  */
static void AHT20_IcInitCmd(void)
{
	uint8_t tmp[2];
	tmp[0] = 0x08;
	tmp[1] = 0x00;
	Soft_I2C_Write(ATH20_SLAVE_ADDRESS, AHT20_INIT_REG, 2, tmp);
}

/**
  * @brief  AHT20 触发测量命令
  * @param  void
  * @retval void
  */
static void AHT20_TrigMeasureCmd(void)
{
	uint8_t tmp[2];
	tmp[0] = 0x33;
	tmp[1] = 0x00;
	Soft_I2C_Write(ATH20_SLAVE_ADDRESS, AHT20_TrigMeasure_REG, 2, tmp);
}

/**
  * @brief  AHT20 软复位命令
  * @param  void
  * @retval void
  */
static void AHT20_SoftResetCmd(void)
{
	uint8_t tmp[1];
	Soft_I2C_Write(ATH20_SLAVE_ADDRESS, AHT20_SoftReset, 0, tmp);
}

/**
  * @brief  AHT20 设备初始化
  * @param  void
  * @retval uint8_t：0 - 初始化AHT20设备成功; 1 - 初始化AHT20失败，可能设备不存在或器件已损坏
  */
uint8_t AHT20_Init(void)
{
	uint8_t rcnt = 2+1;//软复位命令 重试次数，2次
	uint8_t icnt = 2+1;//初始化命令 重试次数，2次

	while(--rcnt)
	{
		icnt = 2+1;

		delay_ms(40);//上电后要等待40ms
		// 读取温湿度之前，首先检查[校准使能位]是否为1
		while((!AHT20_ReadCalEnableCmd()) && (--icnt))// 2次重试机会
		{
			delay_ms(10);
			// 如果不为1，要发送初始化命令
			AHT20_IcInitCmd();
			delay_ms(200);//这个时间不确定，手册没讲
		}

		if(icnt)//[校准使能位]为1,校准正常
		{
			break;//退出rcnt循环
		}
		else//[校准使能位]为0,校准错误
		{
			AHT20_SoftResetCmd();//软复位AHT20器件，重试
			delay_ms(200);//这个时间不确定，手册没讲
		}
	}

	if(rcnt)
	{
		delay_ms(200);//这个时间不确定，手册没讲
		return 0;// AHT20设备初始化正常
	}
	else
	{
		return 1;// AHT20设备初始化失败
	}
}

/**
  * @brief  AHT20 设备读取 相对湿度和温度（原始数据20Bit）
  * @param  uint32_t *HT：存储20Bit原始数据的uint32数组
  * @retval uint8_t：0-读取数据正常; 1-读取设备失败，设备一直处于忙状态，不能获取数据
  */
uint8_t AHT20_ReadHT(uint32_t *HT)
{
	uint8_t cnt=3+1;//忙标志 重试次数，3次
	uint8_t tmp[6];
	uint32_t RetuData = 0;

	// 发送触发测量命令
	AHT20_TrigMeasureCmd();

	do{
		delay_ms(75);//等待75ms待测量完成，忙标志Bit7为0
	}while(AHT20_ReadBusyCmd() && (--cnt));//重试3次

	if(cnt)//设备闲，可以读温湿度数据
	{
		delay_ms(5);
		// 读温湿度数据
		Soft_I2C_Read(ATH20_SLAVE_ADDRESS, AHT20_STATUS_REG, 6, tmp);
		// 计算相对湿度RH。原始值，未计算为标准单位%。
		RetuData = 0;
		RetuData = (RetuData|tmp[1]) << 8;
		RetuData = (RetuData|tmp[2]) << 8;
		RetuData = (RetuData|tmp[3]);
		RetuData = RetuData >> 4;
		HT[0] = RetuData;

		// 计算温度T。原始值，未计算为标准单位°C。
		RetuData = 0;
		RetuData = (RetuData|tmp[3]) << 8;
		RetuData = (RetuData|tmp[4]) << 8;
		RetuData = (RetuData|tmp[5]);
		RetuData = RetuData&0xfffff;
		HT[1] = RetuData;

		return 0;
	}
	else//设备忙，返回读取失败
	{
		return 1;
	}
}

/**
  * @brief  AHT20 温湿度信号转换（由20Bit原始数据，转换为标准单位RH=%，T=°C）
  * @param  struct m_AHT20* aht：存储AHT20传感器信息的结构体
  * @retval uint8_t：0-计算数据正常; 1-计算数据失败，计算值超出元件手册规格范围
  */
int wendu,shidu;
uint8_t StandardUnitCon(struct m_AHT20* aht)
{
	aht->RH = aht->HT[0] *1000/1024/1024;//2^20=1048576 //原式：(double)aht->HT[0] / 1048576 *100，为了浮点精度改为现在的
	aht->Temp = aht->HT[1] *2000 / 1024 / 1024 -500;
	//扩大10倍
    wendu=aht->Temp;
    shidu=aht->RH;
	//限幅,RH=0~100%; Temp=-40~85°C
	if((aht->RH >=0)&&(aht->RH <=1000) && (aht->Temp >=-400)&&(aht->Temp <=850))
	{
		aht->flag = 0;
		return 0;//测量数据正常
	}
	else
	{
		aht->flag = 1;
		return 1;//测量数据超出范围，错误
	}
}
struct m_AHT20 AHT20;
void collect_proc()
{
	IIC_Init();							//IIC管脚初始化
	AHT20.alive=!AHT20_Init();	        //AHT20温湿度传感器初始化
	if(AHT20.alive)// 如果AHT20传感器存在，则读取温湿度数据
	{
		//读取AHT20的 20Bit原始数据
		AHT20.flag = AHT20_ReadHT(AHT20.HT);
		//实际标准单位转换
		StandardUnitCon(&AHT20);
	}
	//delay_ms(2000);
}





