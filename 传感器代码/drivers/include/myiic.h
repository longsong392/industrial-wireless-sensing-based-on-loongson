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
//IO��������
#define SDA_IN()  {gpio_init(5,0);}
#define SDA_OUT() {gpio_init(5,1);}

//IO��������
#define IIC_SCL1()    {gpio_write(4,1);} //SCL
#define IIC_SDA1()    {gpio_write(5,1);}  //SDA
#define IIC_SCL0()    {gpio_write(4,0);} //SCL
#define IIC_SDA0()    {gpio_write(5,0);}  //SDA
#define READ_SDA   gpio_read(5)  //����SDA
#define u8 unsigned char

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

uint8_t Soft_I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf);
uint8_t Soft_I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf);

#include "config.h"

#endif

