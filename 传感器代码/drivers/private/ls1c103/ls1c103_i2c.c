/* Includes ------------------------------------------------------------------*/
#include "ls1x.h"
#include "ls1c103_pmu.h"
#include "ls1c103_i2c.h"
	 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C ENGC mask */
#define CR1_ENGC_Set            ((uint16_t)0x0040)
#define CR1_ENGC_Reset          ((uint16_t)0xFFBF)

/* I2C SWRST mask */
#define CR1_SWRST_Set           ((uint16_t)0x8000)
#define CR1_SWRST_Reset         ((uint16_t)0x7FFF)

/* I2C PEC mask */
#define CR1_PEC_Set             ((uint16_t)0x1000)
#define CR1_PEC_Reset           ((uint16_t)0xEFFF)

/* I2C ENPEC mask */
#define CR1_ENPEC_Set           ((uint16_t)0x0020)
#define CR1_ENPEC_Reset         ((uint16_t)0xFFDF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((uint16_t)0x0010)
#define CR1_ENARP_Reset         ((uint16_t)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((uint16_t)0x0080)
#define CR1_NOSTRETCH_Reset     ((uint16_t)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((uint16_t)0x0800)
#define CR2_DMAEN_Reset         ((uint16_t)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((uint16_t)0x1000)
#define CR2_LAST_Reset          ((uint16_t)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((uint16_t)0x0001)
#define OAR1_ADD0_Reset         ((uint16_t)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((uint16_t)0x0001)
#define OAR2_ENDUAL_Reset       ((uint16_t)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((uint16_t)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((uint16_t)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((uint32_t)0x07000000)

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : I2C_DeInit
* Description    : Deinitializes the I2Cx peripheral registers to their default
*                  reset values.
*******************************************************************************/
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  switch (*(uint32_t*)&I2Cx)
  {
    case I2C1_BASE:
      pmu_rst_cmd(RCC_I2C,ENABLE);
      pmu_rst_cmd(RCC_I2C,DISABLE);
      break;

    default:
      break;
  }
}

/*******************************************************************************
* Function Name  : I2C_Init
* Description    : Initializes the I2Cx peripheral according to the specified 
*                  parameters in the I2C_InitStruct.
******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
  uint16_t tmpreg = 0, freqrange = 0;
  uint16_t result = 0x04;
  uint32_t pclk1 = 8000000;
  //RCC_ClocksTypeDef  rcc_clocks;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_MODE(I2C_InitStruct->I2C_Mode));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_InitStruct->I2C_DutyCycle));
  assert_param(IS_I2C_OWN_ADDRESS1(I2C_InitStruct->I2C_OwnAddress1));
  assert_param(IS_I2C_ACK_STATE(I2C_InitStruct->I2C_Ack));
  assert_param(IS_I2C_ACKNOWLEDGE_ADDRESS(I2C_InitStruct->I2C_AcknowledgedAddress));
  assert_param(IS_I2C_CLOCK_SPEED(I2C_InitStruct->I2C_ClockSpeed));

/*---------------------------- I2Cx CR2 Configuration ------------------------*/
  /* Get the I2Cx CR2 value */
  tmpreg = I2Cx->CR2;
  /* Clear frequency FREQ[5:0] bits */
  tmpreg &= CR2_FREQ_Reset;
  /* Get pclk1 frequency value */
  //RCC_GetClocksFreq(&rcc_clocks);
  //pclk1 = rcc_clocks.PCLK1_Frequency;
  pclk1 = 11111111; // 11.111. MHz 90ns period
  /* Set frequency bits depending on pclk1 value */
  freqrange = (uint16_t)(pclk1 / 1000000);
  tmpreg |= freqrange;
  /* Write to I2Cx CR2 */
  I2Cx->CR2 = tmpreg;

