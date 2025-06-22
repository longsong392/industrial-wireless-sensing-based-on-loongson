//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "Config.h"
#include "ls1c103_adc.h"
#include "ls1x_gpio.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"


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
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |
                                GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11|
                                GPIO_Pin_12| GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  if( *(volatile unsigned *)(PMU_BASE + PMU_SrProt) & 0x1) {
      //printf("spi boot, GPIOB6/7 stay\n");
  } else {
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
  }
}

#define ADC1_DR_Address    ((uint32_t)(ADC_BASE + 0x4c))
#define INTTEST_CH  12
ADC_InitTypeDef           ADC_InitStructure;
DMA_InitTypeDef           DMA_InitStructure;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
TIM_OCInitTypeDef         TIM_OCInitStructure;
vuint16_t ADCINTValue[INTTEST_CH];

static void TIM1_CONFIG(){
  /* TIM1 configuration ------------------------------------------------------*/ 
  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00FF;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned2;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  /* TIM1 channel1-4 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  
  TIM_OCInitStructure.TIM_Pulse = 0x48CF; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;         
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 0x8F6F;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 0xACCF;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 0xE59F;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  
  //TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

}

static void TIM2_CONFIG(){
  /* TIM2 configuration ------------------------------------------------------*/ 
  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xB1FF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00FF;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  /* TIM2 channel1-2 configuration in PWM mode */
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = 0x438F; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 0x819F;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  //TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  
}


static void DMA_CONFIG(){
  /* DMA1 Channel1 Configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCINTValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = INTTEST_CH;
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
}

static void ADC_CONFIG(){
  uint8_t  rch[12] = { // regular channel sequence
                  ADC_Channel_0 ,  //0
                  ADC_Channel_1 ,  //1
                  ADC_Channel_2 ,  //2
                  ADC_Channel_4 ,  //3
                  ADC_Channel_6 ,  //4
                  ADC_Channel_7 ,  //5
                  ADC_Channel_8 ,  //6
                  ADC_Channel_9 ,  //7
                  ADC_Channel_10,  //0
                  ADC_Channel_11,  //1
                  ADC_Channel_3 ,  //2
                  ADC_Channel_5    //3
                };
  uint8_t  jch[4]  = { // inject channel sequence
                  ADC_Channel_5,  //0
                  ADC_Channel_6,  //1
                  ADC_Channel_7 ,  //2
                  ADC_Channel_8    //3
                };

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

  /* Deinitializes the ADC*/
  ADC_DeInit(ADC1);

  ADC_StructInit(&ADC_InitStructure);
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = INTTEST_CH;

  ADC_InitStructure.ADC_ClkDivider = 3;  //Loongson Feature
  ADC_InitStructure.ADC_OutPhaseSel = 0;  //Loongson Feature
  ADC_InitStructure.ADC_JTrigMod = 2;    //Loongson Feature
  ADC_InitStructure.ADC_ADCEdge = 1;    //Loongson Feature
  
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel configuration */ 
  ADC_RegularChannelConfig(ADC1, rch[0] , 1 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[1] , 2 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[2] , 3 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[3] , 4 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[4] , 5 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[5] , 6 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[6] , 7 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[7] , 8 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[8] , 9 , ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[9] , 10, ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[10], 11, ADC_SampleTime_1Cycles);
  ADC_RegularChannelConfig(ADC1, rch[11], 12, ADC_SampleTime_1Cycles);

  /* Set injected sequencer length */
  ADC_InjectedSequencerLengthConfig(ADC1, 4);
  /* ADC1 injected channel Configuration */ 
  ADC_InjectedChannelConfig(ADC1, jch[0], 1, ADC_SampleTime_1Cycles);
  ADC_InjectedChannelConfig(ADC1, jch[1], 2, ADC_SampleTime_1Cycles);
  ADC_InjectedChannelConfig(ADC1, jch[2], 3, ADC_SampleTime_1Cycles);
  ADC_InjectedChannelConfig(ADC1, jch[3], 4, ADC_SampleTime_1Cycles);
  /* ADC1 injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_TRGO);
  //ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

  /* Disable automatic injected conversion start after regular one */
  ADC_AutoInjectedConvCmd(ADC1, DISABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 external trigger */ 
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);
  ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

  /* Enable discontinuous mode for the selected ADC regular group channel */
  ADC_DiscModeCmd(ADC1, ENABLE);
  ADC_DiscModeChannelCountConfig(ADC1, 3);

  /* Enable JEOC interupt */
  ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

int tim_int_test(void)
{
  uint32_t tmpreg0 = 0;
  uint16_t tmpreg[4] = {0, 0, 0, 0};

  TIM2_CONFIG();
  TIM1_CONFIG();
  DMA_CONFIG();
  ADC_CONFIG();

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  
  
  /* TIM2 counter enable */
  TIM_Cmd(TIM2, ENABLE);

  

  return 0;
}

int tim_int (int argc, void *argv[])
{   
    tim_int_test();
    return 0;
}

