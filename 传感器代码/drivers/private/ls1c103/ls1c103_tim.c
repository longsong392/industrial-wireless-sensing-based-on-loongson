/* Includes ------------------------------------------------------------------*/
#include "ls1x.h"
#include "ls1c103_pmu.h"
#include "ls1c103_tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ---------------------- TIM registers bit mask ------------------------ */
#define CR1_CEN_Set                 ((uint16_t)0x0001)
#define CR1_CEN_Reset               ((uint16_t)0x03FE)
#define CR1_UDIS_Set                ((uint16_t)0x0002)
#define CR1_UDIS_Reset              ((uint16_t)0x03FD)
#define CR1_URS_Set                 ((uint16_t)0x0004)
#define CR1_URS_Reset               ((uint16_t)0x03FB)
#define CR1_OPM_Reset               ((uint16_t)0x03F7)
#define CR1_CounterMode_Mask        ((uint16_t)0x038F)
#define CR1_ARPE_Set                ((uint16_t)0x0080)
#define CR1_ARPE_Reset              ((uint16_t)0x037F)
#define CR1_CKD_Mask                ((uint16_t)0x00FF)

#define CR2_CCPC_Set                ((uint16_t)0x0001)
#define CR2_CCPC_Reset              ((uint16_t)0xFFFE)
#define CR2_CCUS_Set                ((uint16_t)0x0004)
#define CR2_CCUS_Reset              ((uint16_t)0xFFFB)
#define CR2_CCDS_Set                ((uint16_t)0x0008)
#define CR2_CCDS_Reset              ((uint16_t)0xFFF7)
#define CR2_MMS_Mask                ((uint16_t)0xFF8F)
#define CR2_TI1S_Set                ((uint16_t)0x0080)
#define CR2_TI1S_Reset              ((uint16_t)0xFF7F)
#define CR2_OIS1_Reset              ((uint16_t)0x7EFF)
#define CR2_OIS1N_Reset             ((uint16_t)0x7DFF)
#define CR2_OIS2_Reset              ((uint16_t)0x7BFF)
#define CR2_OIS2N_Reset             ((uint16_t)0x77FF)
#define CR2_OIS3_Reset              ((uint16_t)0x6FFF)
#define CR2_OIS3N_Reset             ((uint16_t)0x5FFF)
#define CR2_OIS4_Reset              ((uint16_t)0x3FFF)

#define SMCR_SMS_Mask               ((uint16_t)0xFFF8)
#define SMCR_ETR_Mask               ((uint16_t)0x00FF)
#define SMCR_TS_Mask                ((uint16_t)0xFF8F)
#define SMCR_MSM_Reset              ((uint16_t)0xFF7F)
#define SMCR_ECE_Set                ((uint16_t)0x4000)

#define CCMR_CC13S_Mask             ((uint16_t)0xFFFC)
#define CCMR_CC24S_Mask             ((uint16_t)0xFCFF)
#define CCMR_TI13Direct_Set         ((uint16_t)0x0001)
#define CCMR_TI24Direct_Set         ((uint16_t)0x0100)
#define CCMR_OC13FE_Reset           ((uint16_t)0xFFFB)
#define CCMR_OC24FE_Reset           ((uint16_t)0xFBFF)
#define CCMR_OC13PE_Reset           ((uint16_t)0xFFF7)
#define CCMR_OC24PE_Reset           ((uint16_t)0xF7FF)
#define CCMR_OC13M_Mask             ((uint16_t)0xFF8F)
#define CCMR_OC24M_Mask             ((uint16_t)0x8FFF) 

#define CCMR_OC13CE_Reset           ((uint16_t)0xFF7F)
#define CCMR_OC24CE_Reset           ((uint16_t)0x7FFF)

#define CCMR_IC13PSC_Mask           ((uint16_t)0xFFF3)
#define CCMR_IC24PSC_Mask           ((uint16_t)0xF3FF)
#define CCMR_IC13F_Mask             ((uint16_t)0xFF0F)
#define CCMR_IC24F_Mask             ((uint16_t)0x0FFF)

#define CCMR_Offset                 ((uint16_t)0x0018)
#define CCER_CCE_Set                ((uint16_t)0x0001)
#define	CCER_CCNE_Set               ((uint16_t)0x0004)

#define CCER_CC1P_Reset             ((uint16_t)0xFFFD)
#define CCER_CC2P_Reset             ((uint16_t)0xFFDF)
#define CCER_CC3P_Reset             ((uint16_t)0xFDFF)
#define CCER_CC4P_Reset             ((uint16_t)0xDFFF)

#define CCER_CC1NP_Reset            ((uint16_t)0xFFF7)
#define CCER_CC2NP_Reset            ((uint16_t)0xFF7F)
#define CCER_CC3NP_Reset            ((uint16_t)0xF7FF)

#define CCER_CC1E_Set               ((uint16_t)0x0001)
#define CCER_CC1E_Reset             ((uint16_t)0xFFFE)

#define CCER_CC1NE_Reset            ((uint16_t)0xFFFB)

#define CCER_CC2E_Set               ((uint16_t)0x0010)
#define CCER_CC2E_Reset             ((uint16_t)0xFFEF)

#define CCER_CC2NE_Reset            ((uint16_t)0xFFBF)

