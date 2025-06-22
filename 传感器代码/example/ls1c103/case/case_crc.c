//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "Config.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"
#include "ls1c103_crc.h"

static int err;

ucint32_t crc_SRC_Const_Buffer[32        ]= {0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                        0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                        0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                        0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                        0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                        0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                        0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                        0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};



//Test Case by Loongson
static int CRCcmp(ucint32_t* pBuffer, uint16_t BufferLength)
{
  unsigned rem = 0xffffffff;
  while(BufferLength--)
  {
    rem = crc32(*pBuffer, rem);
    pBuffer++;
  }
  printf("CRC hard:%x\n\r",CRC_DR);
  printf("CRC soft:%x\n\r",rem);
  if( CRC_DR != rem) return 1;

  return 0;
}

static void crc_dma_cfg()
{
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(CRC_BASE); // CRC_DR, offset 0x0
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)crc_SRC_Const_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 32;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
}


int crc_test(void)
{
    CRC_RST = 1; // RST

    uint32_t tcif1; 

    crc_dma_cfg();
    printf("DMA CH1 CFG\n");

    DMA_Cmd(DMA1_Channel1, ENABLE); //Transferred ch1 with DST = SRC;

    tcif1 = DMA_GetFlagStatus(DMA1_FLAG_TC1);
    while (tcif1==0)
    {
        tcif1 = DMA_GetFlagStatus(DMA1_FLAG_TC1);
    }

    int r = CRCcmp(crc_SRC_Const_Buffer, 32); //Check Transfer by CPU access
    if (r) return 1;
    else printf("CRC Verified\n");

    DMA_ClearFlag(DMA1_FLAG_GL1);

    tcif1 = DMA_GetFlagStatus(DMA1_FLAG_TC1);

    DMA_DeInit(DMA1_Channel1);
  return 0;
}

int crc (int argc, void *argv[])
{   
    crc_test();
    return 0;
}

