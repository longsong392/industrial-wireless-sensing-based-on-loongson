//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1x_gpio.h"
#include "test.h"
#include "Config.h"
#include "ls1x_gpio.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"

static int err;

#define KEY0 (0x00)
#define KEY1 (0x01)
#define STEER_TIM                TIM1
#define STEER_TIM2               TIM2
#define STEER_PERIOD_COUNT        (10158)
#define STEER_PRESCALER_COUNT     (21)

extern INT8U x_getchar(void);
extern unsigned int str2num(unsigned char *);
static uint8_t scan_key(uint32_t key){
  uint16_t keyuse;
  if(key == KEY1){
    keyuse = GPIO_Pin_13;
  }
  else{
    keyuse = GPIO_Pin_12;
  }
  uint8_t sflag = GPIO_ReadInputDataBit(GPIOA, keyuse);
  if(sflag==0){
	while(sflag==0){
	       sflag = GPIO_ReadInputDataBit(GPIOA, keyuse);
	}
	return 1;
  }
  else{
	return 0;
  }
 
}

static void GPIO_Configuration(void)
{
  AFIO_RemapConfig(AFIOA, GPIO_Pin_12 | GPIO_Pin_13, GPIO_FUNC_GPIO);
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

static void GPIO_Configuration_Steer(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  AFIO_RemapConfig(AFIOA, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4, GPIO_FUNC_MAIN);
  AFIO_RemapConfig(AFIOA, GPIO_Pin_13, GPIO_FUNC_SECOND);

  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_Steer2(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  AFIO_RemapConfig(AFIOA, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, GPIO_FUNC_MAIN);
  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void TIM_STEER_CONFIG(){
  uint32_t tmpcr1;
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;
	
  tmpcr1 = STEER_TIM->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= (uint32_t)TIM_CR1_ARPE;
  STEER_TIM->CR1 = tmpcr1;
  /* Set the Autoreload value */
  STEER_TIM->ARR = (uint32_t)STEER_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  STEER_TIM->PSC = STEER_PRESCALER_COUNT - 1;
  STEER_TIM->RCR = 0;
  STEER_TIM->EGR = TIM_EGR_UG;

   /*PWM CH1*/
  STEER_TIM->CCER &= ~TIM_CCER_CC1E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM->CCMR1;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCMode_PWM1;
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= TIM_OCPolarity_High;
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS1;
  /* Set the Output Idle state */
  tmpcr2 |= TIM_OCIdleState_Reset;
  /* Write to TIMx CR2 */
  STEER_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM->CCR1 = 762;
  /* Write to TIMx CCER */
  STEER_TIM->CCER = tmpccer;
  STEER_TIM->CCMR1 |= TIM_CCMR1_OC1PE;

  // PWM CH2
  STEER_TIM->CCER &= ~TIM_CCER_CC2E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM->CCMR1;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
  /* Select the Output Compare Mode */
  tmpccmrx |= (TIM_OCMode_PWM1 << 8);
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC2P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 4);
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS2;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 2);
  /* Write to TIMx CR2 */
  STEER_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM->CCR2 = 762;
  /* Write to TIMx CCER */
  STEER_TIM->CCER = tmpccer;
  STEER_TIM->CCMR1 |= TIM_CCMR1_OC2PE;
		
  // PWM CH3
  STEER_TIM->CCER &= ~TIM_CCER_CC3E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM->CCMR2;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCMode_PWM1;
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC3P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 8);
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS3;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 4);
  /* Write to TIMx CR2 */
  STEER_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM->CCR3 = 762;
  /* Write to TIMx CCER */
  STEER_TIM->CCER = tmpccer;
  STEER_TIM->CCMR2 |= TIM_CCMR2_OC3PE;
	
  // PWM CH4
  STEER_TIM->CCER &= ~TIM_CCER_CC4E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM->CCMR2;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR2_OC4M;
  tmpccmrx &= ~TIM_CCMR2_CC4S;
  /* Select the Output Compare Mode */
  tmpccmrx |= (TIM_OCMode_PWM1 << 8);
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC4P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 12);
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS4;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 8);
  /* Write to TIMx CR2 */
  STEER_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM->CCR4 = 762;
  /* Write to TIMx CCER */
  STEER_TIM->CCER = tmpccer;
  STEER_TIM->CCMR2 |= TIM_CCMR2_OC4PE;


  STEER_TIM->CCER |= (uint32_t)(TIM_CCER_CC1E);
  STEER_TIM->CCER |= (uint32_t)(TIM_CCER_CC2E);
  STEER_TIM->CCER |= (uint32_t)(TIM_CCER_CC3E);
  STEER_TIM->CCER |= (uint32_t)(TIM_CCER_CC4E);

}