/*---------------------------- I2Cx CCR Configuration ------------------------*/
  /* Disable the selected I2C peripheral to configure TRISE */
  I2Cx->CR1 &= CR1_PE_Reset;

  /* Reset tmpreg value */
  /* Clear F/S, DUTY and CCR[11:0] bits */
  tmpreg = 0;

  /* Configure speed in standard mode */
  if (I2C_InitStruct->I2C_ClockSpeed <= 100000)
  {
    /* Standard mode speed calculate */
    result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed << 1));
    /* Test if CCR value is under 0x4*/
    if (result < 0x04)
    {
      /* Set minimum allowed value */
      result = 0x04;  
    }
    /* Set speed value for standard mode */
    tmpreg |= result;	  
    /* Set Maximum Rise Time for standard mode */
    I2Cx->TRISE = freqrange + 1; 
  }
  /* Configure speed in fast mode */
  else /*(I2C_InitStruct->I2C_ClockSpeed <= 400000)*/
  {
    if (I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 3));
    }
    else /*I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_16_9*/
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
      /* Set DUTY bit */
      result |= I2C_DutyCycle_16_9;
    }
    /* Test if CCR value is under 0x1*/
    if ((result & CCR_CCR_Set) == 0)
    {
      /* Set minimum allowed value */
      result |= (uint16_t)0x0001;  
    }
    /* Set speed value and set F/S bit for fast mode */
    tmpreg |= result | CCR_FS_Set;
    /* Set Maximum Rise Time for fast mode */
    I2Cx->TRISE = (uint16_t)(((freqrange * 300) / 1000) + 1);  
  }
  /* Write to I2Cx CCR */
  I2Cx->CCR = tmpreg;

  /* Enable the selected I2C peripheral */
  I2Cx->CR1 |= CR1_PE_Set;

/*---------------------------- I2Cx CR1 Configuration ------------------------*/
  /* Get the I2Cx CR1 value */
  tmpreg = I2Cx->CR1;
  /* Clear ACK, SMBTYPE and  SMBUS bits */
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure I2Cx: mode and acknowledgement */
  /* Set SMBTYPE and SMBUS bits according to I2C_Mode value */
  /* Set ACK bit according to I2C_Ack value */
  tmpreg |= (uint16_t)((uint32_t)I2C_InitStruct->I2C_Mode | I2C_InitStruct->I2C_Ack);
  /* Write to I2Cx CR1 */
  I2Cx->CR1 = tmpreg;

/*---------------------------- I2Cx OAR1 Configuration -----------------------*/
  /* Set I2Cx Own Address1 and acknowledged address */
  I2Cx->OAR1 = (I2C_InitStruct->I2C_AcknowledgedAddress | I2C_InitStruct->I2C_OwnAddress1);
}

/*******************************************************************************
* Function Name  : I2C_StructInit
* Description    : Fills each I2C_InitStruct member with its default value.
*******************************************************************************/
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
/*---------------- Reset I2C init structure parameters values ----------------*/
  /* Initialize the I2C_Mode member */
  I2C_InitStruct->I2C_Mode = I2C_Mode_I2C;

  /* Initialize the I2C_DutyCycle member */
  I2C_InitStruct->I2C_DutyCycle = I2C_DutyCycle_2;

  /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStruct->I2C_OwnAddress1 = 0;

  /* Initialize the I2C_Ack member */
  I2C_InitStruct->I2C_Ack = I2C_Ack_Disable;

  /* Initialize the I2C_AcknowledgedAddress member */
  I2C_InitStruct->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  /* initialize the I2C_ClockSpeed member */
  I2C_InitStruct->I2C_ClockSpeed = 5000;
}

