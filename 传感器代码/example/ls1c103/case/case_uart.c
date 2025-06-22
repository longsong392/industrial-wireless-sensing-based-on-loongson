#include "ls1x_uart.h"
#include "ls1x_latimer.h"
#include "ls1x.h"
#include "ls1x_gpio.h"
#include "test.h"
#include "Config.h"
#include "ls1x_gpio.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"


static int err;

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  AFIO_RemapConfig(AFIOA, GPIO_Pin_7 | GPIO_Pin_8, GPIO_FUNC_SECOND);
  
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}


static void set_uart_1_cfg(void)
{
        unsigned int lcr, dll, dlh, dld;
        lcr = 0x3;
#if defined(INTERNAL_8M_CLK) 
        dll = 0x4;
        dlh = 0x0;
        dld = 0x47;
#elif defined(INTERNAL_11M_CLK)
        dll = 0x5;
        dlh = 0x0;
        dld = 0xca;
#endif
        UART1_LCR = lcr | 0x80;
        UART1_DLL = dll;
        UART1_DLH = dlh;
        UART1_DLD = dld;
        UART1_LCR = lcr;
}


static int uart1_cfg(void)
{

  GPIO_Configuration();
  set_uart_1_cfg();

  return 0;
}

int uart1_init (int argc, void *argv[])
{   
    printf("\r\nuart1 init");
    uart1_cfg();
    return 0;
}

static void uart1_dma_tx_config(const char* buf, int size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    // I2C TX
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(UART1_BASEADDR + 0);
    DMA_InitStructure.DMA_MemoryBaseAddr = ((uint32_t)buf);
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

    *(volatile unsigned *)(UART1_BASEADDR + 0x4) = 0x20; // ier_tx_dma_en
    /* enable tx dma */
    DMA_Cmd(DMA1_Channel4, ENABLE);

    /* wait dma end */
    while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == 0);
    *(volatile unsigned *)(UART1_BASEADDR + 0x4) = 0x0; // ier_tx_dma_en

}

int uart1_dma_tx (int argc, void *argv[])
{       
    char uart1_string[] = "this is the uart1 tx test!\n\r";
    uart1_dma_tx_config(uart1_string, sizeof(uart1_string));
    return 0;
}

static void uart1_dma_rx_config(char* buf, int size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    // I2C TX
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(UART1_BASEADDR + 0);
    DMA_InitStructure.DMA_MemoryBaseAddr = ((uint32_t)buf);
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

    *(volatile unsigned *)(UART1_BASEADDR + 0x4) = 0x10; // ier_rx_dma_en
    /* enable rx dma */
    DMA_Cmd(DMA1_Channel3, ENABLE);

    /* wait dma end */
    while(DMA_GetFlagStatus(DMA1_FLAG_TC3) == 0);
    *(volatile unsigned *)(UART1_BASEADDR + 0x4) = 0x0; // ier_rx_dma_en

}

static void uart0_dma_rx_config(char* buf, int size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(UART0_BASEADDR + 0);
    DMA_InitStructure.DMA_MemoryBaseAddr = ((uint32_t)buf);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

   // *(volatile unsigned *)(UART0_BASEADDR + 0x4) = 0x10; // ier_rx_dma_en
    /* enable rx dma */
   // DMA_Cmd(DMA1_Channel1, ENABLE);

   // /* wait dma end */
   // while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
   // *(volatile unsigned *)(UART0_BASEADDR + 0x4) = 0x0; // ier_rx_dma_en

}

int uart1_dma_rx (int argc, void *argv[])
{       
    char uart1_string[11];
    uart1_string[10] = '\0';
    char load[] = "Please write 10 char:\n\r";
    char loadend[] = "input is:\n\r";
    printf("\n\rwait uart1 rx\n\r");
    uart1_dma_tx_config(load, sizeof(load));
    uart1_dma_rx_config(uart1_string, sizeof(uart1_string)-1);
    uart1_dma_tx_config(loadend, sizeof(loadend));
    uart1_dma_tx_config(uart1_string, sizeof(uart1_string)-1);
    printf("uart1 input is:\n\r%s\n\r",uart1_string);
    return 0;
}

int uart1_dma_rx_2 (int argc, void *argv[])
{       
    char uart1_string[29];
    uart1_string[28] = '\0';
    printf("\n\rwait uart1 rx\n\r");
    uart1_dma_rx_config(uart1_string, sizeof(uart1_string)-1);
    printf("uart1 input is:\n\r%s\n\r",uart1_string);
    return 0;
}

#define ERASE_CMD  0xa0000000
#define PAGE_CMD   0xe0000000
#define CLRPL_CMD  0x40000000
#define UPKEY_CMD  0xf0000000
#define UPBND_CMD  0x90000000
#define VERIF_CMD  0x10000000
#define TESTK_CMD  0x20000000
#define INTCLR_CMD 0x30000000
#define FLASH_ADDR 0x18000000
#define FLASH_ADDR_MASK   0x000ff80

uint8_t flash_program_page(uint32_t page_addr, uint8_t *src)
{
    page_addr |= FLASH_ADDR;
    FLASH_CMD_REG = ERASE_CMD | (page_addr & FLASH_ADDR_MASK);
    FLASH_CMD_REG = CLRPL_CMD;
    for (int j=0;j<0x80;j=j+1)
    {
        *(volatile unsigned char*)(page_addr + j) = *(volatile unsigned char*)(src + j);
    }
    FLASH_CMD_REG = PAGE_CMD | (page_addr & FLASH_ADDR_MASK);
    
    return 0;
}


char uart0_string[135];

int ry(int argc, void *argv[])
{
	uint32_t count = 0;
	uint8_t data = 0;
	uint8_t data1 = 0;

	uart0_dma_rx_config(uart0_string, 132);
	*(volatile unsigned *)(UART0_BASEADDR + 0x4) = 0x10; // ier_rx_dma_en
	//do handshake
	do
	{
		UART_SendData(UART0,0x43);
		delay_ms(50);
		while ((!READ_BIT(UART0->LSR,UART_LSR_FLAG_RXNE) )&&(count < 20))
		{
			count++;
		}
		if (UART0->DAT_DL_L == 0x1)
		{
			for (int i=0;i<3;i++)
			{
				data = UART_ReceiveData(UART0);
			}
			break;
		}
	} while (1);

	//do trans data
	UART_SendData(UART0,0x06);
	UART_SendData(UART0,0x43);
	count = 0;
	do
	{
		data = UART_ReceiveData(UART0);
		if (data == 0x1)
		{
		    DMA_Cmd(DMA1_Channel1, ENABLE);
			while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
			flash_program_page(0x80*count,uart0_string+2);
			count++;
			uart0_dma_rx_config(uart0_string, 132);
			delay_ms(2);
			UART_SendData(UART0,0x06);
		}

		if (data == 0x4)
		{
			UART_SendData(UART0,0x15);
			data = UART_ReceiveData(UART0);
			if (data != 0x4)
			{
				return -1;
			}
			UART_SendData(UART0,0x06);
			UART_SendData(UART0,0x43);
			data = UART_ReceiveData(UART0);
			if (data == 0x1)
			{
				DMA_Cmd(DMA1_Channel1, ENABLE);
				while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
				uart0_dma_rx_config(uart0_string, 132);
				UART_SendData(UART0,0x06);
			}
			break;
		}
	}	while (1);

	//UART_SendData(UART0,0x06);
	//UART_SendData(UART0,0x43);
	return 0;
}
