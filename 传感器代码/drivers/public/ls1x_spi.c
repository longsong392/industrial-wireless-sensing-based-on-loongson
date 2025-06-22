#include "ls1x_spi.h"
#include "ls1x.h"
#include "Config.h"

/*** SPI READ OPTIONS ***/
#define NORMAL    0x0
#define FAST      0x1
#define DUAL_IO   0x2

/**
 * @Description: SPI 分频系数配置 分别配置到控制寄存器、外部寄存器、参数控制寄存器
 * @Input: 分频系数
 * @Output: 无
 * @return {*}无
 * @param {uint8_t} spr 分频系数
 */
void Spi_FreqDiv(uint8_t spr)
{

    SPI->SPCR &=0xFC;
    SPI->SPER &=0xFC;

    SPI->SPCR |=(spr & 0x03);
    SPI->SPER |=((spr>>2) & 0x03);

    SPI->PARAM &= 0x02;
    SPI->PARAM |=((spr<<4) & 0xF0);
    printf("\r\n0 SPI->PARAM:%x H \r\n",SPI->PARAM);
}
/**
 * @Description: spi init
 * @Input: spi 分频系数
 * SPI_DIV_2 SPI_DIV_4 SPI_DIV_16  SPI_DIV_32  SPI_DIV_8 SPI_DIV_64  SPI_DIV_128
 * SPI_DIV_256 SPI_DIV_512 SPI_DIV_1024 SPI_DIV_2048 SPI_DIV_4096
 * @Output: 无
 * @return {*}
 * @param {uint8_t} spr 分频系数 固定为以上的宏定义
 */
void Spi_Init(uint8_t spr)
{
    SPI->SPCR = 0x50; // {int_en, spi, rsvd, mstr, cpol, cpha, spr[1:0]}
    SPI->SPER = 0;
    while(!SPI_TXEMPTY)         ; // wait till tx empty
    while(!SPI_RXEMPTY) SPI->DATA; // read till rx empty
}

/**
 * @Description: spi 读、写 1~4字节
 * @Input: 	数据缓冲，读、写数量
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n		读写数据字节数
 */
void Spi_Write_Read_1to4(uint8_t* buf, int n)
{

    uint32_t i;
    for (i=0; i<n; i++) SPI->DATA = buf[i];
    while(!SPI_TXEMPTY) ;
    for (i=0; i<n-1; i++) buf[i] = SPI->DATA;
    while(SPI_RXEMPTY) ;
    buf[n-1] = SPI->DATA;
}

/**
 * @Description: spi 读 写 N字节
 * @Input: 数据缓冲，读写字节数
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n	读写字节数
 */
void Spi_Write_Read_N(uint8_t* buf, int n)
{
    uint32_t i, j;
    for (i=0; i<4; i++) SPI->DATA = buf[i];
    for (j=0; i<n; i++, j++)
    {
        while(SPI_RXEMPTY) ;
        buf[j] =  SPI->DATA;
        SPI->DATA = buf[i];
    }
    for (; j<n; j++)
    {
        while(SPI_RXEMPTY) ;
        buf[j] = SPI->DATA;
    }
}

/**
 * @Description: SPI 片选信号0 使能并拉低
 * @Input: 无
 * @Output: 无
 * @return {*}
 */
void Spi_Cs_Down(void)
{
    // SPI->SOFTCS = 0xef;

    SPI_ENABLE_CS(1);
    SPI_LOW_CS(1);
}
/**
 * @Description: SPI 片选信号0 使能并拉高
 * @Input: 无
 * @Output: 无
 * @return {*}
 */
void Spi_Cs_Up(void)
{
    // SPI->SOFTCS = 0xff;

    SPI_ENABLE_CS(1);
    SPI_HIGH_CS(1);
}

/**
 * @Description: SPI 发送数据
 * @Input: 数据缓冲；数据数量
 * @Output: 无
 * @return {*}
 * @param {uint8_t*} buf 数据缓冲
 * @param {int} n		数据量
 */
void Spi_Send(uint8_t* buf, int n)
{
    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, n);
    Spi_Cs_Up();
}

/**
 * @Description:
 * @Input:
 * @Output:
 * @return {*}
 */
uint8_t Spiflash_Rdsr(void)
{
    uint8_t buf[4] = {0x05};
    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 2);
    Spi_Cs_Up();
    return buf[1];
}

/**
 * @Description: 等待置0
 * @Input: 无
 * @Output:无
 * @return {*}
 */
void Spiflash_Wait_Wip_Zero(void)
{
    while (Spiflash_Rdsr() & 0x01) {}
}

/**
 * @Description: spi flash 写
 * @Input:
 * @Output:
 * @return {*}
 */
void Spiflash_Wren(void)
{
    uint8_t buf[4] = {0x06};

    Spiflash_Wait_Wip_Zero();
    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 1);
    Spi_Cs_Up();
}

/**
 * @Description: spi flash 写寄存器
 * @Input: 状态
 * @Output: 无
 * @return {*}
 * @param {uint8_t} st 状态
 */
void Spiflash_Wrsr(uint8_t st)
{
    uint8_t buf[4] = {0x01};

    Spiflash_Wren();
    buf[1] = st;
    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 2);
    Spi_Cs_Up();
}

