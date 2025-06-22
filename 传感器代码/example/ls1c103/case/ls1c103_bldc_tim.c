//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1c103_bldc_tim.h"
#include "ls1c103_bldc_ctrl.h"
#include "test.h"
#include "Config.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"
#include "ls1x_gpio.h"

static motor_dir_t direction  = MOTOR_FWD;     
static uint32_t    dutyfactor = 0;             
static uint32_t    is_motor_en = 0;            
static uint32_t    bldcm_pulse = 0;


static void GPIO_Configuration_Bldcm(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOA Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_Hall(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void GPIO_Configuration_EN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  AFIO_PORTA &= ~((uint32_t)0x00300000);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_10);
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
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= (uint32_t)TIM_CR1_ARPE;
  MOTOR_TIM->CR1 = tmpcr1;
  /* Set the Autoreload value */
  MOTOR_TIM->ARR = (uint32_t)PWM_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  MOTOR_TIM->PSC = PWM_PRESCALER_COUNT - 1;
  MOTOR_TIM->RCR = 1;
  MOTOR_TIM->EGR = TIM_EGR_UG;

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
	

  MOTOR_TIM->SMCR &= ~TIM_SMCR_TS;
  MOTOR_TIM->SMCR |= TIM_COM_TS_ITRx;
  /* Select the Capture Compare preload feature */
  MOTOR_TIM->CR2 |= TIM_CR2_CCPC;
  /* Select the Commutation event source */
  MOTOR_TIM->CR2 &= ~TIM_CR2_CCUS;
  /* Disable Commutation Interrupt */
  MOTOR_TIM->DIER &= ~TIM_IT_COM;
  /* Disable Commutation DMA request */
  MOTOR_TIM->DIER &= ~TIM_DMA_COM;

  MOTOR_TIM->CR2 |= TIM_TRGOSource_Update;

  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC1E);
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC2E);
  MOTOR_TIM->CCER |= (uint32_t)(TIM_CCER_CC3E);

}


static void TIM_Hall_Init(void)
{ 
  uint32_t tmpcr1;
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* time base set*/
  tmpcr1 = HALL_TIM->CR1;
  tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  tmpcr1 |= TIM_CounterMode_Up;
  tmpcr1 &= ~TIM_CR1_CKD;
  tmpcr1 |= (uint32_t)TIM_CKD_DIV1;
  tmpcr1 |= (uint32_t)TIM_CR1_ARPE;
  HALL_TIM->CR1 = tmpcr1;
  /* Set the Autoreload value */
  HALL_TIM->ARR = HALL_PERIOD_COUNT - 1;
  /* Set the Prescaler value */
  HALL_TIM->PSC = HALL_PRESCALER_COUNT - 1;
  HALL_TIM->EGR = TIM_EGR_UG;

  //TIM_TI1_SetConfig
  /* Disable the Channel 1: Reset the CC1E Bit */
  HALL_TIM->CCER &= ~TIM_CCER_CC1E;
  tmpccmr1 = HALL_TIM->CCMR1;
  tmpccer = HALL_TIM->CCER;
  /* Select the Input */
  tmpccmr1 &= ~TIM_CCMR1_CC1S;
  tmpccmr1 |= TIM_ICSelection_TRC;
  //tmpccmr1 &= ~TIM_CCMR1_CC2S;
  //tmpccmr1 |= (TIM_ICSelection_DirectTI << 8);           
  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;
  tmpccmr1 |= ((10 << 4U) & TIM_CCMR1_IC1F);  //TIM_ICFilter
  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  //tmpccer |= (TIM_ICPOLARITY_BOTHEDGE & (TIM_CCER_CC1P | TIM_CCER_CC1NP));
  /* Write to TIMx CCMR1 and CCER registers */
  HALL_TIM->CCMR1 = tmpccmr1;
  HALL_TIM->CCER = tmpccer;

  //HALL_TIM->CCMR2 &= ~TIM_CCMR2_CC3S;
  //HALL_TIM->CCMR2 |= TIM_ICSelection_DirectTI;
	
  /* Reset the IC1PSC Bits */
  HALL_TIM->CCMR1 &= ~TIM_CCMR1_IC1PSC;
  /* Set the IC1PSC value */
  HALL_TIM->CCMR1 |= TIM_ICPSC_DIV1;
  /* Enable the Hall sensor interface (XOR function of the three inputs) */
  HALL_TIM->CR2 |= TIM_CR2_TI1S;
  /* Select the TIM_TS_TI1F_ED signal as Input trigger for the TIM */
  HALL_TIM->SMCR &= ~TIM_SMCR_TS;
  HALL_TIM->SMCR |= TIM_TS_TI1F_ED;
  /* Use the TIM_TS_TI1F_ED signal to reset the TIM counter each edge detection */
  HALL_TIM->SMCR &= ~TIM_SMCR_SMS;
  HALL_TIM->SMCR |= TIM_SlaveMode_Reset;
  HALL_TIM->SR &= ~TIM_IT_Update;
  HALL_TIM->SR &= ~TIM_IT_Trigger;
}

