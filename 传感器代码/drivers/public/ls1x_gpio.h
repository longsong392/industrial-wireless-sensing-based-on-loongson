#ifndef _LS1X_GPIO_H_
#define _LS1X_GPIO_H_

#include "ls1x.h"
#include "test.h"

/**
 * @说明 判断是否为GPIO外设
 */
#define IS_GPIO(PERIPH) (((PERIPH) == GPIOA) || ((PERIPH) == GPIOB))

#define GPIO_Pin_0                 ((uint32_t)0x00000001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint32_t)0x00000002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint32_t)0x00000004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x00000008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x00000010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x00000020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x00000040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x00000080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint32_t)0x00000100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint32_t)0x00000200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint32_t)0x00000400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint32_t)0x00000800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint32_t)0x00001000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint32_t)0x00002000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint32_t)0x00004000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint32_t)0x00008000)  /*!< Pin 15 selected */
#define GPIO_Pin_16                ((uint32_t)0x00010000)  /*!< Pin 16 selected */
#define GPIO_Pin_17                ((uint32_t)0x00020000)  /*!< Pin 17 selected */
#define GPIO_Pin_18                ((uint32_t)0x00040000)  /*!< Pin 18 selected */
#define GPIO_Pin_19                ((uint32_t)0x00080000)  /*!< Pin 19 selected */
#define GPIO_Pin_20                ((uint32_t)0x00100000)  /*!< Pin 20 selected */
#define GPIO_Pin_21                ((uint32_t)0x00200000)  /*!< Pin 21 selected */
#define GPIO_Pin_22                ((uint32_t)0x00400000)  /*!< Pin 22 selected */
#define GPIO_Pin_23                ((uint32_t)0x00800000)  /*!< Pin 23 selected */
#define GPIO_Pin_24                ((uint32_t)0x01000000)  /*!< Pin 24 selected */
#define GPIO_Pin_25                ((uint32_t)0x02000000)  /*!< Pin 25 selected */
#define GPIO_Pin_26                ((uint32_t)0x04000000)  /*!< Pin 26 selected */
#define GPIO_Pin_27                ((uint32_t)0x08000000)  /*!< Pin 27 selected */
#define GPIO_Pin_28                ((uint32_t)0x10000000)  /*!< Pin 28 selected */
#define GPIO_Pin_29                ((uint32_t)0x20000000)  /*!< Pin 29 selected */
#define GPIO_Pin_30                ((uint32_t)0x40000000)  /*!< Pin 30 selected */
#define GPIO_Pin_31                ((uint32_t)0x80000000)  /*!< Pin 31 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFFFFFF)  /*!< All pins selected */
#define IS_GPIO_PIN(PIN)		(((PIN) == GPIO_Pin_0) || \
                                 ((PIN) == GPIO_Pin_1) || \
                                 ((PIN) == GPIO_Pin_2) || \
                                 ((PIN) == GPIO_Pin_3) || \
                                 ((PIN) == GPIO_Pin_4) || \
                                 ((PIN) == GPIO_Pin_5) || \
                                 ((PIN) == GPIO_Pin_6) || \
                                 ((PIN) == GPIO_Pin_7) || \
                                 ((PIN) == GPIO_Pin_8) || \
                                 ((PIN) == GPIO_Pin_9) || \
                                 ((PIN) == GPIO_Pin_10) || \
                                 ((PIN) == GPIO_Pin_11) || \
                                 ((PIN) == GPIO_Pin_12) || \
                                 ((PIN) == GPIO_Pin_13) || \
                                 ((PIN) == GPIO_Pin_14) || \
                                 ((PIN) == GPIO_Pin_15)	|| \
                                 ((PIN) == GPIO_Pin_16) || \
                                 ((PIN) == GPIO_Pin_17) || \
                                 ((PIN) == GPIO_Pin_18) || \
                                 ((PIN) == GPIO_Pin_19) || \
                                 ((PIN) == GPIO_Pin_20) || \
                                 ((PIN) == GPIO_Pin_21) || \
                                 ((PIN) == GPIO_Pin_22) || \
                                 ((PIN) == GPIO_Pin_23) || \
                                 ((PIN) == GPIO_Pin_24) || \
                                 ((PIN) == GPIO_Pin_25) || \
                                 ((PIN) == GPIO_Pin_26) || \
                                 ((PIN) == GPIO_Pin_27) || \
                                 ((PIN) == GPIO_Pin_28) || \
                                 ((PIN) == GPIO_Pin_29) || \
                                 ((PIN) == GPIO_Pin_30) || \
                                 ((PIN) == GPIO_Pin_31))