static void TIM_STEER_CONFIG2(){
  uint32_t tmpcr1;
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;
	
  tmpcr1 = STEER_TIM2->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= (uint32_t)TIM_CR1_ARPE;
  STEER_TIM2->CR1 = tmpcr1;
  /* Set the Autoreload value */
  STEER_TIM2->ARR = (uint32_t)STEER_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  STEER_TIM2->PSC = STEER_PRESCALER_COUNT - 1;
  STEER_TIM2->EGR = TIM_EGR_UG;


  STEER_TIM2->CCER &= ~TIM_CCER_CC1E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM2->CCMR1;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCMode_PWM1;
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= TIM_OCPolarity_High;
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS1;
  /* Set the Output Idle state */
  tmpcr2 |= TIM_OCIdleState_Reset;
  /* Write to TIMx CR2 */
  STEER_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM2->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM2->CCR1 = 762;
  /* Write to TIMx CCER */
  STEER_TIM2->CCER = tmpccer;
  STEER_TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

  STEER_TIM2->CCER &= ~TIM_CCER_CC2E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM2->CCMR1;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
  /* Select the Output Compare Mode */
  tmpccmrx |= (TIM_OCMode_PWM1 << 8);
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC2P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 4);
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS2;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 2);
  /* Write to TIMx CR2 */
  STEER_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM2->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM2->CCR2 = 762;
  /* Write to TIMx CCER */
  STEER_TIM2->CCER = tmpccer;
  STEER_TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
		
  STEER_TIM2->CCER &= ~TIM_CCER_CC3E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM2->CCMR2;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCMode_PWM1;
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC3P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 8);
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS3;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 4);
  /* Write to TIMx CR2 */
  STEER_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM2->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM2->CCR3 = 762;
  /* Write to TIMx CCER */
  STEER_TIM2->CCER = tmpccer;
  STEER_TIM2->CCMR2 |= TIM_CCMR2_OC3PE;

  STEER_TIM2->CCER &= ~TIM_CCER_CC4E;
  /* Get the TIMx CCER register value */
  tmpccer = STEER_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  STEER_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = STEER_TIM2->CCMR2;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR2_OC4M;
  tmpccmrx &= ~TIM_CCMR2_CC4S;
  /* Select the Output Compare Mode */
  tmpccmrx |= (TIM_OCMode_PWM1 << 8);
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC4P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 12);
  /* Reset the Output Compare IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS4;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 8);
  /* Write to TIMx CR2 */
  STEER_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  STEER_TIM2->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  STEER_TIM2->CCR4 = 762;
  /* Write to TIMx CCER */
  STEER_TIM2->CCER = tmpccer;
  STEER_TIM2->CCMR2 |= TIM_CCMR2_OC4PE;


  STEER_TIM2->CCER |= (uint32_t)(TIM_CCER_CC1E);
  STEER_TIM2->CCER |= (uint32_t)(TIM_CCER_CC2E);
  STEER_TIM2->CCER |= (uint32_t)(TIM_CCER_CC3E);
  STEER_TIM2->CCER |= (uint32_t)(TIM_CCER_CC4E);

}

static int tim_steer_test(void)
{

  printf("\n\rSTEER TEST\n\r");
	 
  GPIO_Configuration();
  GPIO_Configuration_Steer();
  GPIO_Configuration_Steer2();
  TIM_STEER_CONFIG();
  TIM_STEER_CONFIG2();
  
  STEER_TIM->BDTR|=(TIM_BDTR_MOE);
  STEER_TIM->CR1|=TIM_CR1_CEN;
  STEER_TIM2->CR1|=TIM_CR1_CEN;

  return 0;
}

