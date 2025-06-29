/*
 * SHT30.c
 *
 * created: 2025/5/5
 *  author:
 */


#include "config.h"
#include "SHT30.h"
#include "ls1x_gpio.h"

extern void gpio_init(unsigned char gpio,unsigned char io);
extern void gpio_write(unsigned char gpio,unsigned char val);
extern unsigned char gpio_read(unsigned char gpio);
extern void delay_us(unsigned int us);
extern void delay_ms(unsigned int ms);

#define SHT30_ADDR_WRITE 0x88
#define SHT30_ADDR_READ 0x89


unsigned char CY;
int Temp=0;
int Humi=0;



void gpio_SHT30_init(void)
{
    gpio_init(5,1);
    gpio_init(4,1);
}

/*================================================================
【名 称】void SHT30_Start()
【功 能】I2C起始信号
【备 注】SCL、SDA同为高，SDA跳变成低之后，SCL跳变成低
================================================================*/
void SHT30_Start()
{
    gpio_write(5,1);
    gpio_write(4,1);
    delay_us(5);
    gpio_write(5,0);
    delay_us(10);
    gpio_write(4,0);
    delay_us(5);
}

/*================================================================
【名 称】void SHT30_Stop()
【功 能】I2C停止信号
【备 注】SCL、SDA同为低，SCL跳变成高之后，SDA跳变成高
================================================================*/
void SHT30_Stop()
{
    gpio_write(5,0);
    delay_us(5);
    gpio_write(4,1);
    delay_us(5);
    gpio_write(5,1);
}

/*================================================================
【名 称】void SHT30_SendACK()
【功 能】发送ACK间隔信号
【备 注】
================================================================*/
void SHT30_SendACK()
{
    gpio_write(5,0);
    delay_us(5);
    gpio_write(4,1);
    delay_us(5);
    gpio_write(4,0);
    delay_us(5);
    gpio_write(5,1);
    delay_us(5);
}

unsigned char SHT30_RecvACK()
{
    gpio_write(4,1);
    gpio_init(5,0);
    delay_us(5);
    //gpio_init(5,0);
    CY = gpio_read(5);
    gpio_init(5,1);
    gpio_write(4,0);
    delay_us(5);
    return CY;
}

/*================================================================
【名 称】void SHT30_SendByte(unsigned char dat)
【功 能】I2C写一个字节数据
【备 注】从高到低，依次发送
================================================================*/
void SHT30_SendByte(unsigned char dat)
{
    unsigned int i;

    for (i=0; i<8; i++)
    {
        if((dat&0x80)==0x80)
            CY=1;
        else CY=0;
        gpio_write(5,CY);
        delay_us(5);
        gpio_write(4,1);     //输出SDA稳定后，拉高SCL给出上升沿，从机检测到后进行数据采样
        delay_us(5);
        gpio_write(4,0);
        delay_us(5);
        dat <<= 1;
    }
    SHT30_RecvACK();
}

/*================================================================
【名 称】unsigned char SHT30_ReadByte()
【功 能】I2C读一个字节数据
【备 注】从高到低，依次接收
================================================================*/
unsigned char SHT30_ReadByte()
{
    unsigned int i;
    unsigned char dat;

    gpio_write(4,0);
    gpio_write(5,1);      //释放总线

    for (i=0; i<8; i++)
    {
        dat <<= 1;           //移位
        gpio_write(4,1);     //给出上升沿
        delay_us(5);         //延时等待信号稳定
        dat |= gpio_read(5); //采样获取数据
        gpio_write(4,0);
        delay_us(5);
    }
    return dat;
}

/*================================================================
【名 称】void SHT3XInit(void)
【功 能】SHT3X初始化函数，主函数中调用
================================================================*/
void SHT30_Init()
{
    gpio_SHT30_init();

    SHT30_Start();
    SHT30_SendByte(SHT30_ADDR_WRITE);
    SHT30_SendByte(0x20);
    SHT30_SendByte(0x32);
    SHT30_Stop();
    delay_ms(50);
}

int SHT30_temp(void)
{
    unsigned char temp_h, temp_l, crc1, humi_h, humi_l, crc2;
    int temp;

    SHT30_Start();
    SHT30_SendByte(SHT30_ADDR_WRITE);  // 主机发送写地址
    SHT30_SendByte(0xE0);  // 选择开始温湿度测量命令
    SHT30_SendByte(0x00);
    SHT30_Stop();

    delay_ms(260);    // 延时等待温湿度测量完成

    SHT30_Start();
    SHT30_SendByte(SHT30_ADDR_READ);  // 主机发送读地址
    temp_h = SHT30_ReadByte();
    SHT30_SendACK();
    temp_l = SHT30_ReadByte();
    SHT30_SendACK();
    crc1 = SHT30_ReadByte();
    SHT30_SendACK();
    humi_h = SHT30_ReadByte();
    SHT30_SendACK();
    humi_l = SHT30_ReadByte();
    SHT30_SendACK();
    crc2 = SHT30_ReadByte();
    SHT30_Stop();

    temp = ((temp_h<<8) | temp_l)*175/65535 - 45; // 温度值转换公式

    return temp;
}

int SHT30_humi(void)
{
    unsigned char temp_h, temp_l, crc1, humi_h, humi_l, crc2;
    int humi;

    SHT30_Start();
    SHT30_SendByte(SHT30_ADDR_WRITE);  // 主机发送写地址
    SHT30_SendByte(0xE0);  // 选择开始温湿度测量命令
    SHT30_SendByte(0x00);
    SHT30_Stop();

    delay_ms(20);    // 延时等待温湿度测量完成

    SHT30_Start();
    SHT30_SendByte(SHT30_ADDR_READ);  // 主机发送读地址
    temp_h = SHT30_ReadByte();
    SHT30_SendACK();
    temp_l = SHT30_ReadByte();
    SHT30_SendACK();
    crc1 = SHT30_ReadByte();
    SHT30_SendACK();
    humi_h = SHT30_ReadByte();
    SHT30_SendACK();
    humi_l = SHT30_ReadByte();
    SHT30_SendACK();
    crc2 = SHT30_ReadByte();
    SHT30_Stop();

    humi = ((humi_h<<8) | humi_l)*100/65535; // 湿度值转换公式

    return humi;
}
void SHT30_Get_Temp()
{
    SHT30_Init();
    Temp = SHT30_temp();
    //printf("Temp2:%d\n", Temp);
    if(Temp >=30)
    {
        //gpio_write_pin(GPIO_PIN_15,0);
        for(int i=0;i<1;i++)
        {
            gpio_write_pin(GPIO_PIN_15,0);
            delay_ms(500);
            gpio_write_pin(GPIO_PIN_15,1);
            delay_ms(100);
        }
    }
    else
    {
        gpio_write_pin(GPIO_PIN_15,1);
    }
}

int SHT30_Get_Humi()
{
    SHT30_Init();
    Humi = SHT30_humi();
    //printf("%d ", Humi);
    return Humi;
}


