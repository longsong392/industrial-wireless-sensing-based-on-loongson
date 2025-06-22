#ifndef _LS1X_CLOCK_H_
#define _LS1X_CLOCK_H_

#include "ls1x.h"


typedef struct
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK/cpu clock frequency expressed in Hz */
  uint32_t PCLK_Frequency;    /*!< returns PCLK/power clock frequency expressed in Hz */
  uint32_t BUSCLK_Frequency;  /*!< returns BUSCLK clock frequency expressed in Hz */

}CLOCKS_TypeDef;


/* ------------ timing registers bit define  ----------- */

/** @defgroup Timming_configuration
  *
  */
#define CLOCK_HSE_FAIL                        	((uint32_t)0x80000000)
#define CLOCK_HSE_SELECT_STATUS               	((uint32_t)0x40000000)
#define CLOCK_LSE_FAIL                        	((uint32_t)0x20000000)
#define CLOCK_LSE_SELECT_STATUS               	((uint32_t)0x10000000)

#define CLOCK_TURBO_OFF                         ((uint32_t)0x00000000)
#define CLOCK_TURBO_ON                          ((uint32_t)0x08000000)
#define CLOCK_TURBO_MASK                        ((uint32_t)0x08000000)

#define CLOCK_FASTEN_OFF                        ((uint32_t)0x00000000)
#define CLOCK_FASTEN_ON                         ((uint32_t)0x00000800)
#define CLOCK_FASTEN_MASK                       ((uint32_t)0x00000800)
#define CLOCK_SYSCLK_HSE_SELECT		      	  	((uint32_t)0x00000080)
#define CLOCK_SYSCLK_HSE_EN		      	  	    ((uint32_t)0x00000040)
#define CLOCK_LSE_SELECT	          		  	((uint32_t)0x00000020)
#define CLOCK_LSI_SPEED_32K	          			((uint32_t)0x00000000)
#define CLOCK_LSI_SPEED_1K	          			((uint32_t)0x00000010)
#define CLOCK_LSI_SPEED_MASK	          		((uint32_t)0x00000010)
/* 内部32K OSC Trimming 值*/
#define CLOCK_LSE_TRIM_15	          			((uint32_t)0x0000000F)
#define CLOCK_LSE_TRIM_14	          			((uint32_t)0x0000000E)
#define CLOCK_LSE_TRIM_13	          			((uint32_t)0x0000000D)
#define CLOCK_LSE_TRIM_12	          			((uint32_t)0x0000000C)
#define CLOCK_LSE_TRIM_11	          			((uint32_t)0x0000000B)
#define CLOCK_LSE_TRIM_10	          			((uint32_t)0x0000000A)
#define CLOCK_LSE_TRIM_9	          			((uint32_t)0x00000009)
#define CLOCK_LSE_TRIM_8	          			((uint32_t)0x00000008)
#define CLOCK_LSE_TRIM_7	         			((uint32_t)0x00000007)
#define CLOCK_LSE_TRIM_6	          			((uint32_t)0x00000006)
#define CLOCK_LSE_TRIM_5	          			((uint32_t)0x00000005)
#define CLOCK_LSE_TRIM_4	          			((uint32_t)0x00000004)
#define CLOCK_LSE_TRIM_3	          			((uint32_t)0x00000003)
#define CLOCK_LSE_TRIM_2	          			((uint32_t)0x00000002)
#define CLOCK_LSE_TRIM_1	          			((uint32_t)0x00000001)
#define CLOCK_LSE_TRIM_0	          			((uint32_t)0x00000000)
#define CLOCK_LSE_TRIM_MASK	      				((uint32_t)0x0000000F)


/* 高速晶振开启到开始测量的延迟 */
#define CLOCK_START_CLK_HS_5140US	      	  	((uint32_t)0x00000000)
#define CLOCK_START_CLK_HS_480US	          	((uint32_t)0x00000100)
#define CLOCK_START_CLK_HS_1460US	      	  	((uint32_t)0x00000200)
#define CLOCK_START_CLK_HS_2440US	      	  	((uint32_t)0x00000300)
#define CLOCK_START_CLK_HS_MASK	      			((uint32_t)0x00000300)


/** @defgroup HSE_configuration
  * @{
  */
#define CLOCK_HSE_OFF                           ((uint32_t)0x00000000)
#define CLOCK_HSE_ON                            CLOCK_SYSCLK_HSE_SELECT
#define IS_CLOCK_HSE(HSE)                       (((HSE) == CLOCK_HSE_OFF) || ((HSE) == CLOCK_HSE_ON))



/** @defgroup HSE_configuration
  * @{
  */
