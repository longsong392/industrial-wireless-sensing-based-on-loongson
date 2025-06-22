//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "Config.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"

static int err;

#define DMA_TEST_ADDR0 (RAM0_BASE + 0x400)
#define DMA_TEST_ADDR1 (RAM0_BASE + 0x500)

#define DMA_TEST_ADDR2 (RAM1_BASE + 0x400)

#define BufferSize  32

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


ucint32_t SRC_Const_Buffer[BufferSize]= {0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                        0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                        0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                        0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                        0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                        0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                        0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                        0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};

//Test Case by Loongson

TestStatus Buffercmp(ucint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return FAILED;
    }
    
    pBuffer++;
    pBuffer1++;
  }

  return PASSED;  
}

static void dma_ch1_cfg()
{   
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_TEST_ADDR0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
}

static void dma_ch2_cfg()
{
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DMA_TEST_ADDR0;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_TEST_ADDR1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
}

static void dma_ch3_cfg()
{
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DMA_TEST_ADDR1;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_TEST_ADDR2;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
}

static void dma_ch4_cfg()
{
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DMA_TEST_ADDR2;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_TEST_ADDR0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
}


static int dma_test(void)
{
    uint32_t tcif1, tcif2, tcif3, tcif4; 
    TestStatus TransferStatus;
    dma_ch1_cfg();
    dma_ch2_cfg();
    dma_ch3_cfg();
    dma_ch4_cfg();
    printf("CH CFG DONE\n");

    //DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel1, ENABLE); 

    tcif1 = DMA_GetFlagStatus(DMA1_FLAG_TC1);
    while (tcif1==0)
    {
        tcif1 = DMA_GetFlagStatus(DMA1_FLAG_TC1);
    }
    printf("tcif1:%d\n", tcif1);

    TransferStatus = Buffercmp(SRC_Const_Buffer,(uint32_t *)DMA_TEST_ADDR0, BufferSize); //Check Transfer by CPU access
    if (TransferStatus == FAILED) return 1;
    else printf("CH1 Verified\n");
    DMA_DeInit(DMA1_Channel1);
    DMA_ClearFlag(DMA1_FLAG_GL1);


    DMA_Cmd(DMA1_Channel2, ENABLE); //Transfer ch2 with 0x400 = DST; Prio_lvl = M
    tcif2 = DMA_GetFlagStatus(DMA1_FLAG_TC2);
    while (tcif2==0)
    {
        tcif2 = DMA_GetFlagStatus(DMA1_FLAG_TC2);
    }
    TransferStatus = Buffercmp((uint32_t *)DMA_TEST_ADDR1, (uint32_t *)DMA_TEST_ADDR0, BufferSize);
    if (TransferStatus == FAILED) return 1;
    else printf("CH2 Verified\n");
    DMA_DeInit(DMA1_Channel2);
    DMA_ClearFlag(DMA1_FLAG_GL2);


    DMA_Cmd(DMA1_Channel3, ENABLE); //Transfer ch3 with 0x500 = SRC; Prio_lvl = H
    tcif3 = DMA_GetFlagStatus(DMA1_FLAG_TC3);
    while (tcif3==0)
    {
        tcif3 = DMA_GetFlagStatus(DMA1_FLAG_TC3);
    }
    TransferStatus = Buffercmp((uint32_t *)DMA_TEST_ADDR2,(uint32_t *) DMA_TEST_ADDR1, BufferSize);
    if (TransferStatus == FAILED) return 1;
    else printf("CH3 Verified\n");
    DMA_ClearFlag(DMA1_FLAG_GL3);
    DMA_DeInit(DMA1_Channel3);


    DMA_Cmd(DMA1_Channel4, ENABLE); //Transfer ch4 with 0x600 = DST; Prio_lvl = V
    tcif4 = DMA_GetFlagStatus(DMA1_FLAG_TC4);
    while (tcif4==0)
    {
        tcif4 = DMA_GetFlagStatus(DMA1_FLAG_TC4);
    }
    TransferStatus = Buffercmp((uint32_t *)DMA_TEST_ADDR0,(uint32_t *) DMA_TEST_ADDR2, BufferSize);
    if (TransferStatus == FAILED) return 1;
    else printf("CH4 Verified\n");
    DMA_ClearFlag(DMA1_FLAG_GL4);
    DMA_DeInit(DMA1_Channel4);

    return 0;
}

int dma_ram(int argc, void *argv[])
{   
    dma_test();
    return 0;
}