/**
 * @Description: spi flash 读寄存器2
 * @Input: 无
 * @Output: 返回寄存器读取数据
 * @return {*}
 */
uint8_t Spiflash_Rdsr2(void)
{
    uint8_t buf[4] = {0x35};

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 2);
    Spi_Cs_Up();

    return buf[1];
}

/**
 * @Description: spi flash 读寄存器3
 * @Input: 无
 * @Output: 返回读取的值
 * @return {*}
 */
uint8_t Spiflash_Rdsr3(void)
{
    uint8_t buf[4] = {0x15};

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 2);
    Spi_Cs_Up();

    return buf[1];
}

/**
 * @Description: SPI flash 读 ID
 * @Input: 无
 * @Output: 返回id
 * @return {*}
 */
uint16_t Spiflash_Rdid(void)
{
    uint8_t buf[4] = {0x90, 0, 0, 0};

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 4);//trans
    Spi_Write_Read_1to4(buf, 2);//recv
    Spi_Cs_Up();

    return *((uint16_t*)buf);
}

/**
 * @Description: SPI flash 扇区擦除
 * @Input:
 * @Output:
 * @return {*}
 * @param {uint32_t} addr
 */
void Spiflash_Sector_Erase(uint32_t addr)
{
    uint8_t buf[4] = {0x20};

    buf[3] = addr & 0xff;
    addr >>= 8;
    buf[2] = addr & 0xff;
    addr >>= 8;
    buf[1] = addr & 0xff;

    Spiflash_Wren();

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 4);
    Spi_Cs_Up();
    Spiflash_Wait_Wip_Zero();
}


/**
 * @Description: SPI flash 64k 块擦除
 * @Input: 地址
 * @Output: 无
 * @return {*}
 * @param {uint32_t} addr
 */
void Spiflash_Block64k_Erase(uint32_t addr)
{
    uint8_t buf[4] = {0xd8};

    buf[3] = addr & 0xff;
    addr >>= 8;
    buf[2] = addr & 0xff;
    addr >>= 8;
    buf[1] = addr & 0xff;

    Spiflash_Wren();

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 4);
    Spi_Cs_Up();
    Spiflash_Wait_Wip_Zero();
}

/**
 * @Description: SPI flash 页编程
 * @Input: 页地址；数据；
 * @Output: 无
 * @return {*}
 * @param {uint32_t} addr	页地址
 * @param {uint8_t*} data_256	数据缓冲指针
 */
void Spiflash_Page_Program(uint32_t addr, uint8_t* data_256,int n)
{
    uint8_t buf[4] = {0x02};

    buf[3] = addr & 0xff;
    addr >>= 8;
    buf[2] = addr & 0xff;
    addr >>= 8;
    buf[1] = addr & 0xff;

    Spiflash_Wren();

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 4);
    Spi_Write_Read_N(data_256, n);
    Spi_Cs_Up();
    Spiflash_Wait_Wip_Zero();
}

/**
 * @Description: spi flash 读数据
 * @Input: 地址；数据缓冲；数据量；
 * @Output: 无
 * @return {*}
 * @param {uint32_t} addr
 * @param {uint8_t*} data
 * @param {int} n
 */
void Spiflash_Read_Bytes(uint32_t addr, uint8_t* data, int n)
{
    uint8_t buf[4] = {0x03};

    buf[3] = addr & 0xff;
    addr >>= 8;
    buf[2] = addr & 0xff;
    addr >>= 8;
    buf[1] = addr & 0xff;

    Spi_Cs_Down();
    Spi_Write_Read_1to4(buf, 4);
    Spi_Write_Read_N(data, n);
    Spi_Cs_Up();
}






/******************************************************************************
 * 函数名： SpiTransByte
 * 功能： 通过SPI接口传输一个字节
 * 输入参数：
 * 		  byte：写入字节
 * 返回值：
 *      none
 * 备注：无
 ******************************************************************************/
uint8_t SPI_TransByte(uint8_t byte)
{
    uint8_t ucResult=0;
    uint8_t retry=0;
    /*清除中断和溢出标志位*/
    SPI->SPSR = 0xc0;

    //判断写寄存器是否为空
    while(!(SPI->SPSR & 0x04))
    {
        retry++;
        if(retry > 200)
        {
            printf("spi no data send\r\n");
            return 0;
        }
    }
    retry = 0;
    SPI->DATA = byte;
    while(SPI->SPSR & 0x01)
    {
        retry++;
        if(retry > 200)
        {
            printf("spi no data receive\r\n");
            return 0;
        }
    }
    ucResult = SPI->DATA;


    return ucResult;
}

/******************************************************************************
 * 函数名： SpiFlashCsn0Enable
 * 功能： spi flash csn0 使能
 * 输入参数：
 * 		none
 * 返回值：
 *      none
 * 备注：无
 ******************************************************************************/
void SpiFlashCsn0Enable()
{
    /*使能片选信号CSN0*/
    SPI->SOFTCS = 0xE1;//E1
}

/******************************************************************************
 * 函数名： SpiFlashCsn0Disable
 * 功能： spi flash csn0 不使能
 * 输入参数：
 * 		 none
 * 返回值：
 *      none
 * 备注：无
 ******************************************************************************/
void SpiFlashCsn0Disable()
{
    /*使能片选信号CSN0*/
    SPI->SOFTCS = 0xF1;
}