static void TIM_PID_CONFIG(void)
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
  

  BASIC_TIM->SR &= (uint32_t)~TIM_IT_Update;
  BASIC_TIM->DIER |= (TIM_IT_Update);
  BASIC_TIM->CR1|=(TIM_CR1_CEN);
  // intc enable
  //  INT_EDGE &= 0;
  // INT_EN |= (1<<6);
}


void Motor_Init(){
  printf("Motor config begin\n\r");
  GPIO_Configuration_Bldcm();
  GPIO_Configuration_EN();
  TIM_MOTOR_CONFIG();
  printf("Motor config complete\n\r");
}

void Hall_Init(void)
{
  printf("Hall config begin\n\r");
  GPIO_Configuration_Hall();
  TIM_Hall_Init();   
  printf("Hall config complete\n\r");
}

void BasicTim_Init(){
  printf("Basic config begin\n\r");
  TIM_PID_CONFIG();
  printf("Basic config complete\n\r");
}

void set_pwm_pulse(uint32_t pulse)
{
  bldcm_pulse = pulse;
}

void set_direction(motor_dir_t dir)
{
  direction = dir;
}

void stop_pwm_output(void)
{
  MOTOR_TIM->BDTR &= ~(TIM_BDTR_MOE);
  MOTOR_TIM->CCR1 = 0;
  MOTOR_TIM->CCR2 = 0;
  MOTOR_TIM->CCR3 = 0;
  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
  MOTOR_TIM->EGR = TIM_EventSource_COM;
}

void hall_enable(void)
{
  HALL_TIM->DIER |= TIM_IT_Trigger;
  HALL_TIM->DIER |= TIM_IT_Update;
  HALL_TIM->CCER |= TIM_CCER_CC1E;
  HALL_TIM->CR1 |= TIM_CR1_CEN;
  HALL_TriggerCallback();   
}

void hall_disable(void)
{
  HALL_TIM->DIER &= ~TIM_IT_Trigger;
  HALL_TIM->DIER &= ~TIM_IT_Update;
  HALL_TIM->CCER &= ~TIM_CCER_CC1E;
  HALL_TIM->CR1 &= ~TIM_CR1_CEN;
}

uint8_t get_hall_state(void)
{
  uint8_t state = 0;
  uint8_t hallu, hallv, hallw;
  /* get hall status U */ 
  hallu = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
  if(hallu==1)
  {
    state |= 0x01U << 0;
  }
  /* get hall status V */ 
  hallv = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
  if(hallv==1)
  {
    state |= 0x01U << 1;
  }
  /* get hall status W */ 
  hallw = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9);
  if(hallw==1)
  {
    state |= 0x01U << 2;
  }
  return state;    // return HALL status
}

int update = 0;     // hall timer update flag,to watch timeout event


