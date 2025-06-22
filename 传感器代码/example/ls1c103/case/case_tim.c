#include "test.h"
//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"
#include "ls1x_gpio.h"
#include "Config.h"

static int err;


//user code
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

static uint16_t CCR1_Val = (uint16_t)2;
static uint16_t CCR2_Val = (uint16_t)4;
static uint16_t CCR3_Val = (uint16_t)6;
static uint16_t CCR4_Val = (uint16_t)8;

static void GPIO_Configuration_atim(void);
static void GPIO_Configuration_btim(void);

extern INT8U x_getchar(void);
int TIM1_pwmout(){
  
   /* TIM1 Configuration ---------------------------------------------------
   Generates 7 PWM signals with 4 different duty cycles:
   TIM1CLK = 72 MHz, Prescaler = 0, TIM1 counter clock = 72 MHz
   TIM1 frequency = TIM1CLK/(TIM1_Period + 1) = 17.57 KHz
  - TIM1 Channel1 & Channel1N duty cycle = TIM1->CCR1 / (TIM1_Period + 1) = 50% 
  - TIM1 Channel2 & Channel2N duty cycle = TIM1->CCR2 / (TIM1_Period + 1) = 37.5% 
  - TIM1 Channel3 & Channel3N duty cycle = TIM1->CCR3 / (TIM1_Period + 1) = 25%
  - TIM1 Channel4 duty cycle = TIM1->CCR4 / (TIM1_Period + 1) = 12.5% 
  ----------------------------------------------------------------------- */
  // printf("tim1:%x",&TIM1->CR1);
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 4095;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  return 0;
}

int TIM2_pwm(){
  /* ---------------------------------------------------------------
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 36 MHz, Prescaler = 4, TIM2 counter clock = 7.2 MHz
    CC1 update rate = TIM2 counter clock / CCR1_Val = 146.48 Hz
    CC2 update rate = TIM2 counter clock / CCR2_Val = 219.7 Hz
    CC3 update rate = TIM2 counter clock / CCR3_Val = 439.4 Hz
    CC4 update rate = TIM2 counter clock / CCR4_Val =  878.9 Hz
  --------------------------------------------------------------- */
 
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 9;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM2, 0, TIM_PSCReloadMode_Immediate);
  TIM_OCStructInit(&TIM_OCInitStructure);
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
  TIM_CtrlPWMOutputs(TIM2, ENABLE);

  /* TIM IT enable */
  // TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);

}

int TIM6_timebase(){
  
   /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 4000;
  TIM_TimeBaseStructure.TIM_Prescaler = 10667;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
   /* TIM IT enable */
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM6, ENABLE);

}

int TIM1_timebase(){
  
   /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 3000; //3000*1ms = 3s
  TIM_TimeBaseStructure.TIM_Prescaler = 8000;   // clkbus=8M 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
   /* TIM IT enable */
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM1, ENABLE);

}

int TIM1_SYNC(){
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  /* TIM2 Peripheral Configuration ----------------------------------------*/
  /* TIM2 Configuration: PWM1 Mode */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 20;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 2;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 3;
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = 4;
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
  
  /* Master Mode selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* TIM2 counter enable */
  TIM_Cmd(TIM2, ENABLE);
 
  /* TIM1 Peripheral Configuration ----------------------------------------*/
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 255;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 4;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 127;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  /* Automatic Output enable, Break, dead time and lock configuration*/
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStructure.TIM_DeadTime = 5;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

   /* Slave Mode selection: TIM4 */
  TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Trigger);
  TIM_SelectInputTrigger(TIM1, TIM_TS_ITR0);

  

  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

int btim_int(){
  printf("btim  int test begin\n");
  printf("tim6:%x\n",&TIM6->CR1);
  TIM6_timebase();
  INT_EN = 0x1;
  while(1){
    char c; 
    c = x_getchar();
    if (c == 0x3)  {
  	INT_EN &= (~0x1);
  	TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
  	TIM_Cmd(TIM6, DISABLE);
	break;
    }
  }
  printf("dier:%x,sr:%x\n",TIM6->DIER,TIM6->SR);
  
  return 0;
}

int atim_int(){
  printf("atim  int test begin\n");
  printf("tim1:%x\n",&TIM1->CR1);

  TIM1_timebase();
  while(1){
    char c; 
    c = x_getchar();
    if (c == 0x3)  {
  	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
  	TIM_Cmd(TIM1, DISABLE);
	break;
    }
  }
  printf("dier:%x,sr:%x\n",TIM1->DIER,TIM1->SR);
  
  return 0;
}

int tim_test(){
  GPIO_Configuration_atim();
  GPIO_Configuration_btim();
  printf("tim begin\n");
  printf("tim1:%x\n",&TIM1->CR1);
  printf("tim2:%x\n",&TIM2->CR1);
  printf("tim6:%x\n",&TIM6->CR1);
  //TIM1_pwmout();
  //printf("ccr:%d %d %d %d\n",TIM1->CCR1,TIM1->CCR2,TIM1->CCR3,TIM1->CCR4);

   TIM2_pwm();
  //printf("dier:%x,sr:%x\n",TIM2->DIER,TIM2->SR);

  //TIM1_timebase();
 // printf("dier:%x,sr:%x\n",TIM1->DIER,TIM1->SR);
  //TIM6_timebase();
  //printf("dier:%x,sr:%x\n",TIM6->DIER,TIM6->SR);

  //TIM1_SYNC();
  //printf("ccr:%d %d %d %d\n",TIM1->CCR1,TIM1->CCR2,TIM1->CCR3,TIM1->CCR4);

  
  
  return 0;
}

static void GPIO_Configuration_atim(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  AFIO_PORTA |= 0x0C000000;
  /* GPIOA Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_btim(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  AFIO_PORTA |= 0x00004000;
  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