#define CCER_CC3E_Set               ((uint16_t)0x0100)
#define CCER_CC3E_Reset             ((uint16_t)0xFEFF)

#define CCER_CC3NE_Reset            ((uint16_t)0xFBFF)

#define CCER_CC4E_Set               ((uint16_t)0x1000)
#define CCER_CC4E_Reset             ((uint16_t)0xEFFF)

#define BDTR_MOE_Set                ((uint16_t)0x8000)
#define BDTR_MOE_Reset              ((uint16_t)0x7FFF)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : TIM_DeInit
* Description    : Deinitializes the TIMx peripheral registers to their default
*                  reset values.
*******************************************************************************/
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
 
  switch (*(uint32_t*)&TIMx)
  {
    case TIM1_BASE:
      pmu_rst_cmd(RCC_ATIM,ENABLE);
      pmu_rst_cmd(RCC_ATIM,DISABLE);
      break; 
      
    case TIM2_BASE:
      pmu_rst_cmd(RCC_GTIM,ENABLE);
      pmu_rst_cmd(RCC_GTIM,DISABLE);
      break;
 
    case TIM6_BASE:
      pmu_rst_cmd(RCC_BTIM,ENABLE);
      pmu_rst_cmd(RCC_BTIM,DISABLE);
      break;
      
    default:
      break;
  }
}

/*******************************************************************************
* Function Name  : TIM_TimeBaseInit
* Description    : Initializes the TIMx Time Base Unit peripheral according to 
*                  the specified parameters in the TIM_TimeBaseInitStruct.
*******************************************************************************/
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));

  /* Select the Counter Mode and set the clock division */
  TIMx->CR1 &= CR1_CKD_Mask & CR1_CounterMode_Mask;
  TIMx->CR1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_ClockDivision |
                TIM_TimeBaseInitStruct->TIM_CounterMode;
  /* Set the Autoreload value */
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;

  /* Set the Prescaler value */
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;

  /* Generate an update event to reload the Prescaler value immediatly */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;
    
  if (((*(uint32_t*)&TIMx) == TIM1_BASE))  
  {
    /* Set the Repetition Counter value */
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }        
}

/*******************************************************************************
* Function Name  : TIM_OC1Init
* Description    : Initializes the TIMx Channel1 according to the specified
*                  parameters in the TIM_OCInitStruct.
*******************************************************************************/
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= CCER_CC1E_Reset;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= CCER_CC1P_Reset;

  /* Set the Output Compare Polarity */
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* Set the Output State */
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
  
  /* Set the Capture Compare Register value */
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(uint32_t*)&TIMx == TIM1_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level */
    tmpccer &= CCER_CC1NP_Reset;

    /* Set the Output N Polarity */
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;

    /* Reset the Output N State */
    tmpccer &= CCER_CC1NE_Reset;
    
    /* Set the Output N State */
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;

    /* Reset the Ouput Compare and Output Compare N IDLE State */
    tmpcr2 &= CR2_OIS1_Reset;
    tmpcr2 &= CR2_OIS1N_Reset;

    /* Set the Output Idle state */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;

    /* Set the Output N Idle state */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC2Init
* Description    : Initializes the TIMx Channel2 according to the specified
*                  parameters in the TIM_OCInitStruct.
*******************************************************************************/
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= CCER_CC2E_Reset;
  
  /* Get the TIMx CCER register value */  
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* Select the Output Compare Mode */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level */
  tmpccer &= CCER_CC2P_Reset;

  /* Set the Output Compare Polarity */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* Set the Output State */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 4);
  
  /* Set the Capture Compare Register value */
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(uint32_t*)&TIMx == TIM1_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level */
    tmpccer &= CCER_CC2NP_Reset;

    /* Set the Output N Polarity */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 4);

    /* Reset the Output N State */
    tmpccer &= CCER_CC2NE_Reset;
    
    /* Set the Output N State */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 4);

    /* Reset the Ouput Compare and Output Compare N IDLE State */
    tmpcr2 &= CR2_OIS2_Reset;
    tmpcr2 &= CR2_OIS2N_Reset;

    /* Set the Output Idle state */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 2);

    /* Set the Output N Idle state */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC3Init
* Description    : Initializes the TIMx Channel3 according to the specified
*                  parameters in the TIM_OCInitStruct.
*******************************************************************************/
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= CCER_CC3E_Reset;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= CCER_CC3P_Reset;

  /* Set the Output Compare Polarity */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* Set the Output State */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 8);
  
  /* Set the Capture Compare Register value */
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(uint32_t*)&TIMx == TIM1_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* Reset the Output N Polarity level */
    tmpccer &= CCER_CC3NP_Reset;

    /* Set the Output N Polarity */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 8);

    /* Reset the Output N State */
    tmpccer &= CCER_CC3NE_Reset;
    
    /* Set the Output N State */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 8);

    /* Reset the Ouput Compare and Output Compare N IDLE State */
    tmpcr2 &= CR2_OIS3_Reset;
    tmpcr2 &= CR2_OIS3N_Reset;

    /* Set the Output Idle state */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 4);

    /* Set the Output N Idle state */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC4Init
