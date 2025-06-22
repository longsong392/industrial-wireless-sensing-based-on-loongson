#ifndef _LS1X_EXTI_H_
#define _LS1X_EXTI_H_
#include "ls1x.h"
#include "Config.h"
#include "test.h"
typedef enum
{
    EXTI_Mode_Level = 0x00,	//电平模式
    EXTI_Mode_Edge  = 0x01		//边沿模式
} EXTIMode_TypeDef;

#define IS_EXTI_MODE(MODE) (((MODE) == EXTI_Mode_Level) || ((MODE) == EXTI_Mode_Edge))

/**
  * @brief  EXTI Trigger enumeration
  */

typedef enum
{
    EXTI_Trigger_Rising_High= 0,
    EXTI_Trigger_Falling_Low = 0x01,
} EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_Trigger_Rising_High) || \
                                  ((TRIGGER) == EXTI_Trigger_Falling_Low))
/**
  * @brief  EXTI Init Structure definition
  */

typedef struct
{
    INT32U EXTI_GPIO;               /*!< Specifies the EXTI lines to be enabled or disabled.
                                         This parameter can be any combination of @ref EXTI_GPIOs */

    EXTIMode_TypeDef EXTI_Mode;       /*!< Specifies the mode for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

    EXTITrigger_TypeDef EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */

    FunctionalState EXTI_GpioCmd;     /*!< Specifies the new state of the selected EXTI lines.
                                         This parameter can be set either to ENABLE or DISABLE */
} EXTI_InitTypeDef;

/** @defgroup EXTI_GPIOs
  * @{
  */
#define EXTI_GPIONONE    ((INT32U)0x00000)  /* No interrupt selected */
#define EXTI_GPIO0       ((INT32U)0x00000001)  /*!< External interrupt line 0 */
#define EXTI_GPIO1       ((INT32U)0x00000002)  /*!< External interrupt line 1 */
#define EXTI_GPIO2       ((INT32U)0x00000004)  /*!< External interrupt line 2 */
#define EXTI_GPIO3       ((INT32U)0x00000008)  /*!< External interrupt line 3 */
#define EXTI_GPIO4       ((INT32U)0x00000010)  /*!< External interrupt line 4 */
#define EXTI_GPIO5       ((INT32U)0x00000020)  /*!< External interrupt line 5 */
#define EXTI_GPIO6       ((INT32U)0x00000040)  /*!< External interrupt line 6 */
#define EXTI_GPIO7       ((INT32U)0x00000080)  /*!< External interrupt line 7 */
#define EXTI_GPIO8       ((INT32U)0x00000100)  /*!< External interrupt line 8 */
#define EXTI_GPIO9       ((INT32U)0x00000200)  /*!< External interrupt line 9 */
#define EXTI_GPIO10      ((INT32U)0x00000400)  /*!< External interrupt line 10 */
#define EXTI_GPIO11      ((INT32U)0x00000800)  /*!< External interrupt line 11 */
#define EXTI_GPIO12      ((INT32U)0x00001000)  /*!< External interrupt line 12 */
#define EXTI_GPIO13      ((INT32U)0x00002000)  /*!< External interrupt line 13 */
#define EXTI_GPIO14      ((INT32U)0x00004000)  /*!< External interrupt line 14 */
#define EXTI_GPIO15      ((INT32U)0x00008000)  /*!< External interrupt line 15 */
#define EXTI_GPIO16      ((INT32U)0x00010000)  /*!< External interrupt line 16 */
#define EXTI_GPIO17      ((INT32U)0x00020000)  /*!< External interrupt line 17 */
#define EXTI_GPIO18      ((INT32U)0x00040000)  /*!< External interrupt line 18 */
#define EXTI_GPIO19      ((INT32U)0x00080000)  /*!< External interrupt line 19 */
#define EXTI_GPIO20      ((INT32U)0x00100000)  /*!< External interrupt line 20 */
#define EXTI_GPIO21      ((INT32U)0x00200000)  /*!< External interrupt line 21 */
#define EXTI_GPIO22      ((INT32U)0x00400000)  /*!< External interrupt line 22 */
#define EXTI_GPIO23      ((INT32U)0x00800000)  /*!< External interrupt line 23 */
#define EXTI_GPIO24      ((INT32U)0x01000000)  /*!< External interrupt line 24 */
#define EXTI_GPIO25      ((INT32U)0x02000000)  /*!< External interrupt line 25 */
#define EXTI_GPIO26      ((INT32U)0x04000000)  /*!< External interrupt line 26 */
#define EXTI_GPIO27      ((INT32U)0x08000000)  /*!< External interrupt line 27 */
#define EXTI_GPIO28      ((INT32U)0x10000000)  /*!< External interrupt line 28 */
#define EXTI_GPIO29      ((INT32U)0x20000000)  /*!< External interrupt line 29 */
#define EXTI_GPIO30      ((INT32U)0x40000000)  /*!< External interrupt line 30 */
#define EXTI_GPIO31      ((INT32U)0x80000000)  /*!< External interrupt line 31 */

