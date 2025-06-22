//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "Config.h"
#include "ls1c103_adc.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"

//This test uses regular channels only
static int err;

#define ADC1_DR_Address    ((uint32_t)(ADC_BASE + 0x4c))
#define REG_CH  6
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
vuint16_t ADCRegular[REG_CH];
    
int adc_reg_only_test(void)
{
  uint8_t  rch[6] = { // regular channel sequence
                  ADC_Channel_0 ,  //0
                  ADC_Channel_1 ,  //1
                  ADC_Channel_7 ,  //2
                  ADC_Channel_0 ,  //3
                  ADC_Channel_1 ,  //4
                  ADC_Channel_7 ,  //5
                };

  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCRegular;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = REG_CH;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Clear DMA1 FLAG of channel1 */
  DMA_ClearFlag(DMA1_FLAG_GL1);
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
     
  /* Deinitializes the ADC*/
  ADC_DeInit(ADC1);

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = REG_CH;
  ADC_InitStructure.ADC_ClkDivider = 0x1;  //Loongson Feature
  ADC_InitStructure.ADC_JTrigMod = 2;    //Loongson Feature
  ADC_InitStructure.ADC_ADCEdge = 0;     //Loongson Feature
  ADC_InitStructure.ADC_OutPhaseSel = 2; //Loongson Feature
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel configuration */ 
  ADC_RegularChannelConfig(ADC1, rch[0], 1, ADC_SampleTime_16Cycles);
  ADC_RegularChannelConfig(ADC1, rch[1], 2, ADC_SampleTime_16Cycles);
  ADC_RegularChannelConfig(ADC1, rch[2], 3, ADC_SampleTime_16Cycles);
  ADC_RegularChannelConfig(ADC1, rch[3], 4, ADC_SampleTime_16Cycles);
  ADC_RegularChannelConfig(ADC1, rch[4], 5, ADC_SampleTime_16Cycles);
  ADC_RegularChannelConfig(ADC1, rch[5], 6, ADC_SampleTime_16Cycles);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  while (!DMA_GetFlagStatus(DMA1_FLAG_TC1));

  DMA_ClearFlag(DMA1_FLAG_GL1);

  int i;

  for (i=0;i<REG_CH;i++)
  {
      printf("val of %d rconv: %d mv\n", i+1, (ADCRegular[i]*3300)>>12);
  }

  /* Deinitializes the ADC*/
  //ADC_DeInit(ADC1);

  return 0;
}

void regular_test (int argc, void *argv[])
{
    printf("\n");
    adc_reg_only_test();
}