#define CLOCK_LSE_OFF                                ((uint32_t)0x00000000)
#define CLOCK_LSE_ON                                 CLOCK_LSE_SELECT
#define IS_CLOCK_LSE(LSE)                            (((HSE) == CLOCK_LSE_OFF) || ((HSE) == CLOCK_LSE_ON) )


/** @defgroup HSE_configuration
  * @{
  */
#define CLOCK_HSI_OFF                          CLOCK_SYSCLK_HSE_SELECT
#define CLOCK_HSI_ON                           ((uint32_t)0x00000000)
#define IS_CLOCK_HSI(LSE) (((HSE) == CLOCK_HSI_OFF) || ((HSE) == CLOCK_HSI_ON) )

/** @defgroup HSE_configuration
  * @{
  */
#define CLOCK_LSI_OFF                            CLOCK_LSE_SELECT
#define CLOCK_LSI_ON                             ((uint32_t)0x00000000)
#define IS_CLOCK_LSI(LSE) (((HSE) == CLOCK_LSI_OFF) || ((HSE) == CLOCK_LSI_ON) )


#define CLOCK_FLAG_HSERDY               		((uint32_t)0x40000000)
#define CLOCK_FLAG_HSEFAIL                   	((uint32_t)0x80000000)
#define CLOCK_FLAG_LSERDY               		((uint32_t)0x10000000)
#define CLOCK_FLAG_LSEFAIL                   	((uint32_t)0x20000000)
#define IS_CLOCK_FLAG(FLAG)                     (((FLAG) == CLOCK_FLAG_HSERDY) || ((FLAG) == CLOCK_FLAG_HSEFAIL) \
							                    ((FLAG) == CLOCK_FLAG_LSERDY) || ((FLAG) == CLOCK_FLAG_LSEFAIL))

/** @defgroup RCC_Oscillator_Type Oscillator Type
  * @{
  */
#define CLOCK_OSCILLATORTYPE_NONE            (0x00000000U)
#define CLOCK_OSCILLATORTYPE_HSE             (0x00000001U)
#define CLOCK_OSCILLATORTYPE_HSI             (0x00000002U)
#define CLOCK_OSCILLATORTYPE_LSE             (0x00000004U)
#define CLOCK_OSCILLATORTYPE_LSI             (0x00000008U)
#define IS_CLOCK_OSCILLATOR_TYPE(TYPE)       (((TYPE) == CLOCK_OSCILLATORTYPE_HSE) || ((TYPE) == CLOCK_OSCILLATORTYPE_HSI)||\
										     ((TYPE) == CLOCK_OSCILLATORTYPE_LSE) || ((TYPE) == CLOCK_OSCILLATORTYPE_LSI)||\
									        	((TYPE) == CLOCK_OSCILLATORTYPE_NONE))


typedef struct
{
	  uint32_t OscillatorType;        /* The oscillators to be configured.
	                                       This parameter can be a value of @ref RCC_Oscillator_Type */

	  uint32_t HSEState;              /*  The new state of the HSE.
	                                       This parameter can be a value of @ref RCC_HSE_Config */

	  uint32_t LSEState;              /*  The new state of the LSE.
	                                       This parameter can be a value of @ref RCC_LSE_Config */
	  uint32_t LSISpeed;              /*  The speed of the LSI.
	                                       This parameter can be a value of @ref RCC_LSI_Config */
	  uint32_t StartClkHS;			  /*   the delay time before external clock start work*/

	  uint32_t FastEn;			  /*   the delay time before external clock start work*/
	  
	  uint32_t TurboEn;			  /*   the delay time before external clock start work*/

}CLOCK_InitTypeDef;

#define CLOCK_delayUs(n)											\
	do {															\
		unsigned d = ((n*118)>>7)+1;								\
		while(d--)													\
		{															\
			asm volatile("nop;nop;nop;nop");						\
		}															\
	} while(0)

#define CLOCK_delayMs(n)			CLOCK_delayUs(1013*n)

void CLOCK_StructInit(CLOCK_InitTypeDef  *CLOCK_InitStruct);

uint32_t CLOCK_GetClocksFreq(void);
uint32_t CLOCK_IramToDramEn(void);
uint32_t SystemClockInit(void);

ErrorStatus CLOCK_Init(CLOCK_InitTypeDef  *CLOCK_InitStruct);
ErrorStatus CLOCK_HSEConfig(uint32_t CLOCK_HSE);
ErrorStatus CLOCK_LSEConfig(uint32_t CLOCK_LSE);

FlagStatus CLOCK_GetFlagStatus(uint32_t CLOCK_FLAG);

#define CLOCK_EXT32k_ENABLE	0x02
void CLOCK_checkSelect(unsigned char enable);


#endif