int tim_steer(int argc, void *argv[])
{   
    tim_steer_test();
    return 0;
}

int angle_set(int argc, void *argv[])
{   
    if (argc != 3)
    {
        printf("\n usage: angle set <channel:1-8> <angle:0-180>\n");
        return 1;
    }

    uint32_t  channel = str2num(argv[1]);
    uint32_t  angle = str2num(argv[2]);
    uint32_t  data;
    if(angle > 180){
       angle = 180;
    }
    else if(angle < 0){
       angle = 0;
    }

    data = (254 + ((1270000-254000)/180*angle)/1000);
    //data = (uint32_t)(254.0 + ((1270.0-254.0)/180.0*angle));

    switch(channel){
      case 1:  STEER_TIM->CCR1 = data; break; 
      case 2:  STEER_TIM->CCR2 = data; break; 
      case 3:  STEER_TIM->CCR3 = data; break; 
      case 4:  STEER_TIM->CCR4 = data; break; 
      case 5:  STEER_TIM2->CCR1 = data; break; 
      case 6:  STEER_TIM2->CCR2 = data; break; 
      case 7:  STEER_TIM2->CCR3 = data; break; 
      case 8:  STEER_TIM2->CCR4 = data; break; 
    }
    return 0;
}

int angle_keyset(int argc, void *argv[])
{   
    if (argc != 2)
    {
        printf("\n usage: angle keyset <channel:1-8>\n");
        return 1;
    }

    uint32_t  channel = str2num(argv[1]);
    volatile uint32_t ChannelPulse = 762;
    int i = 0;

    switch(channel){
      case 1:  ChannelPulse = STEER_TIM->CCR1; break; 
      case 2:  ChannelPulse = STEER_TIM->CCR2; break; 
      case 3:  ChannelPulse = STEER_TIM->CCR3; break; 
      case 4:  ChannelPulse = STEER_TIM->CCR4; break; 
      case 5:  ChannelPulse = STEER_TIM2->CCR1; break; 
      case 6:  ChannelPulse = STEER_TIM2->CCR2; break; 
      case 7:  ChannelPulse = STEER_TIM2->CCR3; break; 
      case 8:  ChannelPulse = STEER_TIM2->CCR4; break; 
    }

    while(1){
      char c; 
      c = x_getchar();
      if (c == 0x3)    break;

      if(scan_key(KEY0)){
        ChannelPulse += 20;
         if(ChannelPulse > 1270){
	         ChannelPulse = 1270;
         }
      }
      else if (scan_key(KEY1)){ 
         ChannelPulse -= 20;
        if(ChannelPulse < 254){
	       ChannelPulse = 254;
        }
      }

    switch(channel){
      case 1:  STEER_TIM->CCR1 = ChannelPulse; break; 
      case 2:  STEER_TIM->CCR2 = ChannelPulse; break; 
      case 3:  STEER_TIM->CCR3 = ChannelPulse; break; 
      case 4:  STEER_TIM->CCR4 = ChannelPulse; break; 
      case 5:  STEER_TIM2->CCR1 = ChannelPulse; break; 
      case 6:  STEER_TIM2->CCR2 = ChannelPulse; break; 
      case 7:  STEER_TIM2->CCR3 = ChannelPulse; break; 
      case 8:  STEER_TIM2->CCR4 = ChannelPulse; break; 
    }

    i++;
	  if(i==100000){
	    i = 0;
	    printf(".");
	  }

  }   
    return 0;
}

int tim_set_steer(int argc, void *argv[])
{   
    if (argc != 4)
    {
        printf("\n usage: tim set <0:atim/1:gtim> <psc> <arr>\n");
        return 1;
    }

    uint32_t  tim_choice = str2num(argv[1]);
    uint32_t  psc_in = str2num(argv[2]);
    uint32_t  arr_in = str2num(argv[2]);
    uint32_t  data;


    switch(tim_choice){
      case 1:  STEER_TIM->PSC = psc_in;  STEER_TIM->ARR = arr_in; break; 
      case 2:  STEER_TIM2->PSC = psc_in;  STEER_TIM2->ARR = arr_in; break; 
    }
    return 0;
}
