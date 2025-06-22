#include "Config.h"
#include "ls1x.h"
#include "foc_tim.h"
#include "test.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"
#include "ls1x_gpio.h"

/* 定时器溢出次数 */
volatile int Foc_Encoder_Overflow_Count = 0;
motor_rotate_t motor_drive = {0};    // 定义电机驱动管理结构体

static motor_dir_t direction  = MOTOR_FWD;     // 记录方向
static uint32_t    dutyfactor = 0;             // 记录占空比
static uint32_t    is_motor_en = 0;             // 电机使能
static uint32_t    bldcm_pulse = 0;

static void GPIO_Configuration_Foc(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  AFIO_RemapConfig(AFIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5, GPIO_FUNC_MAIN);

  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOA Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_Encoder(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  AFIO_RemapConfig(AFIOA, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, GPIO_FUNC_MAIN);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_EN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  AFIO_PORTA &= ~((uint32_t)0x0C000000);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_13);
}

static void TIM_MOTOR_CONFIG(){
  uint32_t tmpcr1;
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;
	
  tmpcr1 = MOTOR_TIM->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= TIM_CKD_DIV1;
  tmpcr1 |= TIM_CR1_ARPE;
  MOTOR_TIM->CR1 = tmpcr1;
  /* Set the Autoreload value */
  MOTOR_TIM->ARR = (uint32_t)PWM_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  MOTOR_TIM->PSC = PWM_PRESCALER_COUNT - 1;
  MOTOR_TIM->RCR = 0;
  MOTOR_TIM->EGR = TIM_EGR_UG;

   /*PWM模式配置*/
  //配置为PWM模式1
  // 初始化通道 1 输出 PWM 
  MOTOR_TIM->CCER &= ~TIM_CCER_CC1E;
  /* Get the TIMx CCER register value */
  tmpccer = MOTOR_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  MOTOR_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = MOTOR_TIM->CCMR1;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCMode_PWM1;
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= TIM_OCPolarity_High;
  /* Reset the Output N Polarity level */
  tmpccer &= ~TIM_CCER_CC1NP;
  /* Set the Output N Polarity */
  tmpccer |= TIM_OCNPolarity_High;
  /* Reset the Output N State */
  tmpccer &= ~TIM_CCER_CC1NE;
  /* Reset the Output Compare and Output Compare N IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS1;
  tmpcr2 &= ~TIM_CR2_OIS1N;
  /* Set the Output Idle state */
  tmpcr2 |= TIM_OCIdleState_Reset;
  //  /* Set the Output N Idle state */
  tmpcr2 |= TIM_OCNIdleState_Reset;
  /* Write to TIMx CR2 */
  MOTOR_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  MOTOR_TIM->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  MOTOR_TIM->CCR1 = 0;
  /* Write to TIMx CCER */
  MOTOR_TIM->CCER = tmpccer;
  MOTOR_TIM->CCMR1 |= TIM_CCMR1_OC1PE;

  // 初始化通道 2 输出 PWM 
  MOTOR_TIM->CCER &= ~TIM_CCER_CC2E;
  /* Get the TIMx CCER register value */
  tmpccer = MOTOR_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  MOTOR_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = MOTOR_TIM->CCMR1;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
  /* Select the Output Compare Mode */
  tmpccmrx |= (TIM_OCMode_PWM1 << 8);
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC2P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 4);
  /* Reset the Output N Polarity level */
  tmpccer &= ~TIM_CCER_CC2NP;
  /* Set the Output N Polarity */
  tmpccer |= (TIM_OCNPolarity_High << 4);
  /* Reset the Output N State */
  tmpccer &= ~TIM_CCER_CC2NE;
  /* Reset the Output Compare and Output Compare N IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS2;
  tmpcr2 &= ~TIM_CR2_OIS2N;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 2);
  /* Set the Output N Idle state */
  tmpcr2 |= (TIM_OCNIdleState_Reset << 2);
  /* Write to TIMx CR2 */
  MOTOR_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  MOTOR_TIM->CCMR1 = tmpccmrx;
  /* Set the Capture Compare Register value */
  MOTOR_TIM->CCR2 = 0;
  /* Write to TIMx CCER */
  MOTOR_TIM->CCER = tmpccer;
  MOTOR_TIM->CCMR1 |= TIM_CCMR1_OC2PE;
		
  // 初始化通道 3 输出 PWM 
  MOTOR_TIM->CCER &= ~TIM_CCER_CC3E;
  /* Get the TIMx CCER register value */
  tmpccer = MOTOR_TIM->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  MOTOR_TIM->CR2;
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = MOTOR_TIM->CCMR2;
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCMode_PWM1;
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC3P;
  /* Set the Output Compare Polarity */
  tmpccer |= (TIM_OCPolarity_High << 8);
	/* Reset the Output N Polarity level */
  tmpccer &= ~TIM_CCER_CC3NP;
  /* Set the Output N Polarity */
  tmpccer |= (TIM_OCNPolarity_High << 8);
  /* Reset the Output N State */
  tmpccer &= ~TIM_CCER_CC3NE;
  /* Reset the Output Compare and Output Compare N IDLE State */
  tmpcr2 &= ~TIM_CR2_OIS3;
  tmpcr2 &= ~TIM_CR2_OIS3N;
  /* Set the Output Idle state */
  tmpcr2 |= (TIM_OCIdleState_Reset << 4);
  /* Set the Output N Idle state */
  tmpcr2 |= (TIM_OCNIdleState_Reset << 4);
  /* Write to TIMx CR2 */
  MOTOR_TIM->CR2 = tmpcr2;
  /* Write to TIMx CCMR1 */
  MOTOR_TIM->CCMR2 = tmpccmrx;
  /* Set the Capture Compare Register value */
  MOTOR_TIM->CCR3 = 0;
  /* Write to TIMx CCER */
  MOTOR_TIM->CCER = tmpccer;
  MOTOR_TIM->CCMR2 |= TIM_CCMR2_OC3PE;
	
   /* 配置触发源 */
  MOTOR_TIM->SMCR &= ~TIM_SMCR_TS;
  MOTOR_TIM->SMCR |= TIM_COM_TS_ITRx;
  /* Select the Capture Compare preload feature */
  //MOTOR_TIM->CR2 |= TIM_CR2_CCPC;
  /* Select the Commutation event source */
  MOTOR_TIM->CR2 &= ~TIM_CR2_CCUS;
  // MOTOR_TIM->CR2 |= TIM_COMMUTATION_SOFTWARE;
  /* Disable Commutation Interrupt */
  MOTOR_TIM->DIER &= ~TIM_IT_COM;
  /* Disable Commutation DMA request */
  MOTOR_TIM->DIER &= ~TIM_DMA_COM;

  /* 开启定时器通道1输出PWM */
  /* Reset the CCxE Bit */
  MOTOR_TIM->CCER &= ~(uint32_t)(TIM_CCER_CC1E);
  /* Set or reset the CCxE Bit */
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC1E);
  
  /* 开启定时器通道2输出PWM */
  /* Reset the CCxE Bit */
  MOTOR_TIM->CCER &= ~(uint32_t)(TIM_CCER_CC2E);
  /* Set or reset the CCxE Bit */
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC2E);
  /* 开启定时器通道3输出PWM */
  /* Reset the CCxE Bit */
  MOTOR_TIM->CCER &= ~(uint32_t)(TIM_CCER_CC3E);
  /* Set or reset the CCxE Bit */
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC3E);
  
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC1NE);
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC2NE);
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC3NE);
  
  MOTOR_TIM->SR &= ~TIM_IT_Update;
  MOTOR_TIM->DIER &= ~TIM_IT_Update;
  
  MOTOR_TIM->BDTR &= ~(TIM_BDTR_MOE);
  MOTOR_TIM->CR1  &= ~TIM_CR1_CEN;



}


