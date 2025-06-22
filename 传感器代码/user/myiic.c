/*
 * myiic.c
 *
 * created: 2024/5/21
 *  author: 
 */

#include "myiic.h"

unsigned char I2C_Direction_Transmitter=0;
unsigned char I2C_Direction_Receiver=1;
//��ʼ��IIC
void IIC_Init(void)
{
    /*
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	//PB6,PB7 �����
	*/
	gpio_init(4,1);
	gpio_init(5,1);
	IIC_SDA1();
	IIC_SCL1();
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA1();
	IIC_SCL1();
	delay_us(5);
 	IIC_SDA0();//START:when CLK is high,DATA change form high to low
	delay_us(5);
	IIC_SCL0();//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL0();
	IIC_SDA0();//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL1();
	IIC_SDA1();//����I2C���߽����ź�
	delay_us(5);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����
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
	IIC_SCL0();//ʱ�����0
	return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    u8 a;
	SDA_OUT();
    IIC_SCL0();//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}



/* ��������д�ķ�װ���ӻ�������ַΪ1�ֽ� */


/**
  * @brief  ��I2C�豸����д���ݣ������ڷ���IICͨ��Э��ļĴ�����ַΪuint8���͵�������
  * @param  addr:I2C�ӻ�������ַ
  * @param  reg: I2C�ӻ��Ĵ�����ַ
  * @param  len: д�볤��
  * @param  buf: uint8��������
  * @retval 0,����; ����,�������;
  */
uint8_t Soft_I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf)
{
    uint8_t i;

	IIC_Start();
	IIC_Send_Byte(dev_addr << 1 | I2C_Direction_Transmitter);//����������ַ+д����
	if(IIC_Wait_Ack())//�ȴ�Ӧ��
	{
		IIC_Stop();
		return 1;
	}

	IIC_Send_Byte(reg_addr);//д�Ĵ�����ַ
    IIC_Wait_Ack();//�ȴ�Ӧ��

	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(data_buf[i]);//��������
		if(IIC_Wait_Ack())//�ȴ�ACK
		{
			IIC_Stop();
			return 1;
		}
	}
    IIC_Stop();
	return 0;
}

/**
  * @brief  ��I2C�豸���������ݣ������ڷ���IICͨ��Э��ļĴ�����ַΪuint8���͵�������
  * @param  addr:I2C�ӻ�������ַ
  * @param  reg: I2C�ӻ��Ĵ�����ַ
  * @param  len: ��������
  * @param  buf: uint8��������
  * @retval 0,����; ����,�������;
  */
uint8_t Soft_I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, unsigned char *data_buf)
{
	IIC_Start();
	/*
	IIC_Send_Byte(dev_addr << 1 | I2C_Direction_Transmitter);//����������ַ+д����
	if(IIC_Wait_Ack())//�ȴ�Ӧ��
	{
		IIC_Stop();
		return 1;
	}

	IIC_Send_Byte(reg_addr);//д�Ĵ�����ַ
    IIC_Wait_Ack();//�ȴ�Ӧ��
    */
	IIC_Start();
	IIC_Send_Byte(dev_addr << 1 | I2C_Direction_Receiver);//����������ַ+������
	IIC_Wait_Ack();//�ȴ�Ӧ��
    delay_us(5);
    while(len>0)
	{
		if(len==1)*data_buf=IIC_Read_Byte(0);//������,����nACK
		else *data_buf=IIC_Read_Byte(1);//������,����ACK
		data_buf++;
		len--;
	}
    IIC_Stop();//����һ��ֹͣ����
	return 0;
}