#if defined (LS1C102)
#define  IRQ_GPIO_A0        EXTI_GPIO0
#define  IRQ_GPIO_A1        EXTI_GPIO1
#define  IRQ_GPIO_A2        EXTI_GPIO2
#define  IRQ_GPIO_A3        EXTI_GPIO3
#define  IRQ_GPIO_A4        EXTI_GPIO4
#define  IRQ_GPIO_A5        EXTI_GPIO5
#define  IRQ_GPIO_A6        EXTI_GPIO6
#define  IRQ_GPIO_A7        EXTI_GPIO7
#define  IRQ_GPIO_B0        EXTI_GPIO8
#define  IRQ_GPIO_B1        EXTI_GPIO9
#define  IRQ_GPIO_B2        EXTI_GPIO10
#define  IRQ_GPIO_B3        EXTI_GPIO11
#define  IRQ_GPIO_B4        EXTI_GPIO12
#define  IRQ_GPIO_B5        EXTI_GPIO13
#define  IRQ_GPIO_B6        EXTI_GPIO14
#define  IRQ_GPIO_B7        EXTI_GPIO15
#define  IRQ_GPIO_C0        EXTI_GPIO16
#define  IRQ_GPIO_C1        EXTI_GPIO17
#define  IRQ_GPIO_C2        EXTI_GPIO18
#define  IRQ_GPIO_C3        EXTI_GPIO19
#define  IRQ_GPIO_C4        EXTI_GPIO20
#define  IRQ_GPIO_C5        EXTI_GPIO21
#define  IRQ_GPIO_C6        EXTI_GPIO22
#define  IRQ_GPIO_C7        EXTI_GPIO23
#define  IRQ_GPIO_D0        EXTI_GPIO24
#define  IRQ_GPIO_D1        EXTI_GPIO25
#define  IRQ_GPIO_D2        EXTI_GPIO26
#define  IRQ_GPIO_D3        EXTI_GPIO27
#define  IRQ_GPIO_D4        EXTI_GPIO28
#define  IRQ_GPIO_D5        EXTI_GPIO29
#define  IRQ_GPIO_D6        EXTI_GPIO30
#define  IRQ_GPIO_D7        EXTI_GPIO31
#elif defined (LS1C103)
#define  IRQ_GPIO_A0        EXTI_GPIO0
#define  IRQ_GPIO_A1        EXTI_GPIO1
#define  IRQ_GPIO_A2        EXTI_GPIO2
#define  IRQ_GPIO_A3        EXTI_GPIO3
#define  IRQ_GPIO_A4        EXTI_GPIO4
#define  IRQ_GPIO_A5        EXTI_GPIO5
#define  IRQ_GPIO_A6        EXTI_GPIO6
#define  IRQ_GPIO_A7        EXTI_GPIO7
#define  IRQ_GPIO_A8        EXTI_GPIO8
#define  IRQ_GPIO_A9        EXTI_GPIO9
#define  IRQ_GPIO_A10       EXTI_GPIO10
#define  IRQ_GPIO_A11       EXTI_GPIO11
#define  IRQ_GPIO_A12       EXTI_GPIO12
#define  IRQ_GPIO_A13       EXTI_GPIO13
#define  IRQ_GPIO_A14       EXTI_GPIO14
#define  IRQ_GPIO_A15       EXTI_GPIO15
#define  IRQ_GPIO_B0        EXTI_GPIO16
#define  IRQ_GPIO_B1        EXTI_GPIO17
#define  IRQ_GPIO_B2        EXTI_GPIO18
#define  IRQ_GPIO_B3        EXTI_GPIO19
#define  IRQ_GPIO_B4        EXTI_GPIO20
#define  IRQ_GPIO_B5        EXTI_GPIO21
#define  IRQ_GPIO_B6        EXTI_GPIO22
#define  IRQ_GPIO_B7        EXTI_GPIO23
#define  IRQ_GPIO_B8        EXTI_GPIO24
#define  IRQ_GPIO_B9        EXTI_GPIO25
#define  IRQ_GPIO_B10       EXTI_GPIO26
#define  IRQ_GPIO_B11       EXTI_GPIO27
#define  IRQ_GPIO_B12       EXTI_GPIO28
#define  IRQ_GPIO_B13       EXTI_GPIO29
#define  IRQ_GPIO_B14       EXTI_GPIO30
#define  IRQ_GPIO_B15       EXTI_GPIO31