* Description    : Initializes the TIMx Channel4 according to the specified
*                  parameters in the TIM_OCInitStruct.
*******************************************************************************/
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   

  /* Disable the Channel 2: Reset the CC4E Bit */
  TIMx->CCER &= CCER_CC4E_Reset;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;

  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* Select the Output Compare Mode */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level */
  tmpccer &= CCER_CC4P_Reset;

  /* Set the Output Compare Polarity */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* Set the Output State */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 12);
  
  /* Set the Capture Compare Register value */
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  if((*(uint32_t*)&TIMx == TIM1_BASE))
  {
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));

    /* Reset the Ouput Compare IDLE State */
    tmpcr2 &= CR2_OIS4_Reset;

    /* Set the Output Idle state */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 6);
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 */  
  TIMx->CCMR2 = tmpccmrx;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_ICInit
* Description    : Initializes the TIM peripheral according to the specified
*                  parameters in the TIM_ICInitStruct.
*******************************************************************************/
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    /* TI2 Configuration */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    /* TI3 Configuration */
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    /* TI4 Configuration */
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/*******************************************************************************
* Function Name  : TIM_PWMIConfig
* Description    : Configures the TIM peripheral according to the specified
*                  parameters in the TIM_ICInitStruct to measure an external PWM
*                  signal.
*******************************************************************************/
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  uint16_t icoppositepolarity = TIM_ICPolarity_Rising;
  uint16_t icoppositeselection = TIM_ICSelection_DirectTI;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Select the Opposite Input Polarity */
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }

  /* Select the Opposite Input */
  if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI)
  {
    icoppositeselection = TIM_ICSelection_IndirectTI;
  }
  else
  {
    icoppositeselection = TIM_ICSelection_DirectTI;
  }

  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);

    /* TI2 Configuration */
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 Configuration */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);

    /* TI1 Configuration */
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);

    /* Set the Input Capture Prescaler value */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/*******************************************************************************
* Function Name  : TIM_BDTRConfig
* Description    : Configures the: Break feature, dead time, Lock level, the OSSI,
*                  the OSSR State and the AOE(automatic output enable).
*******************************************************************************/
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
  assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
  assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
  assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
  assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));

  /* Set the Lock level, the Break enable Bit and the Ploarity, the OSSR State,
     the OSSI State, the dead time value and the Automatic Output Enable Bit */

  TIMx->BDTR = (uint32_t)TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
             TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
             TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
             TIM_BDTRInitStruct->TIM_AutomaticOutput;

}

/*******************************************************************************
* Function Name  : TIM_TimeBaseStructInit
* Description    : Fills each TIM_TimeBaseInitStruct member with its default value.
*******************************************************************************/
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Set the default configuration */
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

/*******************************************************************************
* Function Name  : TIM_OCStructInit
* Description    : Fills each TIM_OCInitStruct member with its default value.
*******************************************************************************/
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* Set the default configuration */
  TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct->TIM_Pulse = 0x0000;
  TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

/*******************************************************************************
* Function Name  : TIM_ICStructInit
* Description    : Fills each TIM_ICInitStruct member with its default value.
*******************************************************************************/
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Set the default configuration */
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/*******************************************************************************
* Function Name  : TIM_BDTRStructInit
* Description    : Fills each TIM_BDTRInitStruct member with its default value.
*******************************************************************************/
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
{
  /* Set the default configuration */
  TIM_BDTRInitStruct->TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct->TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct->TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct->TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct->TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct->TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

/*******************************************************************************
* Function Name  : TIM_Cmd
* Description    : Enables or disables the specified TIM peripheral.
*******************************************************************************/
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    TIMx->CR1 |= CR1_CEN_Set;
  }
  else
  {
    /* Disable the TIM Counter */
    TIMx->CR1 &= CR1_CEN_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_CtrlPWMOutputs
* Description    : Enables or disables the TIM peripheral Main Outputs.
*******************************************************************************/
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the TIM Main Output */
    TIMx->BDTR |= BDTR_MOE_Set;
  }
  else
  {
    /* Disable the TIM Main Output */
    TIMx->BDTR &= BDTR_MOE_Reset;
  }  
}

/*******************************************************************************
* Function Name  : TIM_ITConfig
* Description    : Enables or disables the specified TIM interrupts.
*******************************************************************************/
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)
{  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  assert_param(IS_TIM_PERIPH_IT((TIMx), (TIM_IT)));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
    TIMx->DIER &= (uint16_t)~TIM_IT;
  }
}

/*******************************************************************************
* Function Name  : TIM_GenerateEvent
* Description    : Configures the TIMx event to be generate by software.
*******************************************************************************/
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource)
{ 
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));
  assert_param(IS_TIM_PERIPH_EVENT((TIMx), (TIM_EventSource)));

  /* Set the event sources */
  TIMx->EGR = TIM_EventSource;
}

/*******************************************************************************
* Function Name  : TIM_DMAConfig
* Description    : Configures the TIMx’s DMA interface.
*******************************************************************************/
void TIM_DMAConfig(TIM_TypeDef* TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
  assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));

  /* Set the DMA Base and the DMA Burst Length */
  TIMx->DCR = TIM_DMABase | TIM_DMABurstLength;
}