/**
  * @brief  配置TIMx
  * @param  无
  * @retval 无
  */
static void TIM_Encoder_Init(void)
{ 
  uint32_t tmpcr1;
  uint32_t tmpsmcr;
  uint32_t tmpccmr1;
  uint32_t tmpccer;
  
  ENCODER_TIM->SMCR &= ~(TIM_SMCR_SMS | TIM_SMCR_ECE);
  /* Configure the Time base in the Encoder Mode */
  tmpcr1 = ENCODER_TIM->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  ENCODER_TIM->CR1 = tmpcr1;
  /* Set the Autoreload value */
  ENCODER_TIM->ARR = (uint32_t)ENCODER_TIM_PERIOD - 1 ;
  /* Set the Prescaler value */
  ENCODER_TIM->PSC = ENCODER_TIM_PRESCALER;
  ENCODER_TIM->RCR = 0;
  ENCODER_TIM->EGR = TIM_EGR_UG;
  
  /* Get the TIMx SMCR register value */
  tmpsmcr = ENCODER_TIM->SMCR;
  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = ENCODER_TIM->CCMR1;
  /* Get the TIMx CCER register value */
  tmpccer = ENCODER_TIM->CCER;
  /* Set the encoder Mode */
  tmpsmcr |= TIM_EncoderMode_TI12;
  /* Select the Capture Compare 1 and the Capture Compare 2 as input */
  tmpccmr1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S);
  tmpccmr1 |= (TIM_ICSelection_DirectTI | (TIM_ICSelection_DirectTI << 8U));
  /* Set the Capture Compare 1 and the Capture Compare 2 prescalers and filters */
  tmpccmr1 &= ~(TIM_CCMR1_IC1PSC | TIM_CCMR1_IC2PSC);
  tmpccmr1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC2F);
  tmpccmr1 |= TIM_ICPSC_DIV1 | (TIM_ICPSC_DIV1 << 8U);
  tmpccmr1 |= (0 << 4U) | (0 << 12U);
  /* Set the TI1 and the TI2 Polarities */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  tmpccer |= TIM_ICPolarity_Rising | (TIM_ICPolarity_Rising << 4U);
  /* Write to TIMx SMCR */
  ENCODER_TIM->SMCR = tmpsmcr;
  /* Write to TIMx CCMR1 */
  ENCODER_TIM->CCMR1 = tmpccmr1;
  /* Write to TIMx CCER */
  ENCODER_TIM->CCER = tmpccer;
  
  /* 清零计数器 */
  ENCODER_TIM->CNT = 0;
  
  /* 清零中断标志位 */
  ENCODER_TIM->SR = ~(TIM_IT_Update);
  /* 使能定时器的更新事件中断 */
  ENCODER_TIM->DIER |= (TIM_IT_Update);
  /* 设置更新事件请求源为：计数器溢出 */
  ENCODER_TIM->CR1|= TIM_CR1_URS;

  /* 使能编码器接口 */
 /* Reset the CCxE Bit */
  ENCODER_TIM->CCER &= ~TIM_CCER_CC1E;
  /* Set or reset the CCxE Bit */
  ENCODER_TIM->CCER |= TIM_CCER_CC1E; /* 0x1FU = 31 bits max shift */
  /* Reset the CCxE Bit */
  ENCODER_TIM->CCER &= ~TIM_CCER_CC2E;
  /* Set or reset the CCxE Bit */
  ENCODER_TIM->CCER |= TIM_CCER_CC2E; /* 0x1FU = 31 bits max shift */
  ENCODER_TIM->CR1 &= ~(TIM_CR1_CEN);

}