#endif


#define IS_EXTI_GPIO(GPIO) (((GPIO) == EXTI_GPIO0) || ((GPIO) == EXTI_GPIO1) || \
                            ((GPIO) == EXTI_GPIO2) || ((GPIO) == EXTI_GPIO3) || \
                            ((GPIO) == EXTI_GPIO4) || ((GPIO) == EXTI_GPIO5) || \
                            ((GPIO) == EXTI_GPIO6) || ((GPIO) == EXTI_GPIO7) || \
                            ((GPIO) == EXTI_GPIO8) || ((GPIO) == EXTI_GPIO9) || \
                            ((GPIO) == EXTI_GPIO10) || ((GPIO) == EXTI_GPIO11) || \
                            ((GPIO) == EXTI_GPIO12) || ((GPIO) == EXTI_GPIO13) || \
                            ((GPIO) == EXTI_GPIO14) || ((GPIO) == EXTI_GPIO15) || \
                            ((GPIO) == EXTI_GPIO16) || ((GPIO) == EXTI_GPIO17) || \
                            ((GPIO) == EXTI_GPIO18) || ((GPIO) == EXTI_GPIO19) ||\
                            ((GPIO) == EXTI_GPIO20) || ((GPIO) == EXTI_GPIO21) || \
                            ((GPIO) == EXTI_GPIO22) || ((GPIO) == EXTI_GPIO23) || \
                            ((GPIO) == EXTI_GPIO24) || ((GPIO) == EXTI_GPIO25) || \
                            ((GPIO) == EXTI_GPIO26) || ((GPIO) == EXTI_GPIO27) || \
                            ((GPIO) == EXTI_GPIO28) || ((GPIO) == EXTI_GPIO29) || \
                            ((GPIO) == EXTI_GPIO30) || ((GPIO) == EXTI_GPIO31))




/** @defgroup EXTI_Exported_Functions
  * @{
  */
void EXTI_DeInit(EXTI_TypeDef* exti);
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_ClearFlag(EXTI_TypeDef* exti, INT32U EXTI_GPIO);
void EXTI_ClearITPendingBit(EXTI_TypeDef* exti, INT32U EXTI_GPIO);
void EXTI_Init(EXTI_TypeDef* exti, EXTI_InitTypeDef* EXTI_InitStruct);

FlagStatus EXTI_GetFlagStatus(EXTI_TypeDef* exti, INT32U EXTI_GPIO);

ITStatus EXTI_GetITStatus(EXTI_TypeDef* exti, INT32U EXTI_GPIO);




#endif /* INCLUDE_EXTI_H_ */