/*******************************************************************************
* Function Name  : I2C_Cmd
* Description    : Enables or disables the specified I2C peripheral.
*******************************************************************************/
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral */
    I2Cx->CR1 |= CR1_PE_Set;
  }
  else
  {
    /* Disable the selected I2C peripheral */
    I2Cx->CR1 &= CR1_PE_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_DMACmd
* Description    : Enables or disables the specified I2C DMA requests.
*******************************************************************************/
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C DMA requests */
    I2Cx->CR2 |= CR2_DMAEN_Set;
  }
  else
  {
    /* Disable the selected I2C DMA requests */
    I2Cx->CR2 &= CR2_DMAEN_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_DMALastTransferCmd
* Description    : Specifies that the next DMA transfer is the last one.
*******************************************************************************/
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Next DMA transfer is the last transfer */
    I2Cx->CR2 |= CR2_LAST_Set;
  }
  else
  {
    /* Next DMA transfer is not the last transfer */
    I2Cx->CR2 &= CR2_LAST_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_GenerateSTART
* Description    : Generates I2Cx communication START condition.
*******************************************************************************/
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a START condition */
    I2Cx->CR1 |= CR1_START_Set;
  }
  else
  {
    /* Disable the START condition generation */
    I2Cx->CR1 &= CR1_START_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_GenerateSTOP
* Description    : Generates I2Cx communication STOP condition.
*******************************************************************************/
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a STOP condition */
    I2Cx->CR1 |= CR1_STOP_Set;
  }
  else
  {
    /* Disable the STOP condition generation */
    I2Cx->CR1 &= CR1_STOP_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_AcknowledgeConfig
* Description    : Enables or disables the specified I2C acknowledge feature.
*******************************************************************************/
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the acknowledgement */
    I2Cx->CR1 |= CR1_ACK_Set;
  }
  else
  {
    /* Disable the acknowledgement */
    I2Cx->CR1 &= CR1_ACK_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_OwnAddress2Config
* Description    : Configures the specified I2C own address2.
*******************************************************************************/
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address)
{
  uint16_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Get the old register value */
  tmpreg = I2Cx->OAR2;
  /* Reset I2Cx Own address2 bit [7:1] */
  tmpreg &= OAR2_ADD2_Reset;
  /* Set I2Cx Own address2 */
  tmpreg |= (uint16_t)(Address & (uint16_t)0x00FE);
  /* Store the new register value */
  I2Cx->OAR2 = tmpreg;
}

/*******************************************************************************
* Function Name  : I2C_DualAddressCmd
* Description    : Enables or disables the specified I2C dual addressing mode.
*******************************************************************************/
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable dual addressing mode */
    I2Cx->OAR2 |= OAR2_ENDUAL_Set;
  }
  else
  {
    /* Disable dual addressing mode */
    I2Cx->OAR2 &= OAR2_ENDUAL_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_GeneralCallCmd
* Description    : Enables or disables the specified I2C general call feature.
*******************************************************************************/
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable generall call */
    I2Cx->CR1 |= CR1_ENGC_Set;
  }
  else
  {
    /* Disable generall call */
    I2Cx->CR1 &= CR1_ENGC_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_ITConfig
* Description    : Enables or disables the specified I2C interrupts.
*******************************************************************************/
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_I2C_CONFIG_IT(I2C_IT));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts */
    I2Cx->CR2 |= I2C_IT;
  }
  else
  {
    /* Disable the selected I2C interrupts */
    I2Cx->CR2 &= (uint16_t)~I2C_IT;
  }
}

/*******************************************************************************
* Function Name  : I2C_SendData
* Description    : Sends a data byte through the I2Cx peripheral.
*******************************************************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Write in the DR register the data to be sent */
  I2Cx->DR = Data;
}

/*******************************************************************************
* Function Name  : I2C_ReceiveData
* Description    : Returns the most recent received data by the I2Cx peripheral.
*******************************************************************************/
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Return the data in the DR register */
  return (uint8_t)I2Cx->DR;
}

/*******************************************************************************
* Function Name  : I2C_Send7bitAddress
* Description    : Transmits the address byte to select the slave device.
*******************************************************************************/
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DIRECTION(I2C_Direction));

  /* Test on the direction to set/reset the read/write bit */
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* Set the address bit0 for read */
    Address |= OAR1_ADD0_Set;
  }
  else
  {
    /* Reset the address bit0 for write */
    Address &= OAR1_ADD0_Reset;
  }
  /* Send the address */
  I2Cx->DR = Address;
}

