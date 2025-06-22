//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1x_gpio.h"
#include "test.h"
#include "ls1x_gpio.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"
#include "ls1c103_dcm_tim.h"
#include "Config.h"
static int err;

static dcm_motor_dir_t direction;     // dir
static uint16_t    dutyfactor;             // speed

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

static void GPIO_Configuration_DCM(void)
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

static void GPIO_Configuration_DCM2(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  AFIO_RemapConfig(AFIOA, GPIO_Pin_7 | GPIO_Pin_8, GPIO_FUNC_MAIN);
  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void TIM_DCM_CONFIG(){
  uint32_t tmpcr1;
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;
	
  tmpcr1 = DCM_TIM->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= (uint32_t)TIM_CR1_ARPE;
  DCM_TIM->CR1 = tmpcr1;
  /* Set the Autoreload value */
  DCM_TIM->ARR = (uint32_t)DCM_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  DCM_TIM->PSC = DCM_PRESCALER_COUNT - 1;
  DCM_TIM->RCR = 0;
  DCM_TIM->EGR = TIM_EGR_UG;

   /*PWM模式配置*/
  // 初始化通道 1 输出 PWM 
  DCM_TIM->CCER &= ~TIM_CCER_CC1E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM->CCMR1;
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
  DCM_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM->CCR1 = 0;
  /* Write to TIMx CCER */
  DCM_TIM->CCER = tmpccer;
  DCM_TIM->CCMR1 |= TIM_CCMR1_OC1PE;

  // 初始化通道 2 输出 PWM 
  DCM_TIM->CCER &= ~TIM_CCER_CC2E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM->CCMR1;
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
  DCM_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM->CCR2 = 0;
  /* Write to TIMx CCER */
  DCM_TIM->CCER = tmpccer;
  DCM_TIM->CCMR1 |= TIM_CCMR1_OC2PE;
		
  // 初始化通道 3 输出 PWM 
  DCM_TIM->CCER &= ~TIM_CCER_CC3E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM->CCMR2;
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
  DCM_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM->CCR3 = 0;
  /* Write to TIMx CCER */
  DCM_TIM->CCER = tmpccer;
  DCM_TIM->CCMR2 |= TIM_CCMR2_OC3PE;
	
  // 初始化通道 4 输出 PWM 
  DCM_TIM->CCER &= ~TIM_CCER_CC4E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM->CCMR2;
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
  DCM_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM->CCR4 = 0;
  /* Write to TIMx CCER */
  DCM_TIM->CCER = tmpccer;
  DCM_TIM->CCMR2 |= TIM_CCMR2_OC4PE;

  DCM_TIM->CCER |= (uint32_t)(TIM_CCER_CC1E);
  DCM_TIM->CCER |= (uint32_t)(TIM_CCER_CC2E);
  DCM_TIM->CCER |= (uint32_t)(TIM_CCER_CC3E);
  DCM_TIM->CCER |= (uint32_t)(TIM_CCER_CC4E);
}

static void TIM_DCM_CONFIG2(){
  uint32_t tmpcr1;
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;
	
  tmpcr1 = DCM_TIM2->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= (uint32_t)TIM_CR1_ARPE;
  DCM_TIM2->CR1 = tmpcr1;
  /* Set the Autoreload value */
  DCM_TIM2->ARR = (uint32_t)DCM_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  DCM_TIM2->PSC = DCM_PRESCALER_COUNT - 1;
  DCM_TIM2->EGR = TIM_EGR_UG;

   /*PWM模式配置*/
  // 初始化通道 1 输出 PWM 
  DCM_TIM2->CCER &= ~TIM_CCER_CC1E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM2->CCMR1;
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
  DCM_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM2->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM2->CCR1 = 0;
  /* Write to TIMx CCER */
  DCM_TIM2->CCER = tmpccer;
  DCM_TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

  // 初始化通道 2 输出 PWM 
  DCM_TIM2->CCER &= ~TIM_CCER_CC2E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM2->CCMR1;
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
  DCM_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM2->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM2->CCR2 = 0;
  /* Write to TIMx CCER */
  DCM_TIM2->CCER = tmpccer;
  DCM_TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
		
  // 初始化通道 3 输出 PWM 
  DCM_TIM2->CCER &= ~TIM_CCER_CC3E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM2->CCMR2;
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
  DCM_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM2->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM2->CCR3 = 0;
  /* Write to TIMx CCER */
  DCM_TIM2->CCER = tmpccer;
  DCM_TIM2->CCMR2 |= TIM_CCMR2_OC3PE;
	
  // 初始化通道 4 输出 PWM 
  DCM_TIM2->CCER &= ~TIM_CCER_CC4E;
  /* Get the TIMx CCER register value */
  tmpccer = DCM_TIM2->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  DCM_TIM2->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = DCM_TIM2->CCMR2;
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
  DCM_TIM2->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  DCM_TIM2->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  DCM_TIM2->CCR4 = 0;
  /* Write to TIMx CCER */
  DCM_TIM2->CCER = tmpccer;
  DCM_TIM2->CCMR2 |= TIM_CCMR2_OC4PE;

  DCM_TIM2->CCER |= (uint32_t)(TIM_CCER_CC1E);
  DCM_TIM2->CCER |= (uint32_t)(TIM_CCER_CC2E);
  DCM_TIM2->CCER |= (uint32_t)(TIM_CCER_CC3E);
  DCM_TIM2->CCER |= (uint32_t)(TIM_CCER_CC4E);
}

static void set_motor_speed(uint32_t channel)
{
  
  if (direction == DCM_MOTOR_FWD)
  {   
    switch(channel){
      case 1:  DCM_TIM->CCR2 = 0;  DCM_TIM->CCR1 = dutyfactor; break; 
      case 2:  DCM_TIM->CCR4 = 0;  DCM_TIM->CCR3 = dutyfactor; break; 
      case 3:  DCM_TIM2->CCR2 = 0;  DCM_TIM2->CCR1 = dutyfactor; break; 
      case 4:  DCM_TIM2->CCR4 = 0;  DCM_TIM2->CCR3 = dutyfactor; break; 
    }
  }
  else
  { 
    switch(channel){
      case 1:  DCM_TIM->CCR1 = 0;  DCM_TIM->CCR2 = dutyfactor; break; 
      case 2:  DCM_TIM->CCR3 = 0;  DCM_TIM->CCR4 = dutyfactor; break; 
      case 3:  DCM_TIM2->CCR1 = 0;  DCM_TIM2->CCR2 = dutyfactor; break; 
      case 4:  DCM_TIM2->CCR3 = 0;  DCM_TIM2->CCR4 = dutyfactor; break; 
    }
  }
}

extern void irq_motor_init(int index);
static int tim_dcm_test(void)
{
  printf("\n\rDCM TEST\n\r");
  GPIO_Configuration();
	 
  GPIO_Configuration_DCM();
  GPIO_Configuration_DCM2();
  TIM_DCM_CONFIG();
  TIM_DCM_CONFIG2();
  dcm_mode = DCM_FOUR;
  irq_motor_init(0);

  while(1){
    if(scan_key(KEY0)){
        printf("start\n\r");
        DCM_TIM->BDTR|=(TIM_BDTR_MOE);
        DCM_TIM->CR1|=TIM_CR1_CEN;
        DCM_TIM2->CR1|=TIM_CR1_CEN;
        break;
    }   
  }
    return 0;
}

int tim_dcm(int argc, void *argv[])
{   
    tim_dcm_test();
    return 0;
}


int speed_set_dcm(int argc, void *argv[])
{   
    if (argc != 4)
    {
        printf("\n usage: speed set <channel:1-4> <dir:0/1> <speed:0-900>\n");
        return 1;
    }

    uint32_t  channel = str2num(argv[1]);
    uint32_t  dir = str2num(argv[2]);
    uint32_t  speed = str2num(argv[3]);

    if(dcm_mode != DCM_FOUR){
    	printf("\nNOT IN FOUR MOTOR MODE!\n\r");
	return 0;
    }

    if(speed > 900){
       speed = 900;
    }
    else if(speed < 0){
       speed = 0;
    }

    if(dir == 0){
        direction  = DCM_MOTOR_FWD;
      }
       else{
        direction  = DCM_MOTOR_REV;
       }

	dutyfactor = speed;
	
  if (direction == DCM_MOTOR_FWD)
  {   
    switch(channel){
      case 1:  DCM_TIM->CCR2 = 0;  DCM_TIM->CCR1 = dutyfactor; break; 
      case 2:  DCM_TIM->CCR4 = 0;  DCM_TIM->CCR3 = dutyfactor; break; 
      case 3:  DCM_TIM2->CCR2 = 0;  DCM_TIM2->CCR1 = dutyfactor; break; 
      case 4:  DCM_TIM2->CCR4 = 0;  DCM_TIM2->CCR3 = dutyfactor; break; 
    }
  }
  else
  { 
    switch(channel){
      case 1:  DCM_TIM->CCR1 = 0;  DCM_TIM->CCR2 = dutyfactor; break; 
      case 2:  DCM_TIM->CCR3 = 0;  DCM_TIM->CCR4 = dutyfactor; break; 
      case 3:  DCM_TIM2->CCR1 = 0;  DCM_TIM2->CCR2 = dutyfactor; break; 
      case 4:  DCM_TIM2->CCR3 = 0;  DCM_TIM2->CCR4 = dutyfactor; break; 
    }
  }

    return 0;
}

int speed_keyset_dcm(int argc, void *argv[])
{   
    if (argc != 2)
    {
        printf("\n usage: speed set <channel:1-4>\n");
        return 1;
    }

    uint32_t  channel = str2num(argv[1]);
    volatile int ChannelPulse = 0;
    int i = 0;
    direction  = DCM_MOTOR_FWD;     // dir
    dutyfactor = 0;

    if(dcm_mode != DCM_FOUR){
    	printf("\nNOT IN FOUR MOTOR MODE!\n\r");
	return 0;
    }

    while(1){
      if(scan_key(KEY0)){
         ChannelPulse += 20;
        if(ChannelPulse > 900){
	       ChannelPulse = 900;
        }
        if(ChannelPulse >= 0){
         dutyfactor = ChannelPulse;
         direction  = DCM_MOTOR_FWD;
        }
        else{
	       dutyfactor = -ChannelPulse;
         direction  = DCM_MOTOR_REV;
        }
        set_motor_speed(channel);
      }
      else if (scan_key(KEY1)){ 
        ChannelPulse -= 20;
        if(ChannelPulse < -900){
	        ChannelPulse = -900;
        }
        if(ChannelPulse >= 0){
	        dutyfactor = ChannelPulse;
          direction  = DCM_MOTOR_FWD;
        }
        else{
	        dutyfactor = -ChannelPulse;
          direction  = DCM_MOTOR_REV;
        }
        set_motor_speed(channel);
      }


      
	
      char c; 
      c = x_getchar();
      if (c == 0x3)    break;
      i++;
      if(i==100000){
	    i = 0;
	    printf(".");
	  }
  }


  return 0;
}  

int tim_set_dcm(int argc, void *argv[])
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
      case 1:  DCM_TIM->PSC = psc_in;  DCM_TIM->ARR = arr_in; break; 
      case 2:  TIM2->PSC = psc_in;  TIM2->ARR = arr_in; break; 
    }
    return 0;
}
 