/*******************************************************************************
* Function Name  : TIM_DMACmd
* Description    : Enables or disables the TIMx DMA Requests.
*******************************************************************************/
void TIM_DMACmd(TIM_TypeDef* TIMx, uint16_t TIM_DMASource, FunctionalState NewState)
{ 
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
  assert_param(IS_TIM_PERIPH_DMA(TIMx, TIM_DMASource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the DMA sources */
    TIMx->DIER |= TIM_DMASource; 
  }
  else
  {
    /* Disable the DMA sources */
    TIMx->DIER &= (uint16_t)~TIM_DMASource;
  }
}

/*******************************************************************************
* Function Name  : TIM_InternalClockConfig
* Description    : Configures the TIMx interrnal Clock
*******************************************************************************/
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Disable slave mode to clock the prescaler directly with the internal clock */
  TIMx->SMCR &=  SMCR_SMS_Mask;
}
/*******************************************************************************
* Function Name  : TIM_ITRxExternalClockConfig
* Description    : Configures the TIMx Internal Trigger as External Clock
*******************************************************************************/
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Select the Internal Trigger */
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);

  /* Select the External clock mode1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}
/*******************************************************************************
* Function Name  : TIM_TIxExternalClockConfig
* Description    : Configures the TIMx Trigger as External Clock
*******************************************************************************/
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));

  /* Configure the Timer Input Clock Source */
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }

  /* Select the Trigger source */
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);

  /* Select the External clock mode1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/*******************************************************************************
* Function Name  : TIM_ETRClockMode1Config
* Description    : Configures the External clock Mode1
*******************************************************************************/
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  /* Configure the ETR Clock source */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;

  /* Reset the SMS Bits */
  tmpsmcr &= SMCR_SMS_Mask;
  /* Select the External clock mode1 */
  tmpsmcr |= TIM_SlaveMode_External1;

  /* Select the Trigger selection : ETRF */
  tmpsmcr &= SMCR_TS_Mask;
  tmpsmcr |= TIM_TS_ETRF;

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* Function Name  : TIM_ETRClockMode2Config
* Description    : Configures the External clock Mode2
*******************************************************************************/
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  /* Configure the ETR Clock source */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);

  /* Enable the External clock mode2 */
  TIMx->SMCR |= SMCR_ECE_Set;
}

/*******************************************************************************
* Function Name  : TIM_ETRConfig
* Description    : Configures the TIMx External Trigger (ETR).
*******************************************************************************/
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));

  tmpsmcr = TIMx->SMCR;

  /* Reset the ETR Bits */
  tmpsmcr &= SMCR_ETR_Mask;

  /* Set the Prescaler, the Filter value and the Polarity */
  tmpsmcr |= TIM_ExtTRGPrescaler | TIM_ExtTRGPolarity | (uint16_t)(ExtTRGFilter << 8);

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* Function Name  : TIM_PrescalerConfig
* Description    : Configures the TIMx Prescaler.
*******************************************************************************/
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));

  /* Set the Prescaler value */
  TIMx->PSC = Prescaler;

  /* Set or reset the UG Bit */
  TIMx->EGR = TIM_PSCReloadMode;
}

/*******************************************************************************
* Function Name  : TIM_CounterModeConfig
* Description    : Specifies the TIMx Counter Mode to be used.
*******************************************************************************/
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode)
{
  uint16_t tmpcr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));

  tmpcr1 = TIMx->CR1;

  /* Reset the CMS and DIR Bits */
  tmpcr1 &= CR1_CounterMode_Mask;

  /* Set the Counter Mode */
  tmpcr1 |= TIM_CounterMode;

  /* Write to TIMx CR1 register */
  TIMx->CR1 = tmpcr1;
}

/*******************************************************************************
* Function Name  : TIM_SelectInputTrigger
* Description    : Selects the Input Trigger source
*******************************************************************************/
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  uint16_t tmpsmcr = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;

  /* Reset the TS Bits */
  tmpsmcr &= SMCR_TS_Mask;

  /* Set the Input Trigger source */
  tmpsmcr |= TIM_InputTriggerSource;

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/*******************************************************************************
* Function Name  : TIM_EncoderInterfaceConfig
* Description    : Configures the TIMx Encoder Interface.
*******************************************************************************/
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
  uint16_t tmpsmcr = 0;
  uint16_t tmpccmr1 = 0;
  uint16_t tmpccer = 0;
    
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = TIMx->CCMR1;

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;

  /* Set the encoder Mode */
  tmpsmcr &= SMCR_SMS_Mask;
  tmpsmcr |= TIM_EncoderMode;

  /* Select the Capture Compare 1 and the Capture Compare 2 as input */
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_CC24S_Mask;
  tmpccmr1 |= CCMR_TI13Direct_Set | CCMR_TI24Direct_Set;

  /* Set the TI1 and the TI2 Polarities */
  tmpccer &= CCER_CC1P_Reset & CCER_CC2P_Reset;
  tmpccer |= (TIM_IC1Polarity | (uint16_t)(TIM_IC2Polarity << 4));

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_ForcedOC1Config
* Description    : Forces the TIMx output 1 waveform to active or inactive level.
*******************************************************************************/
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1M Bits */
  tmpccmr1 &= CCMR_OC13M_Mask;

  /* Configure The Forced output Mode */
  tmpccmr1 |= TIM_ForcedAction;

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_ForcedOC2Config
* Description    : Forces the TIMx output 2 waveform to active or inactive level.
*******************************************************************************/
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2M Bits */
  tmpccmr1 &= CCMR_OC24M_Mask;

  /* Configure The Forced output Mode */
  tmpccmr1 |= (uint16_t)(TIM_ForcedAction << 8);

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_ForcedOC3Config
* Description    : Forces the TIMx output 3 waveform to active or inactive level.
*******************************************************************************/
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC1M Bits */
  tmpccmr2 &= CCMR_OC13M_Mask;

  /* Configure The Forced output Mode */
  tmpccmr2 |= TIM_ForcedAction;

  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_ForcedOC4Config