enum GPIO_PIN
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_16,
    GPIO_PIN_17,
    GPIO_PIN_18,
    GPIO_PIN_19,
    GPIO_PIN_20,
    GPIO_PIN_21,
    GPIO_PIN_22,
    GPIO_PIN_23,
    GPIO_PIN_24,
    GPIO_PIN_25,
    GPIO_PIN_26,
    GPIO_PIN_27,
    GPIO_PIN_28,
    GPIO_PIN_29,
    GPIO_PIN_30,
    GPIO_PIN_31,
    GPIO_PIN_32,
    GPIO_PIN_33,
    GPIO_PIN_34,
    GPIO_PIN_35,
    GPIO_PIN_36,
    GPIO_PIN_37,
    GPIO_PIN_38,
    GPIO_PIN_39,
    GPIO_PIN_40,
    GPIO_PIN_41,
    GPIO_PIN_42,
    GPIO_PIN_43,
    GPIO_PIN_44,
    GPIO_PIN_45,
    GPIO_PIN_46,
    GPIO_PIN_47,
    GPIO_PIN_48,
    GPIO_PIN_49,
    GPIO_PIN_50,
    GPIO_PIN_51,
    GPIO_PIN_52,
    GPIO_PIN_53,
    GPIO_PIN_54,
    GPIO_PIN_55,
    GPIO_PIN_56,
    GPIO_PIN_57,
    GPIO_PIN_58,
    GPIO_PIN_59,
    GPIO_PIN_60,
    GPIO_PIN_61,
    GPIO_PIN_62,
    GPIO_PIN_63,
    GPIO_PIN_MAX
};


typedef enum
{
    GPIO_FUNC_GPIO = 0,
    GPIO_FUNC_MAIN,
    GPIO_FUNC_FIRST,
    GPIO_FUNC_SECOND
} GPIOFunc_TypeDef;

/**
  * @说明   管脚状态枚举
  */
typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} BitAction;
#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

#if defined (LS1C102) /***************LS1C102************************/
typedef enum
{
    GPIO_Mode_In = 0,
    GPIO_Mode_Out
} GPIOMode_TypeDef;

/**
 * @说明  GPIO 初始化定义
 */
typedef struct
{
    INT8U GPIO_Pin;
    GPIOMode_TypeDef GPIO_Mode;
    GPIOFunc_TypeDef GPIO_Func;
} GPIO_InitTypeDef;

//#define 	GPIO_HIGH      1
//#define 	GPIO_LOW       0
//#define 	GPIO_INPUT     1
//#define 	GPIO_OUTPUT    0


INT8U 	gpio_get_pin(INT8U pin);
void 	gpio_write_pin(INT8U pin,INT8U mode);
void 	gpio_pin_remap(INT8U pin,INT8U func);
void 	gpio_set_direction(INT8U pin,INT8U mode);
void GPIO_Init(GPIO_InitTypeDef* GPIO_InitStruct) ;

void gpio_set_InputStatus(INT8U pin , INT8U func)  ;

#elif defined (LS1C103) /***************LS1C103************************/
typedef enum
{
    GPIO_Speed_10MHz = 1,
    GPIO_Speed_2MHz,
    GPIO_Speed_50MHz
} GPIOSpeed_TypeDef;

#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

/* Configuration Mode enumeration --------------------------------------------*/
typedef enum
{
    GPIO_Mode_AIN = 0x0,
    GPIO_Mode_IN_FLOATING = 0x04,
    GPIO_Mode_IPD = 0x28,
    GPIO_Mode_IPU = 0x48,
    GPIO_Mode_Out_OD = 0x14,
    GPIO_Mode_Out_PP = 0x10,
    GPIO_Mode_AF_OD = 0x1C,
    GPIO_Mode_AF_PP = 0x18
} GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))

/* GPIO Init structure definition */
typedef struct
{
    uint16_t GPIO_Pin;
    GPIOSpeed_TypeDef GPIO_Speed;
    GPIOMode_TypeDef GPIO_Mode;
} GPIO_InitTypeDef;

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif



//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
//void GPIO_BitInit(BITIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
//uint8_t GPIO_ReadInputDataBit(BITIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
//uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
//uint8_t GPIO_ReadOutputDataBit(BITIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
//uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
//void GPIO_WriteBit(BITIO_TypeDef* GPIOx, uint32_t GPIO_Pin, BitAction Value);
//void GPIO_WriteData(GPIO_TypeDef* GPIOx, uint32_t Value);
void AFIO_RemapConfig(AFIO_TypeDef* AFIOx, uint16_t GPIO_Pin, GPIOFunc_TypeDef GPIO_Func);

/****only 103 till this***/


#endif