/*******************************************************************************
* Function Name  : I2C_ReadRegister
* Description    : Reads the specified I2C register and returns its value.
*******************************************************************************/
uint16_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_REGISTER(I2C_Register));

  /* Return the selected register value */
  return (*(vuint16_t *)(*((vuint32_t *)&I2Cx) + I2C_Register));
}

/*******************************************************************************
* Function Name  : I2C_SoftwareResetCmd
* Description    : Enables or disables the specified I2C software reset.
*******************************************************************************/
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Peripheral under reset */
    I2Cx->CR1 |= CR1_SWRST_Set;
  }
  else
  {
    /* Peripheral not under reset */
    I2Cx->CR1 &= CR1_SWRST_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_SMBusAlertConfig
* Description    : Drives the SMBusAlert pin high or low for the specified I2C.
*******************************************************************************/
void I2C_SMBusAlertConfig(I2C_TypeDef* I2Cx, uint16_t I2C_SMBusAlert)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_SMBUS_ALERT(I2C_SMBusAlert));

  if (I2C_SMBusAlert == I2C_SMBusAlert_Low)
  {
    /* Drive the SMBusAlert pin Low */
    I2Cx->CR1 |= I2C_SMBusAlert_Low;
  }
  else
  {
    /* Drive the SMBusAlert pin High  */
    I2Cx->CR1 &= I2C_SMBusAlert_High;
  }
}

/*******************************************************************************
* Function Name  : I2C_TransmitPEC
* Description    : Enables or disables the specified I2C PEC transfer.
*******************************************************************************/
void I2C_TransmitPEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C PEC transmission */
    I2Cx->CR1 |= CR1_PEC_Set;
  }
  else
  {
    /* Disable the selected I2C PEC transmission */
    I2Cx->CR1 &= CR1_PEC_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_PECPositionConfig
* Description    : Selects the specified I2C PEC position.
*******************************************************************************/
void I2C_PECPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_PECPosition)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_PEC_POSITION(I2C_PECPosition));

  if (I2C_PECPosition == I2C_PECPosition_Next)
  {
    /* Next byte in shift register is PEC */
    I2Cx->CR1 |= I2C_PECPosition_Next;
  }
  else
  {
    /* Current byte in shift register is PEC */
    I2Cx->CR1 &= I2C_PECPosition_Current;
  }
}

/*******************************************************************************
* Function Name  : I2C_CalculatePEC
* Description    : Enables or disables the PEC value calculation of the
*                  transfered bytes.
*******************************************************************************/
void I2C_CalculatePEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C PEC calculation */
    I2Cx->CR1 |= CR1_ENPEC_Set;
  }
  else
  {
    /* Disable the selected I2C PEC calculation */
    I2Cx->CR1 &= CR1_ENPEC_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_GetPEC
* Description    : Returns the PEC value for the specified I2C.
*******************************************************************************/
uint8_t I2C_GetPEC(I2C_TypeDef* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Return the selected I2C PEC value */
  return ((I2Cx->SR2) >> 8);
}

/*******************************************************************************
* Function Name  : I2C_ARPCmd
* Description    : Enables or disables the specified I2C ARP.
*******************************************************************************/
void I2C_ARPCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C ARP */
    I2Cx->CR1 |= CR1_ENARP_Set;
  }
  else
  {
    /* Disable the selected I2C ARP */
    I2Cx->CR1 &= CR1_ENARP_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_StretchClockCmd
* Description    : Enables or disables the specified I2C Clock stretching.
*******************************************************************************/
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState == DISABLE)
  {
    /* Enable the selected I2C Clock stretching */
    I2Cx->CR1 |= CR1_NOSTRETCH_Set;
  }
  else
  {
    /* Disable the selected I2C Clock stretching */
    I2Cx->CR1 &= CR1_NOSTRETCH_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_FastModeDutyCycleConfig
* Description    : Selects the specified I2C fast mode duty cycle.
*******************************************************************************/
void I2C_FastModeDutyCycleConfig(I2C_TypeDef* I2Cx, uint16_t I2C_DutyCycle)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_DutyCycle));

  if (I2C_DutyCycle != I2C_DutyCycle_16_9)
  {
    /* I2C fast mode Tlow/Thigh=2 */
    I2Cx->CCR &= I2C_DutyCycle_2;
  }
  else
  {
    /* I2C fast mode Tlow/Thigh=16/9 */
    I2Cx->CCR |= I2C_DutyCycle_16_9;
  }
}