* Description    : Forces the TIMx output 4 waveform to active or inactive level.
*******************************************************************************/
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC2M Bits */
  tmpccmr2 &= CCMR_OC24M_Mask;

  /* Configure The Forced output Mode */
  tmpccmr2 |= (uint16_t)(TIM_ForcedAction << 8);

  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_ARRPreloadConfig
* Description    : Enables or disables TIMx peripheral Preload register on ARR.
*******************************************************************************/
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ARR Preload Bit */
    TIMx->CR1 |= CR1_ARPE_Set;
  }
  else
  {
    /* Reset the ARR Preload Bit */
    TIMx->CR1 &= CR1_ARPE_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_SelectCOM
* Description    : Selects the TIM peripheral Commutation event.
*******************************************************************************/
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the COM Bit */
    TIMx->CR2 |= CR2_CCUS_Set;
  }
  else
  {
    /* Reset the COM Bit */
    TIMx->CR2 &= CR2_CCUS_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_SelectCCDMA
* Description    : Selects the TIMx peripheral Capture Compare DMA source.
*******************************************************************************/
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the CCDS Bit */
    TIMx->CR2 |= CR2_CCDS_Set;
  }
  else
  {
    /* Reset the CCDS Bit */
    TIMx->CR2 &= CR2_CCDS_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_CCPreloadControl
* Description    : Sets or Resets the TIM peripheral Capture Compare Preload 
*                  Control bit.
*******************************************************************************/
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the CCPC Bit */
    TIMx->CR2 |= CR2_CCPC_Set;
  }
  else
  {
    /* Reset the CCPC Bit */
    TIMx->CR2 &= CR2_CCPC_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_OC1PreloadConfig
* Description    : Enables or disables the TIMx peripheral Preload register on CCR1.
*******************************************************************************/
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1PE Bit */
  tmpccmr1 &= CCMR_OC13PE_Reset;

  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr1 |= TIM_OCPreload;

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_OC2PreloadConfig
* Description    : Enables or disables the TIMx peripheral Preload register on CCR2.
*******************************************************************************/
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2PE Bit */
  tmpccmr1 &= CCMR_OC24PE_Reset;

  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr1 |= (uint16_t)(TIM_OCPreload << 8);

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_OC3PreloadConfig
* Description    : Enables or disables the TIMx peripheral Preload register on CCR3.
*******************************************************************************/
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3PE Bit */
  tmpccmr2 &= CCMR_OC13PE_Reset;

  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr2 |= TIM_OCPreload;

  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_OC4PreloadConfig
* Description    : Enables or disables the TIMx peripheral Preload register on CCR4.
*******************************************************************************/
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4PE Bit */
  tmpccmr2 &= CCMR_OC24PE_Reset;

  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr2 |= (uint16_t)(TIM_OCPreload << 8);

  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_OC1FastConfig
* Description    : Configures the TIMx Output Compare 1 Fast feature.
*******************************************************************************/
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1FE Bit */
  tmpccmr1 &= CCMR_OC13FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr1 |= TIM_OCFast;

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_OC2FastConfig
* Description    : Configures the TIMx Output Compare 2 Fast feature.
*******************************************************************************/
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2FE Bit */
  tmpccmr1 &= CCMR_OC24FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr1 |= (uint16_t)(TIM_OCFast << 8);

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_OC3FastConfig
* Description    : Configures the TIMx Output Compare 3 Fast feature.
*******************************************************************************/
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR2 register value */
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3FE Bit */
  tmpccmr2 &= CCMR_OC13FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr2 |= TIM_OCFast;

  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_OC4FastConfig
* Description    : Configures the TIMx Output Compare 4 Fast feature.
*******************************************************************************/
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));

  /* Get the TIMx CCMR2 register value */
  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4FE Bit */
  tmpccmr2 &= CCMR_OC24FE_Reset;

  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr2 |= (uint16_t)(TIM_OCFast << 8);

  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_ClearOC1Ref
* Description    : Clears or safeguards the OCREF1 signal on an external event
*******************************************************************************/
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC1CE Bit */
  tmpccmr1 &= CCMR_OC13CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit */
  tmpccmr1 |= TIM_OCClear;

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_ClearOC2Ref
* Description    : Clears or safeguards the OCREF2 signal on an external event
*******************************************************************************/
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* Reset the OC2CE Bit */
  tmpccmr1 &= CCMR_OC24CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit */
  tmpccmr1 |= (uint16_t)(TIM_OCClear << 8);

  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/*******************************************************************************
* Function Name  : TIM_ClearOC3Ref
* Description    : Clears or safeguards the OCREF3 signal on an external event
*******************************************************************************/
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC3CE Bit */
  tmpccmr2 &= CCMR_OC13CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit */
  tmpccmr2 |= TIM_OCClear;

  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_ClearOC4Ref
