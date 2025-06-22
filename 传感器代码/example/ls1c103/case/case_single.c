//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "Config.h"
#include "ls1x_gpio.h"
#include "ls1c103_adc.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"

//This test uses regular channels only
static int err;

#define ADC1_DR_Address    ((uint32_t)(ADC_BASE + 0x4c))
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
vuint16_t ADCSingle[12];


extern unsigned int str2num(unsigned char *);
void GPIO_Configuration_ADC(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13;
                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

int adc_single(unsigned int chn, unsigned int times)
{
  uint8_t  rch[8] = { // regular channel sequence
                  ADC_Channel_0 ,  //0
                  ADC_Channel_1 ,  //1
                  ADC_Channel_2 ,  //2
                  ADC_Channel_4 ,  //3
                  ADC_Channel_6 ,  //4
                  ADC_Channel_7 ,  //5
                  ADC_Channel_8 ,  //6
                  ADC_Channel_9 ,  //7
                };

  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCSingle;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = times;
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
  ADC_InitStructure.ADC_NbrOfChannel = times;
  ADC_InitStructure.ADC_ClkDivider = 0x1;  //Loongson Feature  1:8MHz 3:4MHz
  ADC_InitStructure.ADC_JTrigMod = 2;    //Loongson Feature
  ADC_InitStructure.ADC_ADCEdge = 0;     //Loongson Feature
  ADC_InitStructure.ADC_OutPhaseSel = 2; //Loongson Feature
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel configuration */ 
  int i;
  for (i=0;i<times;i++)
  {
     ADC_RegularChannelConfig(ADC1, rch[chn-1], i+1, ADC_SampleTime_16Cycles);
  }

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  while (!DMA_GetFlagStatus(DMA1_FLAG_TC1));

  DMA_ClearFlag(DMA1_FLAG_GL1);

  for (i=0;i<times;i++)
  {
      printf("val of %d rconv: %d mv,0x%x\n", chn, (ADCSingle[i]*3300)>>12,ADCSingle[i]);
  }

  /* Deinitializes the ADC*/
  //ADC_DeInit(ADC1);

  return 0;
}

void adc_single_test (int argc, void *argv[])
{
    printf("\n");
    unsigned int chn;
    unsigned int times;
    if(argc == 2)
    {
       times = 1;
    }
    else if (argc == 3)
    {
       times = str2num(argv[2]);
       if (times==0 || times>12)
       {
           printf("\n times: 1-12 \n");
       }
    }
    else 
    {
       printf("\n usage: s <ch> <times>\r\n");
       printf("\n times: 1-12 \n");
    }
    chn = str2num(argv[1]);
    GPIO_Configuration_ADC();
    adc_single(chn, times);
}
