//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "Config.h"
#include "ls1x_gpio.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"


static int err;


uint8_t spi_dma_tx_buffer[128];
uint8_t spi_dma_rx_buffer[128];
extern unsigned int str2num(unsigned char *);
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

                  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5/*| GPIO_Pin_6*/ | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}



int spi_cfg(void)
{

  GPIO_Configuration();
        SPI_SPCR    = 0x52; // enable mstr
        SPI_PARAM   = 0x20; // ~memory_en

  return 0;
}

int myspi_init (int argc, void *argv[])
{   
    spi_cfg();
    printf("\n\r");
    return 0;
}

static void spi_dma_tx_cfg(uint32_t size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    // I2C TX
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(SPI_BASE + 0x8); // SPI_DATA
    DMA_InitStructure.DMA_MemoryBaseAddr = ((uint32_t)spi_dma_tx_buffer);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
}

static void spi_dma_rx_cfg(uint32_t size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    // I2C RX
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(SPI_BASE + 0x8); // SPI_DATA
    DMA_InitStructure.DMA_MemoryBaseAddr = ((uint32_t)spi_dma_rx_buffer);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
}

static uint8_t spi_send_byte(uint8_t data){
    uint8_t spir;
    SPI_DATA = data;
    
    while(SPI_SPSR & 0x10);

    spir = SPI_DATA; // read out
    return spir;
}

static uint8_t spi_receive_byte(void){
   return (spi_send_byte((uint8_t)0xff));
}

int spi_dma_read (int argc, void *argv[])
{  
    spi_dma_tx_buffer[0] = 0x03;
    if (argc != 3)
    {
        printf("\n usage: spt <addr/24bits> <size>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);
    uint32_t  size = str2num(argv[2]) + 4;
    spi_dma_tx_buffer[1] = (addr >> 16) & 0xff;	
    spi_dma_tx_buffer[2] = (addr >> 8) & 0xff;	
    spi_dma_tx_buffer[3] = addr & 0xff;	

    int i;
        spi_dma_tx_cfg(size);
        spi_dma_rx_cfg(size);

        SPI_SOFTCS  = 0xe1; // csn out
        SPI_SPER    = 0x8;  // dmaen

  DMA_Cmd(DMA1_Channel4, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);

  /* wait dma end */
  while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
  while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));

        SPI_SPER    = 0x0;  // dmaen
        while(SPI_SPSR & 0x10);

        if((SPI_SPSR & 0x1) == 0) {printf("\n\re\n");             return -1;}
        else                       printf("\n\rc\n\r");
	
	addr = (spi_dma_tx_buffer[1] << 16) | (spi_dma_tx_buffer[2] << 8) | spi_dma_tx_buffer[3];
	printf("\n\raddr:0x%x\n\r",addr);

    for(i=4;i<size;i++){
	printf("%x\n\r",spi_dma_rx_buffer[i]);
    }

        SPI_SOFTCS  = 0xf0; // csn out
    return 0;
}

int spi_erase (int argc, void *argv[])
{ 
    uint8_t spi_tx_buffer[3];
    if (argc != 2)
    {
        printf("\n usage: sper <addr/24bits>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);

    //write enable
    SPI_SOFTCS  = 0xe1; // csn out
    spi_send_byte((uint8_t)0x06);
    SPI_SOFTCS  = 0xf0; // csn out
    //erase sector
    spi_tx_buffer[0] = (addr >> 16) & 0xff;	
    spi_tx_buffer[1] = (addr >> 8) & 0xff;	
    spi_tx_buffer[2] = addr & 0xff;	
    SPI_SOFTCS  = 0xe1; // csn out
    spi_send_byte((uint8_t)0x20);
    spi_send_byte((uint8_t)spi_tx_buffer[0]);
    spi_send_byte((uint8_t)spi_tx_buffer[1]);
    spi_send_byte((uint8_t)spi_tx_buffer[2]);
    SPI_SOFTCS  = 0xf0; // csn out
    return 0;
}

int spi_dma_write (int argc, void *argv[])
{ 
    if (argc != 3)
    {
        printf("\n usage: sper <addr/24bits>\n");
        return 1;
    }

    int i;
    uint8_t j = 0x10;
    uint32_t  addr = str2num(argv[1]);
    uint32_t  size = str2num(argv[2]) + 4;

    //write enable
    SPI_SOFTCS  = 0xe1; // csn out
    spi_send_byte((uint8_t)0x06);
    SPI_SOFTCS  = 0xf0; // csn out
    //erase sector
    spi_dma_tx_buffer[0] = (uint8_t)0x02;	
    spi_dma_tx_buffer[1] = (addr >> 16) & 0xff;	
    spi_dma_tx_buffer[2] = (addr >> 8) & 0xff;	
    spi_dma_tx_buffer[3] = addr & 0xff;	
    for(i=4;i<size;i++){
       spi_dma_tx_buffer[i] = j;
       j++;
    }

    spi_dma_tx_cfg(size);
    spi_dma_rx_cfg(size);

    SPI_SOFTCS  = 0xe1; // csn out
    SPI_SPER    = 0x8;  // dmaen
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);
    
    /* wait dma end */
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));

     SPI_SPER    = 0x0;  // dmaen
    while(SPI_SPSR & 0x10);
    SPI_SOFTCS  = 0xf0; // csn out
    return 0;
}