* Description    : Clears or safeguards the OCREF4 signal on an external event
*******************************************************************************/
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr2 = TIMx->CCMR2;

  /* Reset the OC4CE Bit */
  tmpccmr2 &= CCMR_OC24CE_Reset;

  /* Enable or Disable the Output Compare Clear Bit */
  tmpccmr2 |= (uint16_t)(TIM_OCClear << 8);

  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/*******************************************************************************
* Function Name  : TIM_OC1PolarityConfig
* Description    : Configures the TIMx channel 1 polarity.
*******************************************************************************/
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC1P Bit */
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_OCPolarity;

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC1NPolarityConfig
* Description    : Configures the TIMx Channel 1N polarity.
*******************************************************************************/
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;

  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
   
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC1NP Bit */
  tmpccer &= CCER_CC1NP_Reset;
  tmpccer |= TIM_OCNPolarity;

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC2PolarityConfig
* Description    : Configures the TIMx channel 2 polarity.
*******************************************************************************/
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC2P Bit */
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 4);

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC2NPolarityConfig
* Description    : Configures the TIMx Channel 2N polarity.
*******************************************************************************/
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;

  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
  
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC2NP Bit */
  tmpccer &= CCER_CC2NP_Reset;
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 4);

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC3PolarityConfig
* Description    : Configures the TIMx channel 3 polarity.
*******************************************************************************/
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC3P Bit */
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 8);

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC3NPolarityConfig
* Description    : Configures the TIMx Channel 3N polarity.
*******************************************************************************/
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
 
  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
    
  tmpccer = TIMx->CCER;

  /* Set or Reset the CC3NP Bit */
  tmpccer &= CCER_CC3NP_Reset;
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 8);

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_OC4PolarityConfig
* Description    : Configures the TIMx channel 4 polarity.
*******************************************************************************/
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;

  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));

  tmpccer = TIMx->CCER;

  /* Set or Reset the CC4P Bit */
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 12);

  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TIM_CCxCmd
* Description    : Enables or disables the TIM Capture Compare Channel x.
*******************************************************************************/
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCX(TIM_CCx));

  /* Reset the CCxE Bit */
  TIMx->CCER &= (uint16_t)(~((uint16_t)(CCER_CCE_Set << TIM_Channel)));

  /* Set or reset the CCxE Bit */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCx << TIM_Channel);
}

/*******************************************************************************
* Function Name  : TIM_CCxNCmd
* Description    : Enables or disables the TIM Capture Compare Channel xN.
*******************************************************************************/
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN)
{
  /* Check the parameters */
  assert_param(IS_TIM_1_PERIPH(TIMx));
  assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCXN(TIM_CCxN));

  /* Reset the CCxNE Bit */
  TIMx->CCER &= (uint16_t)(~((uint16_t)(CCER_CCNE_Set << TIM_Channel)));

  /* Set or reset the CCxNE Bit */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCxN << TIM_Channel);
}

/*******************************************************************************
* Function Name  : TIM_SelectOCxM
* Description    : Selects the TIM Ouput Compare Mode.
*                  This function disables the selected channel before changing 
*                  the Ouput Compare Mode. User has to enable this channel using
*                  TIM_CCxCmd and TIM_CCxNCmd functions.
*******************************************************************************/
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_OCM(TIM_OCMode));
  
  /* Disable the Channel: Reset the CCxE Bit */
  TIMx->CCER &= (uint16_t)(~((uint16_t)(CCER_CCE_Set << TIM_Channel)));

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    /* Reset the OCxM bits in the CCMRx register */
    *((vuint32_t *)((*(uint32_t*)&TIMx) + CCMR_Offset + (TIM_Channel>>1))) &= CCMR_OC13M_Mask;
   
    /* Configure the OCxM bits in the CCMRx register */
    *((vuint32_t *)((*(uint32_t*)&TIMx) + CCMR_Offset + (TIM_Channel>>1))) |= TIM_OCMode;

  }
  else
  {
    /* Reset the OCxM bits in the CCMRx register */
    *((vuint32_t *)((*(uint32_t*)&TIMx) + CCMR_Offset + ((uint16_t)(TIM_Channel - 4)>> 1))) &= CCMR_OC24M_Mask;
    
    /* Configure the OCxM bits in the CCMRx register */
    *((vuint32_t *)((*(uint32_t*)&TIMx) + CCMR_Offset + ((uint16_t)(TIM_Channel - 4)>> 1))) |= (uint16_t)(TIM_OCMode << 8);
  }
}

