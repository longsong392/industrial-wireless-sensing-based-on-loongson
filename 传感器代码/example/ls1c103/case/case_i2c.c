#include "test.h"
//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1x_gpio.h"
#include "Config.h"
#include "ls1c103_i2cregs.h"

#include "ls1c103_i2c.h"
#include "ls1x_gpio.h"
#include "ls1c103_dma.h"

ucint8_t i2c_tx_buffer[8] = {
    0xa0, 0xb1, 0xc2, 0xd3, 0xe4, 0xf5, 0x16, 0x27
};
uint8_t i2c_rx_buffer[8];

extern unsigned int str2num(unsigned char *);
static void GPIO_Configuration(void)
{
  printf("i2c pad init\n");
  GPIO_InitTypeDef GPIO_InitStructure;
  AFIO_RemapConfig(AFIOA, GPIO_Pin_11 | GPIO_Pin_12, GPIO_FUNC_MAIN);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_2(void)
{
  AFIO_RemapConfig(AFIOA, GPIO_Pin_9 | GPIO_Pin_10, GPIO_FUNC_SECOND);
  printf("i2c pad init\n");
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void i2c_cfg(void)
{

    apb_w(I2C_CCR  , 60); // 60*90ns = 5.4us for SCL high/low
    apb_w(I2C_TRISE, 13); // 1000ns/90ns=11.1+1 -> 13
    apb_w(I2C_OAR, 0xd8); // slaver addr

    apb_w(I2C_CR1, CR1_SWRST); // soft reset (byte/bit control)
    apb_w(I2C_CR1, 0);

    apb_w(I2C_CR2, 11); // 10MHz clock, no dma/int enable

    apb_w(I2C_CR1, CR1_PE); // PE

}

int i2c_recover(int argc, void *argv[])
{
// recover
    apb_w(I2C_CR1, (CR1_PE | CR1_RECOVER)); // recover

    apb_r(I2C_CR1);
    while(rdata & CR1_RECOVER) apb_r(I2C_CR1);

    wait_reg0(I2C_SR2, SR2_BUSY);
    printf("recover\n\r");
    return 0;
}


int i2c_init(int argc, void *argv[])
{
//1:init; 0: deinit
    if (argc !=2)
    {
        printf("\n usage: i2c_init <0/1/2>\n");
        return 1;
    }
    unsigned int enable;
    enable = str2num(argv[1]);
    if(enable == 1){
    GPIO_Configuration();
    i2c_cfg();
    }
    else if(enable == 2){
    GPIO_Configuration_2();
    i2c_cfg();
    }
    else{
    I2C_Cmd(I2C1, DISABLE);
    }
    return 0;
}

int i2c_wr(int argc, void *argv[])
{
    
    if (argc != 3)
    {
        printf("\n usage: iw <addr> <data>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);
    uint32_t  data = str2num(argv[2]);

    apb_w(I2C_CR1, (CR1_PE | CR1_ACK));
    apb_w(I2C_CR1, (CR1_PE | CR1_ACK | CR1_START)); // start
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa0); // addr, write
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    wait_reg1(I2C_SR1, SR1_TXE); 
    apb_w(I2C_DR, 0x00);         // First Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, addr);         // Second Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, data);
    wait_reg1(I2C_SR1, SR1_BTF); 

    apb_w(I2C_CR1, (CR1_PE | CR1_STOP)); // stop
    wait_reg0(I2C_CR1, CR1_STOP);
}

int i2c_rd_one(uint32_t addr)
{
    apb_w(I2C_CR1, (CR1_PE | CR1_START)); // start,fake write in order to set addr
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa0); // addr, write
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    wait_reg1(I2C_SR1, SR1_TXE); 
    apb_w(I2C_DR, 0x00);         // First Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, addr);         // Second Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 

    apb_w(I2C_CR1, (CR1_PE | CR1_START)); // re start,only one read,NACK
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa1); // addr, read
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    //wait_reg1(SR1, SR1_BTF); //NACK,BTF won't be set
    wait_reg1(I2C_SR1, SR1_RXNE); //
    apb_w(I2C_CR1, (CR1_PE | CR1_STOP)); // stop flag
    apb_r(I2C_DR);
    printf("\n\rread 0x%x :0x%x\n",addr, rdata);  

    wait_reg0(I2C_CR1, CR1_STOP);
    return 0;
}

int i2c_rd(int argc, void *argv[])
{
    if (argc != 2)
    {
        printf("\n usage: ir <addr>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);
    i2c_rd_one(addr);

    return 0;
}

static void i2c_dma_tx_cfg(int size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    // I2C TX
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(I2C_BASE + I2C_Register_DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)i2c_tx_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
}
//transmit certain segment from ram to i2c dev
int i2c_dma_wr(int argc, void *argv[])
{

    if (argc != 3)
    {
        printf("\n usage: idmaw <addr> <size>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);
    uint32_t  size = str2num(argv[2]);

    if(size < 1){
	size = 1;
    }
    else if(size >8){
	size = 8;
    }

    i2c_dma_tx_cfg(size);
    
    apb_w(I2C_CR1, (CR1_PE | CR1_ACK));
    apb_w(I2C_CR1, (CR1_PE | CR1_ACK | CR1_START)); // start
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa0); // addr, write
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    wait_reg1(I2C_SR1, SR1_TXE); 
    apb_w(I2C_DR, 0x00);         // First Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, addr);         // Second Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
 
    I2C_DMACmd(I2C1, ENABLE);
    /* enable tx dma */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* wait dma end */
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
    I2C_DMACmd(I2C1, DISABLE);

    wait_reg1(I2C_SR1, SR1_BTF); 

    apb_w(I2C_CR1, (CR1_PE | CR1_STOP)); // stop
    wait_reg0(I2C_CR1, CR1_STOP);

    return 0;
}

void i2c_dma_rx_cfg(int size)
{
    DMA_InitTypeDef  DMA_InitStructure;
    // I2C RX
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(I2C_BASE + I2C_Register_DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = ((uint32_t)i2c_rx_buffer);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
}

//transmit certain segment from i2c dev to ram
int i2c_dma_rd(int argc, void *argv[])
{
    int i;
    if (argc != 3)
    {
        printf("\n usage: ir <addr> <num>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);
    uint32_t  dnum = str2num(argv[2]);
    if(dnum < 1){
    	dnum = 1;
    }
    else if(dnum > 8){
	dnum = 8;
    }
    if(dnum == 1){
        i2c_rd_one(addr);
	return 0;
    }
    else if(dnum == 2){	
    apb_w(I2C_CR1, (CR1_PE | CR1_START)); // start,fake write in order to set addr
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa0); // addr, write
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    wait_reg1(I2C_SR1, SR1_TXE); 
    apb_w(I2C_DR, 0x00);         // First Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, addr);         // Second Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 

    apb_w(I2C_CR1, (CR1_PE | CR1_ACK | CR1_START)); // re start
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa1); // addr, read
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    wait_reg1(I2C_SR1, SR1_BTF); //
    apb_w(I2C_CR1, (CR1_PE | CR1_POS | CR1_STOP)); // stop flag,pos only for 2
    apb_r(I2C_DR);
    printf("\n\rread 0x%x :0x%x\n",addr, rdata);  

    wait_reg1(I2C_SR1, SR1_RXNE); //
    apb_r(I2C_DR);
    printf("\n\r0x%x\n", rdata);  

    wait_reg0(I2C_CR1, CR1_STOP);
    return 0;
    }
    else{
    uint32_t  size = dnum - 3;

    i2c_dma_rx_cfg(size);

    apb_w(I2C_CR1, (CR1_PE | CR1_START)); // start,fake write in order to set addr
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa0); // addr, write
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    wait_reg1(I2C_SR1, SR1_TXE); 
    apb_w(I2C_DR, 0x00);         // First Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, addr);         // Second Word Addr Byte
    wait_reg1(I2C_SR1, SR1_BTF); 

    apb_w(I2C_CR1, (CR1_PE | CR1_ACK | CR1_START)); // re start
    wait_reg1(I2C_SR1, SR1_SB);

    apb_w(I2C_DR, 0xa1); // addr, read
    wait_reg1(I2C_SR1, SR1_ADDR);
   
    /* enable rx dma */
    I2C_DMACmd(I2C1, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);


    apb_r(I2C_SR2);

    if(size > 0){
    /* wait dma end */
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));
    }
    I2C_DMACmd(I2C1, DISABLE);

    wait_reg1(I2C_SR1, SR1_BTF); //
    apb_w(I2C_CR1, (CR1_PE)); // no ack
    apb_r(I2C_DR);
    i2c_rx_buffer[dnum - 3] = rdata;

    wait_reg1(I2C_SR1, SR1_BTF); //
    apb_w(I2C_CR1, (CR1_PE | CR1_STOP)); // stop flag
    apb_r(I2C_DR);
    i2c_rx_buffer[dnum - 2] = rdata;

    wait_reg1(I2C_SR1, SR1_RXNE); //
    apb_r(I2C_DR);
    i2c_rx_buffer[dnum - 1] = rdata;

    
    wait_reg0(I2C_CR1, CR1_STOP);
    printf("\n\rread from:0x%x\n\r",addr);
    for(i=0;i<dnum;i++){
    	printf("0x%x\n", i2c_rx_buffer[i]);  
    }	
    return 0;
    }

    return 0;
}


int i2c_wr_master(int argc, void *argv[])
{
    
    if (argc != 3)
    {
        printf("\n usage: iwm <addr/0xd8> <data>\n");
        return 1;
    }

    uint32_t  addr = str2num(argv[1]);
    uint32_t  data = str2num(argv[2]);

    apb_w(I2C_CR1, (CR1_PE | CR1_ACK));
    apb_w(I2C_CR1, (CR1_PE | CR1_ACK | CR1_START)); // start
    wait_reg1(I2C_SR1, SR1_SB);

    
    printf("\n\rstart\n\r");
    apb_w(I2C_DR, addr); // addr, write
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    printf("\n\raddr ack\n\r");
    wait_reg1(I2C_SR1, SR1_TXE); 
    apb_w(I2C_DR, data);
    wait_reg1(I2C_SR1, SR1_BTF); 
    apb_w(I2C_DR, data + 0x1);
    wait_reg1(I2C_SR1, SR1_BTF); 
    printf("data ack\n\r");
    apb_w(I2C_CR1, (CR1_PE | CR1_STOP)); // stop
    wait_reg0(I2C_CR1, CR1_STOP);
}

int i2c_wr_slaver(int argc, void *argv[])
{
    
    uint32_t  data,data2;

    printf("\n\rwait rx\n\r");
    apb_w(I2C_CR1, (CR1_PE | CR1_ACK));
    wait_reg1(I2C_SR1, SR1_ADDR);
    apb_r(I2C_SR2);

    printf("addr ack\n\r");
    wait_reg1(I2C_SR1, SR1_RXNE); 
    apb_r(I2C_DR);
    data = rdata;

    wait_reg1(I2C_SR1, SR1_RXNE); 
    apb_r(I2C_DR);
    data2 = rdata;
    printf("data ack\n\r");
    wait_reg0(I2C_CR1, CR1_STOP);
    printf("slaver rx:0x%x  0x%x\n\r",data,data2);
}