/*******************************************************************************
* Function Name  : I2C_GetLastEvent
* Description    : Returns the last I2Cx Event.
*******************************************************************************/
uint32_t I2C_GetLastEvent(I2C_TypeDef* I2Cx)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* Read the I2Cx status register */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Return status */
  return lastevent;
}

/*******************************************************************************
* Function Name  : I2C_CheckEvent
* Description    : Checks whether the last I2Cx Event is equal to the one passed
*                  as parameter.
*******************************************************************************/
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* Read the I2Cx status register */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Check whether the last event is equal to I2C_EVENT */
  if (lastevent == I2C_EVENT )
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }

  /* Return status */
  return status;
}

/*******************************************************************************
* Function Name  : I2C_GetFlagStatus
* Description    : Checks whether the specified I2C flag is set or not.
*******************************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint32_t i2creg = 0, i2cxbase = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

  /* Get the I2Cx peripheral base address */
  i2cxbase = (*(uint32_t*)&(I2Cx));
  
  /* Read flag register index */
  i2creg = I2C_FLAG >> 28;
  
  /* Get bit[23:0] of the flag */
  I2C_FLAG &= FLAG_Mask;
  
  if(i2creg != 0)
  {
    /* Get the I2Cx SR1 register address */
    i2cxbase += 0x14;
  }
  else
  {
    /* Flag in I2Cx SR2 Register */
    I2C_FLAG = (uint32_t)(I2C_FLAG >> 16);
    /* Get the I2Cx SR2 register address */
    i2cxbase += 0x18;
  }
  
  if(((*(vuint32_t *)i2cxbase) & I2C_FLAG) != (uint32_t)RESET)
  {
    /* I2C_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset */
    bitstatus = RESET;
  }
  
  /* Return the I2C_FLAG status */
  return  bitstatus;
}

/*******************************************************************************
* Function Name  : I2C_ClearFlag
* Description    : Clears the I2Cx's pending flags.
*******************************************************************************/
void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
  uint32_t flagpos = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_FLAG(I2C_FLAG));

  /* Get the I2C flag position */
  flagpos = I2C_FLAG & FLAG_Mask;

  /* Clear the selected I2C flag */
  I2Cx->SR1 = (uint16_t)~flagpos;
}

/*******************************************************************************
* Function Name  : I2C_GetITStatus
* Description    : Checks whether the specified I2C interrupt has occurred or not.
*******************************************************************************/
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_IT(I2C_IT));

  /* Check if the interrupt source is enabled or not */
  enablestatus = (uint32_t)(((I2C_IT & ITEN_Mask) >> 16) & (I2Cx->CR2)) ;  

  /* Get bit[23:0] of the flag */
  I2C_IT &= FLAG_Mask;

  /* Check the status of the specified I2C flag */
  if (((I2Cx->SR1 & I2C_IT) != (uint32_t)RESET) && enablestatus)
  {
    /* I2C_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_IT is reset */
    bitstatus = RESET;
  }
  /* Return the I2C_IT status */
  return  bitstatus;
}

/*******************************************************************************
* Function Name  : I2C_ClearITPendingBit
* Description    : Clears the I2Cx?s interrupt pending bits.
*******************************************************************************/
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint32_t I2C_IT)
{
  uint32_t flagpos = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_IT(I2C_IT));

  /* Get the I2C flag position */
  flagpos = I2C_IT & FLAG_Mask;

  /* Clear the selected I2C flag */
  I2Cx->SR1 = (uint16_t)~flagpos;
}