/*******************************************************************************
* Function Name  : TIM_UpdateDisableConfig
* Description    : Enables or Disables the TIMx Update event.
*******************************************************************************/
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the Update Disable Bit */
    TIMx->CR1 |= CR1_UDIS_Set;
  }
  else
  {
    /* Reset the Update Disable Bit */
    TIMx->CR1 &= CR1_UDIS_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_UpdateRequestConfig
* Description    : Configures the TIMx Update Request Interrupt source.
*******************************************************************************/
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint16_t TIM_UpdateSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));

  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* Set the URS Bit */
    TIMx->CR1 |= CR1_URS_Set;
  }
  else
  {
    /* Reset the URS Bit */
    TIMx->CR1 &= CR1_URS_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_SelectHallSensor
* Description    : Enables or disables the TIMx’s Hall sensor interface.
*******************************************************************************/
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the TI1S Bit */
    TIMx->CR2 |= CR2_TI1S_Set;
  }
  else
  {
    /* Reset the TI1S Bit */
    TIMx->CR2 &= CR2_TI1S_Reset;
  }
}

/*******************************************************************************
* Function Name  : TIM_SelectOnePulseMode
* Description    : Selects the TIMx One Pulse Mode.
*******************************************************************************/
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));

  /* Reset the OPM Bit */
  TIMx->CR1 &= CR1_OPM_Reset;

  /* Configure the OPM Mode */
  TIMx->CR1 |= TIM_OPMode;
}

/*******************************************************************************
* Function Name  : TIM_SelectOutputTrigger
* Description    : Selects the TIMx Trigger Output Mode.
*******************************************************************************/
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
  assert_param(IS_TIM_PERIPH_TRGO(TIMx, TIM_TRGOSource));

  /* Reset the MMS Bits */
  TIMx->CR2 &= CR2_MMS_Mask;

  /* Select the TRGO source */
  TIMx->CR2 |=  TIM_TRGOSource;
}

/*******************************************************************************
* Function Name  : TIM_SelectSlaveMode
* Description    : Selects the TIMx Slave Mode.
*******************************************************************************/
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));

  /* Reset the SMS Bits */
  TIMx->SMCR &= SMCR_SMS_Mask;

  /* Select the Slave Mode */
  TIMx->SMCR |= TIM_SlaveMode;
}

/*******************************************************************************
* Function Name  : TIM_SelectMasterSlaveMode
* Description    : Sets or Resets the TIMx Master/Slave Mode.
*******************************************************************************/
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_MasterSlaveMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));

  /* Reset the MSM Bit */
  TIMx->SMCR &= SMCR_MSM_Reset;
  
  /* Set or Reset the MSM Bit */
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/*******************************************************************************
* Function Name  : TIM_SetCounter
* Description    : Sets the TIMx Counter Register value
*******************************************************************************/
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Set the Counter Register value */
  TIMx->CNT = Counter;
}

/*******************************************************************************
* Function Name  : TIM_SetAutoreload
* Description    : Sets the TIMx Autoreload Register value
*******************************************************************************/
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Set the Autoreload Register value */
  TIMx->ARR = Autoreload;
}

/*******************************************************************************
* Function Name  : TIM_SetCompare1
* Description    : Sets the TIMx Capture Compare1 Register value
*******************************************************************************/
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Set the Capture Compare1 Register value */
  TIMx->CCR1 = Compare1;
}

/*******************************************************************************
* Function Name  : TIM_SetCompare2
* Description    : Sets the TIMx Capture Compare2 Register value
*******************************************************************************/
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Set the Capture Compare2 Register value */
  TIMx->CCR2 = Compare2;
}

/*******************************************************************************
* Function Name  : TIM_SetCompare3
* Description    : Sets the TIMx Capture Compare3 Register value
*******************************************************************************/
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Set the Capture Compare3 Register value */
  TIMx->CCR3 = Compare3;
}

/*******************************************************************************
* Function Name  : TIM_SetCompare4
* Description    : Sets the TIMx Capture Compare4 Register value
*******************************************************************************/
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Set the Capture Compare4 Register value */
  TIMx->CCR4 = Compare4;
}

/*******************************************************************************
* Function Name  : TIM_SetIC1Prescaler
* Description    : Sets the TIMx Input Capture 1 prescaler.
*******************************************************************************/
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC1PSC Bits */
  TIMx->CCMR1 &= CCMR_IC13PSC_Mask;

  /* Set the IC1PSC value */
  TIMx->CCMR1 |= TIM_ICPSC;
}

/*******************************************************************************
* Function Name  : TIM_SetIC2Prescaler
* Description    : Sets the TIMx Input Capture 2 prescaler.
*******************************************************************************/
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC2PSC Bits */
  TIMx->CCMR1 &= CCMR_IC24PSC_Mask;

  /* Set the IC2PSC value */
  TIMx->CCMR1 |= (uint16_t)(TIM_ICPSC << 8);
}

/*******************************************************************************
* Function Name  : TIM_SetIC3Prescaler
* Description    : Sets the TIMx Input Capture 3 prescaler.
*******************************************************************************/
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC3PSC Bits */
  TIMx->CCMR2 &= CCMR_IC13PSC_Mask;

  /* Set the IC3PSC value */
  TIMx->CCMR2 |= TIM_ICPSC;
}

/*******************************************************************************
* Function Name  : TIM_SetIC4Prescaler
* Description    : Sets the TIMx Input Capture 4 prescaler.
*******************************************************************************/
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{  
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));

  /* Reset the IC4PSC Bits */
  TIMx->CCMR2 &= CCMR_IC24PSC_Mask;

  /* Set the IC4PSC value */
  TIMx->CCMR2 |= (uint16_t)(TIM_ICPSC << 8);
}