void HALL_TriggerCallback(void)
{
  /* get hall sensor status */
  uint8_t step = 0;
  step = get_hall_state();
  //printf("%d\n\r",step);
  if(direction == MOTOR_FWD)
    {
      switch(step)
	{
	case 1:    /* U+ W- */
	  MOTOR_TIM->CCR2 = 0;                        
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	 
	  MOTOR_TIM->CCR3 = 0;                         
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR1 = bldcm_pulse;       
	  MOTOR_TIM->CCER |= TIM_CCER_CC3NE;
	  
	  break;
	  
	case 2:     /* V+ U- */
	  MOTOR_TIM->CCR3 = 0;                         
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
	  
	  MOTOR_TIM->CCR1 = 0;                         
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR2 = bldcm_pulse;               
	  MOTOR_TIM->CCER |= TIM_CCER_CC1NE;
	  
	  
	  break;
	  
	case 3:    /* V+ W- */
	  MOTOR_TIM->CCR1 = 0;                        
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR3 = 0;                         
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR2 = bldcm_pulse;              
	  MOTOR_TIM->CCER |= TIM_CCER_CC3NE;
				
	  break;
	  
	case 4:     /* W+ V- */
	  MOTOR_TIM->CCR1 = 0;                       
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
				
	  MOTOR_TIM->CCR2 = 0;                        
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
	  
	  MOTOR_TIM->CCR3 = bldcm_pulse;               
	  MOTOR_TIM->CCER |= TIM_CCER_CC2NE;
				
	  
	  break;
	  
	case 5:     /* U+  V -*/
	  MOTOR_TIM->CCR3 = 0;                        
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
	  
	  MOTOR_TIM->CCR2 = 0;                       
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR1 = bldcm_pulse;              
	  MOTOR_TIM->CCER |= TIM_CCER_CC2NE;
	  
	  
	  break;
	  
	case 6:     /* W+ U- */
	  MOTOR_TIM->CCR2 = 0;                       
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR1 = 0;                        
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
				
	  MOTOR_TIM->CCR3 = bldcm_pulse;               
	  MOTOR_TIM->CCER |= TIM_CCER_CC1NE;
	  
	  break;
	}
    }
  else
    {
      switch(step)
	{
	case 1:   /* W+ U- */
	  MOTOR_TIM->CCR2 = 0;                       
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR1 = 0;                      
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
					
	  MOTOR_TIM->CCR3 = bldcm_pulse;             
	  MOTOR_TIM->CCER |= TIM_CCER_CC1NE;
	  break;
	  
	case 2:    /* U+  V -*/
	  MOTOR_TIM->CCR3 = 0;                     
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
	  
	  MOTOR_TIM->CCR2 = 0;                      
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR1 = bldcm_pulse;           
	  MOTOR_TIM->CCER |= TIM_CCER_CC2NE;
	  break;
	  
	case 3:   /* W+ V- */
	  MOTOR_TIM->CCR1 = 0;                      
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR2 = 0;                     
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
	  
	  MOTOR_TIM->CCR3 = bldcm_pulse;            
	  MOTOR_TIM->CCER |= TIM_CCER_CC2NE;     
	  
	  break;
	  
	case 4:    /* V+ W- */
	  MOTOR_TIM->CCR1 = 0;                   
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR3 = 0;                 
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR2 = bldcm_pulse;        
	  MOTOR_TIM->CCER |= TIM_CCER_CC3NE;
	  break;
	  
	case 5:    /* V+ U- */
	  MOTOR_TIM->CCR3 = 0;                   
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC3NE;
	  
	  MOTOR_TIM->CCR1 = 0;            
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR2 = bldcm_pulse;    
	  MOTOR_TIM->CCER |= TIM_CCER_CC1NE;
	  break;
	  
	case 6:    /* U+ W- */
	  MOTOR_TIM->CCR2 = 0;               
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC2NE;
	  
	  MOTOR_TIM->CCR3 = 0;                
	  MOTOR_TIM->CCER &= ~TIM_CCER_CC1NE;
	  
	  MOTOR_TIM->CCR1 = bldcm_pulse;       
	  MOTOR_TIM->CCER |= TIM_CCER_CC3NE;
	  break;
	}
    }
  //soft COM event	
  MOTOR_TIM->EGR = TIM_EventSource_COM;
  update = 0;
}

void HALL_PeriodElapsedCallback(void)
{
  if (update++ > 1)    // time out
  {
    printf("TIMEOUT,MOTOR STOP!!!\n\r");
    update = 0;
    
    /* stop PWM output */
    hall_disable();       // disable Hall
    stop_pwm_output();    
    BLDCM_DISABLE_EN();
  }
}