static void TIM_BASIC_CONFIG(void)
{
  uint32_t tmpcr1;	
	
  tmpcr1 = BASIC_TIM->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= TIM_CR1_ARPE;
  BASIC_TIM->CR1 = tmpcr1;
  
  /* Set the Autoreload value */
  BASIC_TIM->ARR = (uint32_t)BASIC_PERIOD_COUNT ;
  
  /* Set the Prescaler value */
  BASIC_TIM->PSC = BASIC_PRESCALER_COUNT ;
  
  BASIC_TIM->EGR = TIM_EGR_UG;
  
  // 开启定时器更新中断
  BASIC_TIM->SR = ~TIM_IT_Update;
  //BASIC_TIM->DIER |= (TIM_IT_Update);
  //BASIC_TIM->CR1|=(TIM_CR1_CEN);
  // intc enable
  INT_EN = 0x1;

}


void FocMotor_Init(){
  GPIO_Configuration_Foc();
  TIM_MOTOR_CONFIG();
  GPIO_Configuration_EN();
  printf("Motor config complete\n");
}

/**
  * @brief  HALL接口初始化
  * @param  无
  * @retval 无
  */
void FocEncoder_Init(void)
{
  GPIO_Configuration_Encoder();
  TIM_Encoder_Init();   
  printf("Encoder config complete\n");
}


void FocBasicTim_Init(){
  TIM_BASIC_CONFIG();
  printf("Basic config complete\n");
}


/**
  * @brief  停止pwm输出
  * @param  无
  * @retval 无
  */

void foc_stop_pwm_output(void)
{
  MOTOR_TIM->BDTR &= ~(TIM_BDTR_MOE);
  /* 关闭定时器通道1输出PWM */
  MOTOR_TIM->CCR1 = 0;
  /* 关闭定时器通道2输出PWM */
  MOTOR_TIM->CCR2 = 0;
  /* 关闭定时器通道3输出PWM */
  MOTOR_TIM->CCR3 = 0;
}


void mc_angle_init()
{
	ENCODER_TIM->CNT = 0;
	Foc_Encoder_Overflow_Count = 0;
}

/*******************************************************************************
* Description    : 返回电机转子的电角度
* Return         : Rotor electrical angle: 0 -> 0 degrees, 
*                                          S16_MAX-> 180 degrees, 
*                                          S16_MIN-> -180 degrees
*******************************************************************************/
int Get_Electrical_Angle(void)
{
  int temp;
  
  temp = (int)(ENCODER_TIM->CNT) * (int)(UINT32_MAX / (4*ENCODER_RESOLUTION));      
  temp *= POLE_PAIR_NUM;  
  return((int)(temp/65536));   //s16 result
}