/*******************************************************************************
* Function Name  : TIM_SetClockDivision
* Description    : Sets the TIMx Clock Division value.
*******************************************************************************/
void TIM_SetClockDivision(TIM_TypeDef* TIMx, uint16_t TIM_CKD)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));
  assert_param(IS_TIM_CKD_DIV(TIM_CKD));

  /* Reset the CKD Bits */
  TIMx->CR1 &= CR1_CKD_Mask;

  /* Set the CKD value */
  TIMx->CR1 |= TIM_CKD;
}
/*******************************************************************************
* Function Name  : TIM_GetCapture1
* Description    : Gets the TIMx Input Capture 1 value.
*******************************************************************************/
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Get the Capture 1 Register value */
  return TIMx->CCR1;
}

/*******************************************************************************
* Function Name  : TIM_GetCapture2
* Description    : Gets the TIMx Input Capture 2 value.
*******************************************************************************/
uint16_t TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Get the Capture 2 Register value */
  return TIMx->CCR2;
}

/*******************************************************************************
* Function Name  : TIM_GetCapture3
* Description    : Gets the TIMx Input Capture 3 value.
*******************************************************************************/
uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx)); 

  /* Get the Capture 3 Register value */
  return TIMx->CCR3;
}

/*******************************************************************************
* Function Name  : TIM_GetCapture4
* Description    : Gets the TIMx Input Capture 4 value.
*******************************************************************************/
uint16_t TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_12_PERIPH(TIMx));

  /* Get the Capture 4 Register value */
  return TIMx->CCR4;
}

/*******************************************************************************
* Function Name  : TIM_GetCounter
* Description    : Gets the TIMx Counter value.
*******************************************************************************/
uint16_t TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Counter Register value */
  return TIMx->CNT;
}

/*******************************************************************************
* Function Name  : TIM_GetPrescaler
* Description    : Gets the TIMx Prescaler value.
*******************************************************************************/
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));

  /* Get the Prescaler Register value */
  return TIMx->PSC;
}

/*******************************************************************************
* Function Name  : TIM_GetFlagStatus
* Description    : Checks whether the specified TIM flag is set or not.
*******************************************************************************/
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_FLAG(TIM_FLAG));
  
  if ((TIMx->SR & TIM_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* Function Name  : TIM_ClearFlag
* Description    : Clears the TIMx's pending flags.
*******************************************************************************/
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
   
  /* Clear the flags */
  TIMx->SR = (uint16_t)~TIM_FLAG;
}

/*******************************************************************************
* Function Name  : TIM_GetITStatus
* Description    : Checks whether the TIM interrupt has occurred or not.
*******************************************************************************/
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  ITStatus bitstatus = RESET;  
  uint16_t itstatus = 0x0, itenable = 0x0;

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_IT(TIM_IT));
  assert_param(IS_TIM_PERIPH_IT(TIMx, TIM_IT));
   
  itstatus = TIMx->SR & TIM_IT;
  
  itenable = TIMx->DIER & TIM_IT;

  if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/*******************************************************************************
* Function Name  : TIM_ClearITPendingBit
* Description    : Clears the TIMx's interrupt pending bits.
*******************************************************************************/
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PERIPH_IT(TIMx, TIM_IT));

  /* Clear the IT pending Bit */
  TIMx->SR = (uint16_t)~TIM_IT;
}

/*******************************************************************************
* Function Name  : TI1_Config
* Description    : Configure the TI1 as Input.
*******************************************************************************/
static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0;

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= CCER_CC1E_Reset;

  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;

  /* Select the Input and set the filter */
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr1 |= TIM_ICSelection | (uint16_t)(TIM_ICFilter << 4);

  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_ICPolarity | CCER_CC1E_Set;

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TI2_Config
* Description    : Configure the TI2 as Input.
*******************************************************************************/
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= CCER_CC2E_Reset;

  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 4);

  /* Select the Input and set the filter */
  tmpccmr1 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr1 |= (uint16_t)(TIM_ICFilter << 12);
  tmpccmr1 |= (uint16_t)(TIM_ICSelection << 8);

  /* Select the Polarity and set the CC2E Bit */
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |=  tmp | CCER_CC2E_Set;

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TI3_Config
* Description    : Configure the TI3 as Input.
*******************************************************************************/
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 3: Reset the CC3E Bit */
  TIMx->CCER &= CCER_CC3E_Reset;

  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 8);

  /* Select the Input and set the filter */
  tmpccmr2 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr2 |= TIM_ICSelection | (uint16_t)(TIM_ICFilter << 4);

  /* Select the Polarity and set the CC3E Bit */
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= tmp | CCER_CC3E_Set;

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/*******************************************************************************
* Function Name  : TI4_Config
* Description    : Configure the TI1 as Input.
*******************************************************************************/
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= CCER_CC4E_Reset;

  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 12);

  /* Select the Input and set the filter */
  tmpccmr2 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8) | (uint16_t)(TIM_ICFilter << 12);

  /* Select the Polarity and set the CC4E Bit */
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= tmp | CCER_CC4E_Set;

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
}

