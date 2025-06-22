/*
 * myiic.c
 *
 * created: 2024/5/21
 *  author: 
 */

#include "myiic.h"

unsigned char I2C_Direction_Transmitter=0;
unsigned char I2C_Direction_Receiver=1;
//初始化IIC
void IIC_Init(void)
{
    /*
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	//PB6,PB7 输出高
	*/
	gpio_init(4,1);
	gpio_init(5,1);
	IIC_SDA1();
	IIC_SCL1();
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA1();
	IIC_SCL1();
	delay_us(5);
 	IIC_SDA0();//START:when CLK is high,DATA change form high to low
	delay_us(5);
	IIC_SCL0();//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL0();
	IIC_SDA0();//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL1();
	IIC_SDA1();//发送I2C总线结束信号
	delay_us(5);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入
	IIC_SDA1();delay_us(1);
	IIC_SCL1();delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL0();//时钟输出0
	return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL0();
	SDA_OUT();
	IIC_SDA0();
	delay_us(2);
	IIC_SCL1();
	delay_us(2);
	IIC_SCL0();
}
//不产生ACK应答
void IIC_NAck(void)
{
	IIC_SCL0();
	SDA_OUT();
	IIC_SDA1();
    delay_us(2);
	IIC_SCL1();
	delay_us(2);
	IIC_SCL0();
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    u8 a;
	SDA_OUT();
    IIC_SCL0();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        a=(txd&0x80)>>7;
        if(a==1) IIC_SDA1();
        if(a==0) IIC_SDA0();
        txd<<=1;
		delay_us(2);
		IIC_SCL1();
		delay_us(2);
		IIC_SCL0();
		delay_us(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL0();
        delay_us(2);
		IIC_SCL1();
        receive<<=1;
        if(READ_SDA)receive++;
		delay_us(1);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}



/* 对器件读写的封装，从机器件地址为1字节 */


/**
  * @brief  向I2C设备连续写数据（适用于符合IIC通信协议的寄存器地址为uint8类型的器件）
  * @param  addr:I2C从机器件地址
  * @param  reg: I2C从机寄存器地址
  * @param  len: 写入长度
  * @param  buf: uint8数据数组
  * @retval 0,正常; 其他,错误代码;
  */
uint8_t Soft_I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf)
{
    uint8_t i;

	IIC_Start();
	IIC_Send_Byte(dev_addr << 1 | I2C_Direction_Transmitter);//发送器件地址+写命令
	if(IIC_Wait_Ack())//等待应答
	{
		IIC_Stop();
		return 1;
	}

	IIC_Send_Byte(reg_addr);//写寄存器地址
    IIC_Wait_Ack();//等待应答

	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(data_buf[i]);//发送数据
		if(IIC_Wait_Ack())//等待ACK
		{
			IIC_Stop();
			return 1;
		}
	}
    IIC_Stop();
	return 0;
}

/**
  * @brief  从I2C设备连续读数据（适用于符合IIC通信协议的寄存器地址为uint8类型的器件）
  * @param  addr:I2C从机器件地址
  * @param  reg: I2C从机寄存器地址
  * @param  len: 读出长度
  * @param  buf: uint8数据数组
  * @retval 0,正常; 其他,错误代码;
  */
uint8_t Soft_I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf)
{
	IIC_Start();
	/*
	IIC_Send_Byte(dev_addr << 1 | I2C_Direction_Transmitter);//发送器件地址+写命令
	if(IIC_Wait_Ack())//等待应答
	{
		IIC_Stop();
		return 1;
	}

	IIC_Send_Byte(reg_addr);//写寄存器地址
    IIC_Wait_Ack();//等待应答
    */
	IIC_Start();
	IIC_Send_Byte(dev_addr << 1 | I2C_Direction_Receiver);//发送器件地址+读命令
	IIC_Wait_Ack();//等待应答
    delay_us(5);
    while(len>0)
	{
		if(len==1)*data_buf=IIC_Read_Byte(0);//读数据,发送nACK
		else *data_buf=IIC_Read_Byte(1);//读数据,发送ACK
		data_buf++;
		len--;
	}
    IIC_Stop();//产生一个停止条件
	return 0;
}

