/*
 * myiic.h
 *
 * created: 2024/5/21
 *  author: 
 */

#ifndef __MYIIC_H
#define __MYIIC_H

#include "gpio.h"
#include "ls1x_latimer.h"
//IO方向设置
#define SDA_IN()  {gpio_init(5,0);}
#define SDA_OUT() {gpio_init(5,1);}

//IO操作函数
#define IIC_SCL1()    {gpio_write(4,1);} //SCL
#define IIC_SDA1()    {gpio_write(5,1);}  //SDA
#define IIC_SCL0()    {gpio_write(4,0);} //SCL
#define IIC_SDA0()    {gpio_write(5,0);}  //SDA
#define READ_SDA   gpio_read(5)  //输入SDA
#define u8 unsigned char

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

uint8_t Soft_I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf);
uint8_t Soft_I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf);

#include "config.h"

#endif

