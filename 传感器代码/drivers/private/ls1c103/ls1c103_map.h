/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LS1C103_MAP_H_
#define __LS1C103_MAP_H_

/* Includes ------------------------------------------------------------------*/
//#include "ls1c103_type.h"
//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"

/* Exported types ------------------------------------------------------------*/
/******************************************************************************/

/* Uncomment the line below to compile the library in DEBUG mode, this will expanse
   the "assert_param" macro in the firmware library code (see "Exported macro"
   section below) */
/*#define DEBUG    1*/

/*                         Peripheral registers structures                    */
/******************************************************************************/

/*------------------------ Analog to Digital Converter -----------------------*/
typedef struct
{
  vuint32_t SR;
  vuint32_t CR1;
  vuint32_t CR2;
  vuint32_t SMPR1;
  vuint32_t SMPR2;
  vuint32_t JOFR1;
  vuint32_t JOFR2;
  vuint32_t JOFR3;
  vuint32_t JOFR4;
  vuint32_t HTR;
  vuint32_t LTR;
  vuint32_t SQR1;
  vuint32_t SQR2;
  vuint32_t SQR3;
  vuint32_t JSQR;
  vuint32_t JDR1;
  vuint32_t JDR2;
  vuint32_t JDR3;
  vuint32_t JDR4;
  vuint32_t DR;
} ADC_TypeDef;

/*------------------------ CRC calculation unit ------------------------------*/
typedef struct
{
  vuint32_t DR;
  vuint8_t  IDR;
  uint8_t   RESERVED0;
  uint16_t  RESERVED1;
  vuint32_t CR;
} CRC_TypeDef;

/*------------------------ DMA Controller ------------------------------------*/
typedef struct
{
  vuint32_t CCR;
  vuint32_t CNDTR;
  vuint32_t CPAR;
  vuint32_t CMAR;
} DMA_Channel_TypeDef;

typedef struct
{
  vuint32_t ISR;
  vuint32_t IFCR;
} DMA_TypeDef;

/*------------------------ General Purpose and Alternate Function IO ---------*/


/*------------------------ Inter-integrated Circuit Interface ----------------*/
typedef struct
{
  vuint16_t CR1;
  uint16_t  RESERVED0;
  vuint16_t CR2;
  uint16_t  RESERVED1;
  vuint16_t OAR1;
  uint16_t  RESERVED2;
  vuint16_t OAR2;
  uint16_t  RESERVED3;
  vuint16_t DR;
  uint16_t  RESERVED4;
  vuint16_t SR1;
  uint16_t  RESERVED5;
  vuint16_t SR2;
  uint16_t  RESERVED6;
  vuint16_t CCR;
  uint16_t  RESERVED7;
  vuint16_t TRISE;
  uint16_t  RESERVED8;
} I2C_TypeDef;

/*------------------------ TIM -----------------------------------------------*/
typedef struct
{
  vuint16_t CR1;
  uint16_t  RESERVED0;
  vuint16_t CR2;
  uint16_t  RESERVED1;
  vuint16_t SMCR;
  uint16_t  RESERVED2;
  vuint16_t DIER;
  uint16_t  RESERVED3;
  vuint16_t SR;
  uint16_t  RESERVED4;
  vuint16_t EGR;
  uint16_t  RESERVED5;
  vuint16_t CCMR1;
  uint16_t  RESERVED6;
  vuint16_t CCMR2;
  uint16_t  RESERVED7;
  vuint16_t CCER;
  uint16_t  RESERVED8;
  vuint16_t CNT;
  uint16_t  RESERVED9;
  vuint16_t PSC;
  uint16_t  RESERVED10;
  vuint16_t ARR;
  uint16_t  RESERVED11;
  vuint16_t RCR;
  uint16_t  RESERVED12;
  vuint16_t CCR1;
  uint16_t  RESERVED13;
  vuint16_t CCR2;
  uint16_t  RESERVED14;
  vuint16_t CCR3;
  uint16_t  RESERVED15;
  vuint16_t CCR4;
  uint16_t  RESERVED16;
  vuint16_t BDTR;
  uint16_t  RESERVED17;
  vuint16_t DCR;
  uint16_t  RESERVED18;
  vuint16_t DMAR;
  uint16_t  RESERVED19;
} TIM_TypeDef;





/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

#define TIM1_BASE             (ATIM_BASE)
#define TIM2_BASE             (GTIM_BASE)
#define TIM6_BASE             (BTIM_BASE)
#define I2C1_BASE             (I2C_BASE)

#define AFIO_BASE             (AFIO_REG_BASE)       
#define ADC1_BASE             (ADC_BASE)

#define DMA1_BASE             (DMA_BASE)                 //(DMA_BASE + 0x0000)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x0008)       //(DMA_BASE + 0x0008)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x001C)       //(DMA_BASE + 0x001C)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x0030)       //(DMA_BASE + 0x0030)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x0044)       //(DMA_BASE + 0x0044)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/

#define TIM1                ((TIM_TypeDef *) TIM1_BASE)

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)

#define TIM6                ((TIM_TypeDef *) TIM6_BASE)

#define I2C1                ((I2C_TypeDef *) I2C1_BASE)

#define AFIO                ((AFIO_TypeDef *) AFIO_BASE)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)

#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)

#define ADC1                ((ADC_TypeDef *) ADC1_BASE)


#define DMA1                ((DMA_TypeDef *) DMA1_BASE)

#define DMA1_Channel1       ((DMA_Channel_TypeDef *) DMA1_Channel1_BASE)

#define DMA1_Channel2       ((DMA_Channel_TypeDef *) DMA1_Channel2_BASE)

#define DMA1_Channel3       ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)

#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)

// #define CRC                 ((CRC_TypeDef *) CRC_BASE)


/* Exported constants --------------------------------------------------------*/
/******************************************************************************/
/*                                                                            */
/*                          CRC calculation unit                              */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((uint32_t)0xFFFFFFFF) /* Data register bits */


/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((uint8_t)0xFF)        /* General-purpose 8-bit data register bits */


/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((uint8_t)0x01)        /* RESET bit */


/******************************************************************************/
/*                                                                            */
/*                General Purpose and Alternate Function IO                   */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for GPIO_CRL register  *******************/
#define  GPIO_CRL_MODE                       ((uint32_t)0x33333333)        /* Port x mode bits */

#define  GPIO_CRL_MODE0                      ((uint32_t)0x00000003)        /* MODE0[1:0] bits (Port x mode bits, pin 0) */
#define  GPIO_CRL_MODE0_0                    ((uint32_t)0x00000001)        /* Bit 0 */
#define  GPIO_CRL_MODE0_1                    ((uint32_t)0x00000002)        /* Bit 1 */

#define  GPIO_CRL_MODE1                      ((uint32_t)0x00000030)        /* MODE1[1:0] bits (Port x mode bits, pin 1) */
#define  GPIO_CRL_MODE1_0                    ((uint32_t)0x00000010)        /* Bit 0 */
#define  GPIO_CRL_MODE1_1                    ((uint32_t)0x00000020)        /* Bit 1 */

#define  GPIO_CRL_MODE2                      ((uint32_t)0x00000300)        /* MODE2[1:0] bits (Port x mode bits, pin 2) */
#define  GPIO_CRL_MODE2_0                    ((uint32_t)0x00000100)        /* Bit 0 */
#define  GPIO_CRL_MODE2_1                    ((uint32_t)0x00000200)        /* Bit 1 */

#define  GPIO_CRL_MODE3                      ((uint32_t)0x00003000)        /* MODE3[1:0] bits (Port x mode bits, pin 3) */
#define  GPIO_CRL_MODE3_0                    ((uint32_t)0x00001000)        /* Bit 0 */
#define  GPIO_CRL_MODE3_1                    ((uint32_t)0x00002000)        /* Bit 1 */

#define  GPIO_CRL_MODE4                      ((uint32_t)0x00030000)        /* MODE4[1:0] bits (Port x mode bits, pin 4) */
#define  GPIO_CRL_MODE4_0                    ((uint32_t)0x00010000)        /* Bit 0 */
#define  GPIO_CRL_MODE4_1                    ((uint32_t)0x00020000)        /* Bit 1 */

#define  GPIO_CRL_MODE5                      ((uint32_t)0x00300000)        /* MODE5[1:0] bits (Port x mode bits, pin 5) */
#define  GPIO_CRL_MODE5_0                    ((uint32_t)0x00100000)        /* Bit 0 */
#define  GPIO_CRL_MODE5_1                    ((uint32_t)0x00200000)        /* Bit 1 */

#define  GPIO_CRL_MODE6                      ((uint32_t)0x03000000)        /* MODE6[1:0] bits (Port x mode bits, pin 6) */
#define  GPIO_CRL_MODE6_0                    ((uint32_t)0x01000000)        /* Bit 0 */
#define  GPIO_CRL_MODE6_1                    ((uint32_t)0x02000000)        /* Bit 1 */

#define  GPIO_CRL_MODE7                      ((uint32_t)0x30000000)        /* MODE7[1:0] bits (Port x mode bits, pin 7) */
#define  GPIO_CRL_MODE7_0                    ((uint32_t)0x10000000)        /* Bit 0 */
#define  GPIO_CRL_MODE7_1                    ((uint32_t)0x20000000)        /* Bit 1 */


#define  GPIO_CRL_CNF                        ((uint32_t)0xCCCCCCCC)        /* Port x configuration bits */

#define  GPIO_CRL_CNF0                       ((uint32_t)0x0000000C)        /* CNF0[1:0] bits (Port x configuration bits, pin 0) */
#define  GPIO_CRL_CNF0_0                     ((uint32_t)0x00000004)        /* Bit 0 */
#define  GPIO_CRL_CNF0_1                     ((uint32_t)0x00000008)        /* Bit 1 */

#define  GPIO_CRL_CNF1                       ((uint32_t)0x000000C0)        /* CNF1[1:0] bits (Port x configuration bits, pin 1) */
#define  GPIO_CRL_CNF1_0                     ((uint32_t)0x00000040)        /* Bit 0 */
#define  GPIO_CRL_CNF1_1                     ((uint32_t)0x00000080)        /* Bit 1 */

#define  GPIO_CRL_CNF2                       ((uint32_t)0x00000C00)        /* CNF2[1:0] bits (Port x configuration bits, pin 2) */
#define  GPIO_CRL_CNF2_0                     ((uint32_t)0x00000400)        /* Bit 0 */
#define  GPIO_CRL_CNF2_1                     ((uint32_t)0x00000800)        /* Bit 1 */

#define  GPIO_CRL_CNF3                       ((uint32_t)0x0000C000)        /* CNF3[1:0] bits (Port x configuration bits, pin 3) */
#define  GPIO_CRL_CNF3_0                     ((uint32_t)0x00004000)        /* Bit 0 */
#define  GPIO_CRL_CNF3_1                     ((uint32_t)0x00008000)        /* Bit 1 */

#define  GPIO_CRL_CNF4                       ((uint32_t)0x000C0000)        /* CNF4[1:0] bits (Port x configuration bits, pin 4) */
#define  GPIO_CRL_CNF4_0                     ((uint32_t)0x00040000)        /* Bit 0 */
#define  GPIO_CRL_CNF4_1                     ((uint32_t)0x00080000)        /* Bit 1 */

#define  GPIO_CRL_CNF5                       ((uint32_t)0x00C00000)        /* CNF5[1:0] bits (Port x configuration bits, pin 5) */
#define  GPIO_CRL_CNF5_0                     ((uint32_t)0x00400000)        /* Bit 0 */
#define  GPIO_CRL_CNF5_1                     ((uint32_t)0x00800000)        /* Bit 1 */

#define  GPIO_CRL_CNF6                       ((uint32_t)0x0C000000)        /* CNF6[1:0] bits (Port x configuration bits, pin 6) */
#define  GPIO_CRL_CNF6_0                     ((uint32_t)0x04000000)        /* Bit 0 */
#define  GPIO_CRL_CNF6_1                     ((uint32_t)0x08000000)        /* Bit 1 */

#define  GPIO_CRL_CNF7                       ((uint32_t)0xC0000000)        /* CNF7[1:0] bits (Port x configuration bits, pin 7) */
#define  GPIO_CRL_CNF7_0                     ((uint32_t)0x40000000)        /* Bit 0 */
#define  GPIO_CRL_CNF7_1                     ((uint32_t)0x80000000)        /* Bit 1 */


/*******************  Bit definition for GPIO_CRH register  *******************/
#define  GPIO_CRH_MODE                       ((uint32_t)0x33333333)        /* Port x mode bits */

#define  GPIO_CRH_MODE8                      ((uint32_t)0x00000003)        /* MODE8[1:0] bits (Port x mode bits, pin 8) */
#define  GPIO_CRH_MODE8_0                    ((uint32_t)0x00000001)        /* Bit 0 */
#define  GPIO_CRH_MODE8_1                    ((uint32_t)0x00000002)        /* Bit 1 */

#define  GPIO_CRH_MODE9                      ((uint32_t)0x00000030)        /* MODE9[1:0] bits (Port x mode bits, pin 9) */
#define  GPIO_CRH_MODE9_0                    ((uint32_t)0x00000010)        /* Bit 0 */
#define  GPIO_CRH_MODE9_1                    ((uint32_t)0x00000020)        /* Bit 1 */

#define  GPIO_CRH_MODE10                     ((uint32_t)0x00000300)        /* MODE10[1:0] bits (Port x mode bits, pin 10) */
#define  GPIO_CRH_MODE10_0                   ((uint32_t)0x00000100)        /* Bit 0 */
#define  GPIO_CRH_MODE10_1                   ((uint32_t)0x00000200)        /* Bit 1 */

#define  GPIO_CRH_MODE11                     ((uint32_t)0x00003000)        /* MODE11[1:0] bits (Port x mode bits, pin 11) */
#define  GPIO_CRH_MODE11_0                   ((uint32_t)0x00001000)        /* Bit 0 */
#define  GPIO_CRH_MODE11_1                   ((uint32_t)0x00002000)        /* Bit 1 */

#define  GPIO_CRH_MODE12                     ((uint32_t)0x00030000)        /* MODE12[1:0] bits (Port x mode bits, pin 12) */
#define  GPIO_CRH_MODE12_0                   ((uint32_t)0x00010000)        /* Bit 0 */
#define  GPIO_CRH_MODE12_1                   ((uint32_t)0x00020000)        /* Bit 1 */

#define  GPIO_CRH_MODE13                     ((uint32_t)0x00300000)        /* MODE13[1:0] bits (Port x mode bits, pin 13) */
#define  GPIO_CRH_MODE13_0                   ((uint32_t)0x00100000)        /* Bit 0 */
#define  GPIO_CRH_MODE13_1                   ((uint32_t)0x00200000)        /* Bit 1 */

#define  GPIO_CRH_MODE14                     ((uint32_t)0x03000000)        /* MODE14[1:0] bits (Port x mode bits, pin 14) */
#define  GPIO_CRH_MODE14_0                   ((uint32_t)0x01000000)        /* Bit 0 */
#define  GPIO_CRH_MODE14_1                   ((uint32_t)0x02000000)        /* Bit 1 */

#define  GPIO_CRH_MODE15                     ((uint32_t)0x30000000)        /* MODE15[1:0] bits (Port x mode bits, pin 15) */
#define  GPIO_CRH_MODE15_0                   ((uint32_t)0x10000000)        /* Bit 0 */
#define  GPIO_CRH_MODE15_1                   ((uint32_t)0x20000000)        /* Bit 1 */


#define  GPIO_CRH_CNF                        ((uint32_t)0xCCCCCCCC)        /* Port x configuration bits */

#define  GPIO_CRH_CNF8                       ((uint32_t)0x0000000C)        /* CNF8[1:0] bits (Port x configuration bits, pin 8) */
#define  GPIO_CRH_CNF8_0                     ((uint32_t)0x00000004)        /* Bit 0 */
#define  GPIO_CRH_CNF8_1                     ((uint32_t)0x00000008)        /* Bit 1 */

#define  GPIO_CRH_CNF9                       ((uint32_t)0x000000C0)        /* CNF9[1:0] bits (Port x configuration bits, pin 9) */
#define  GPIO_CRH_CNF9_0                     ((uint32_t)0x00000040)        /* Bit 0 */
#define  GPIO_CRH_CNF9_1                     ((uint32_t)0x00000080)        /* Bit 1 */

#define  GPIO_CRH_CNF10                      ((uint32_t)0x00000C00)        /* CNF10[1:0] bits (Port x configuration bits, pin 10) */
#define  GPIO_CRH_CNF10_0                    ((uint32_t)0x00000400)        /* Bit 0 */
#define  GPIO_CRH_CNF10_1                    ((uint32_t)0x00000800)        /* Bit 1 */

#define  GPIO_CRH_CNF11                      ((uint32_t)0x0000C000)        /* CNF11[1:0] bits (Port x configuration bits, pin 11) */
#define  GPIO_CRH_CNF11_0                    ((uint32_t)0x00004000)        /* Bit 0 */
#define  GPIO_CRH_CNF11_1                    ((uint32_t)0x00008000)        /* Bit 1 */

#define  GPIO_CRH_CNF12                      ((uint32_t)0x000C0000)        /* CNF12[1:0] bits (Port x configuration bits, pin 12) */
#define  GPIO_CRH_CNF12_0                    ((uint32_t)0x00040000)        /* Bit 0 */
#define  GPIO_CRH_CNF12_1                    ((uint32_t)0x00080000)        /* Bit 1 */

#define  GPIO_CRH_CNF13                      ((uint32_t)0x00C00000)        /* CNF13[1:0] bits (Port x configuration bits, pin 13) */
#define  GPIO_CRH_CNF13_0                    ((uint32_t)0x00400000)        /* Bit 0 */
#define  GPIO_CRH_CNF13_1                    ((uint32_t)0x00800000)        /* Bit 1 */

#define  GPIO_CRH_CNF14                      ((uint32_t)0x0C000000)        /* CNF14[1:0] bits (Port x configuration bits, pin 14) */
#define  GPIO_CRH_CNF14_0                    ((uint32_t)0x04000000)        /* Bit 0 */
#define  GPIO_CRH_CNF14_1                    ((uint32_t)0x08000000)        /* Bit 1 */

#define  GPIO_CRH_CNF15                      ((uint32_t)0xC0000000)        /* CNF15[1:0] bits (Port x configuration bits, pin 15) */
#define  GPIO_CRH_CNF15_0                    ((uint32_t)0x40000000)        /* Bit 0 */
#define  GPIO_CRH_CNF15_1                    ((uint32_t)0x80000000)        /* Bit 1 */


/*******************  Bit definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR0                        ((uint16_t)0x0001)            /* Port input data, bit 0 */
#define GPIO_IDR_IDR1                        ((uint16_t)0x0002)            /* Port input data, bit 1 */
#define GPIO_IDR_IDR2                        ((uint16_t)0x0004)            /* Port input data, bit 2 */
#define GPIO_IDR_IDR3                        ((uint16_t)0x0008)            /* Port input data, bit 3 */
#define GPIO_IDR_IDR4                        ((uint16_t)0x0010)            /* Port input data, bit 4 */
#define GPIO_IDR_IDR5                        ((uint16_t)0x0020)            /* Port input data, bit 5 */
#define GPIO_IDR_IDR6                        ((uint16_t)0x0040)            /* Port input data, bit 6 */
#define GPIO_IDR_IDR7                        ((uint16_t)0x0080)            /* Port input data, bit 7 */
#define GPIO_IDR_IDR8                        ((uint16_t)0x0100)            /* Port input data, bit 8 */
#define GPIO_IDR_IDR9                        ((uint16_t)0x0200)            /* Port input data, bit 9 */
#define GPIO_IDR_IDR10                       ((uint16_t)0x0400)            /* Port input data, bit 10 */
#define GPIO_IDR_IDR11                       ((uint16_t)0x0800)            /* Port input data, bit 11 */
#define GPIO_IDR_IDR12                       ((uint16_t)0x1000)            /* Port input data, bit 12 */
#define GPIO_IDR_IDR13                       ((uint16_t)0x2000)            /* Port input data, bit 13 */
#define GPIO_IDR_IDR14                       ((uint16_t)0x4000)            /* Port input data, bit 14 */
#define GPIO_IDR_IDR15                       ((uint16_t)0x8000)            /* Port input data, bit 15 */


/*******************  Bit definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR0                        ((uint16_t)0x0001)            /* Port output data, bit 0 */
#define GPIO_ODR_ODR1                        ((uint16_t)0x0002)            /* Port output data, bit 1 */
#define GPIO_ODR_ODR2                        ((uint16_t)0x0004)            /* Port output data, bit 2 */
#define GPIO_ODR_ODR3                        ((uint16_t)0x0008)            /* Port output data, bit 3 */
#define GPIO_ODR_ODR4                        ((uint16_t)0x0010)            /* Port output data, bit 4 */
#define GPIO_ODR_ODR5                        ((uint16_t)0x0020)            /* Port output data, bit 5 */
#define GPIO_ODR_ODR6                        ((uint16_t)0x0040)            /* Port output data, bit 6 */
#define GPIO_ODR_ODR7                        ((uint16_t)0x0080)            /* Port output data, bit 7 */
#define GPIO_ODR_ODR8                        ((uint16_t)0x0100)            /* Port output data, bit 8 */
#define GPIO_ODR_ODR9                        ((uint16_t)0x0200)            /* Port output data, bit 9 */
#define GPIO_ODR_ODR10                       ((uint16_t)0x0400)            /* Port output data, bit 10 */
#define GPIO_ODR_ODR11                       ((uint16_t)0x0800)            /* Port output data, bit 11 */
#define GPIO_ODR_ODR12                       ((uint16_t)0x1000)            /* Port output data, bit 12 */
#define GPIO_ODR_ODR13                       ((uint16_t)0x2000)            /* Port output data, bit 13 */
#define GPIO_ODR_ODR14                       ((uint16_t)0x4000)            /* Port output data, bit 14 */
#define GPIO_ODR_ODR15                       ((uint16_t)0x8000)            /* Port output data, bit 15 */


/******************  Bit definition for GPIO_BSRR register  *******************/
#define GPIO_BSRR_BS0                        ((uint32_t)0x00000001)        /* Port x Set bit 0 */
#define GPIO_BSRR_BS1                        ((uint32_t)0x00000002)        /* Port x Set bit 1 */
#define GPIO_BSRR_BS2                        ((uint32_t)0x00000004)        /* Port x Set bit 2 */
#define GPIO_BSRR_BS3                        ((uint32_t)0x00000008)        /* Port x Set bit 3 */
#define GPIO_BSRR_BS4                        ((uint32_t)0x00000010)        /* Port x Set bit 4 */
#define GPIO_BSRR_BS5                        ((uint32_t)0x00000020)        /* Port x Set bit 5 */
#define GPIO_BSRR_BS6                        ((uint32_t)0x00000040)        /* Port x Set bit 6 */
#define GPIO_BSRR_BS7                        ((uint32_t)0x00000080)        /* Port x Set bit 7 */
#define GPIO_BSRR_BS8                        ((uint32_t)0x00000100)        /* Port x Set bit 8 */
#define GPIO_BSRR_BS9                        ((uint32_t)0x00000200)        /* Port x Set bit 9 */
#define GPIO_BSRR_BS10                       ((uint32_t)0x00000400)        /* Port x Set bit 10 */
#define GPIO_BSRR_BS11                       ((uint32_t)0x00000800)        /* Port x Set bit 11 */
#define GPIO_BSRR_BS12                       ((uint32_t)0x00001000)        /* Port x Set bit 12 */
#define GPIO_BSRR_BS13                       ((uint32_t)0x00002000)        /* Port x Set bit 13 */
#define GPIO_BSRR_BS14                       ((uint32_t)0x00004000)        /* Port x Set bit 14 */
#define GPIO_BSRR_BS15                       ((uint32_t)0x00008000)        /* Port x Set bit 15 */

#define GPIO_BSRR_BR0                        ((uint32_t)0x00010000)        /* Port x Reset bit 0 */
#define GPIO_BSRR_BR1                        ((uint32_t)0x00020000)        /* Port x Reset bit 1 */
#define GPIO_BSRR_BR2                        ((uint32_t)0x00040000)        /* Port x Reset bit 2 */
#define GPIO_BSRR_BR3                        ((uint32_t)0x00080000)        /* Port x Reset bit 3 */
#define GPIO_BSRR_BR4                        ((uint32_t)0x00100000)        /* Port x Reset bit 4 */
#define GPIO_BSRR_BR5                        ((uint32_t)0x00200000)        /* Port x Reset bit 5 */
#define GPIO_BSRR_BR6                        ((uint32_t)0x00400000)        /* Port x Reset bit 6 */
#define GPIO_BSRR_BR7                        ((uint32_t)0x00800000)        /* Port x Reset bit 7 */
#define GPIO_BSRR_BR8                        ((uint32_t)0x01000000)        /* Port x Reset bit 8 */
#define GPIO_BSRR_BR9                        ((uint32_t)0x02000000)        /* Port x Reset bit 9 */
#define GPIO_BSRR_BR10                       ((uint32_t)0x04000000)        /* Port x Reset bit 10 */
#define GPIO_BSRR_BR11                       ((uint32_t)0x08000000)        /* Port x Reset bit 11 */
#define GPIO_BSRR_BR12                       ((uint32_t)0x10000000)        /* Port x Reset bit 12 */
#define GPIO_BSRR_BR13                       ((uint32_t)0x20000000)        /* Port x Reset bit 13 */
#define GPIO_BSRR_BR14                       ((uint32_t)0x40000000)        /* Port x Reset bit 14 */
#define GPIO_BSRR_BR15                       ((uint32_t)0x80000000)        /* Port x Reset bit 15 */


/*******************  Bit definition for GPIO_BRR register  *******************/
#define GPIO_BRR_BR0                         ((uint16_t)0x0001)            /* Port x Reset bit 0 */
#define GPIO_BRR_BR1                         ((uint16_t)0x0002)            /* Port x Reset bit 1 */
#define GPIO_BRR_BR2                         ((uint16_t)0x0004)            /* Port x Reset bit 2 */
#define GPIO_BRR_BR3                         ((uint16_t)0x0008)            /* Port x Reset bit 3 */
#define GPIO_BRR_BR4                         ((uint16_t)0x0010)            /* Port x Reset bit 4 */
#define GPIO_BRR_BR5                         ((uint16_t)0x0020)            /* Port x Reset bit 5 */
#define GPIO_BRR_BR6                         ((uint16_t)0x0040)            /* Port x Reset bit 6 */
#define GPIO_BRR_BR7                         ((uint16_t)0x0080)            /* Port x Reset bit 7 */
#define GPIO_BRR_BR8                         ((uint16_t)0x0100)            /* Port x Reset bit 8 */
#define GPIO_BRR_BR9                         ((uint16_t)0x0200)            /* Port x Reset bit 9 */
#define GPIO_BRR_BR10                        ((uint16_t)0x0400)            /* Port x Reset bit 10 */
#define GPIO_BRR_BR11                        ((uint16_t)0x0800)            /* Port x Reset bit 11 */
#define GPIO_BRR_BR12                        ((uint16_t)0x1000)            /* Port x Reset bit 12 */
#define GPIO_BRR_BR13                        ((uint16_t)0x2000)            /* Port x Reset bit 13 */
#define GPIO_BRR_BR14                        ((uint16_t)0x4000)            /* Port x Reset bit 14 */
#define GPIO_BRR_BR15                        ((uint16_t)0x8000)            /* Port x Reset bit 15 */


/******************  Bit definition for GPIO_LCKR register  *******************/
#define GPIO_LCKR_LCK0                       ((uint32_t)0x00000001)        /* Port x Lock bit 0 */
#define GPIO_LCKR_LCK1                       ((uint32_t)0x00000002)        /* Port x Lock bit 1 */
#define GPIO_LCKR_LCK2                       ((uint32_t)0x00000004)        /* Port x Lock bit 2 */
#define GPIO_LCKR_LCK3                       ((uint32_t)0x00000008)        /* Port x Lock bit 3 */
#define GPIO_LCKR_LCK4                       ((uint32_t)0x00000010)        /* Port x Lock bit 4 */
#define GPIO_LCKR_LCK5                       ((uint32_t)0x00000020)        /* Port x Lock bit 5 */
#define GPIO_LCKR_LCK6                       ((uint32_t)0x00000040)        /* Port x Lock bit 6 */
#define GPIO_LCKR_LCK7                       ((uint32_t)0x00000080)        /* Port x Lock bit 7 */
#define GPIO_LCKR_LCK8                       ((uint32_t)0x00000100)        /* Port x Lock bit 8 */
#define GPIO_LCKR_LCK9                       ((uint32_t)0x00000200)        /* Port x Lock bit 9 */
#define GPIO_LCKR_LCK10                      ((uint32_t)0x00000400)        /* Port x Lock bit 10 */
#define GPIO_LCKR_LCK11                      ((uint32_t)0x00000800)        /* Port x Lock bit 11 */
#define GPIO_LCKR_LCK12                      ((uint32_t)0x00001000)        /* Port x Lock bit 12 */
#define GPIO_LCKR_LCK13                      ((uint32_t)0x00002000)        /* Port x Lock bit 13 */
#define GPIO_LCKR_LCK14                      ((uint32_t)0x00004000)        /* Port x Lock bit 14 */
#define GPIO_LCKR_LCK15                      ((uint32_t)0x00008000)        /* Port x Lock bit 15 */
#define GPIO_LCKR_LCKK                       ((uint32_t)0x00010000)        /* Lock key */


/*----------------------------------------------------------------------------*/


/******************  Bit definition for AFIO_EVCR register  *******************/
#define AFIO_EVCR_PIN                        ((uint8_t)0x0F)               /* PIN[3:0] bits (Pin selection) */
#define AFIO_EVCR_PIN_0                      ((uint8_t)0x01)               /* Bit 0 */
#define AFIO_EVCR_PIN_1                      ((uint8_t)0x02)               /* Bit 1 */
#define AFIO_EVCR_PIN_2                      ((uint8_t)0x04)               /* Bit 2 */
#define AFIO_EVCR_PIN_3                      ((uint8_t)0x08)               /* Bit 3 */

/* PIN configuration */
#define AFIO_EVCR_PIN_PX0                    ((uint8_t)0x00)               /* Pin 0 selected */
#define AFIO_EVCR_PIN_PX1                    ((uint8_t)0x01)               /* Pin 1 selected */
#define AFIO_EVCR_PIN_PX2                    ((uint8_t)0x02)               /* Pin 2 selected */
#define AFIO_EVCR_PIN_PX3                    ((uint8_t)0x03)               /* Pin 3 selected */
#define AFIO_EVCR_PIN_PX4                    ((uint8_t)0x04)               /* Pin 4 selected */
#define AFIO_EVCR_PIN_PX5                    ((uint8_t)0x05)               /* Pin 5 selected */
#define AFIO_EVCR_PIN_PX6                    ((uint8_t)0x06)               /* Pin 6 selected */
#define AFIO_EVCR_PIN_PX7                    ((uint8_t)0x07)               /* Pin 7 selected */
#define AFIO_EVCR_PIN_PX8                    ((uint8_t)0x08)               /* Pin 8 selected */
#define AFIO_EVCR_PIN_PX9                    ((uint8_t)0x09)               /* Pin 9 selected */
#define AFIO_EVCR_PIN_PX10                   ((uint8_t)0x0A)               /* Pin 10 selected */
#define AFIO_EVCR_PIN_PX11                   ((uint8_t)0x0B)               /* Pin 11 selected */
#define AFIO_EVCR_PIN_PX12                   ((uint8_t)0x0C)               /* Pin 12 selected */
#define AFIO_EVCR_PIN_PX13                   ((uint8_t)0x0D)               /* Pin 13 selected */
#define AFIO_EVCR_PIN_PX14                   ((uint8_t)0x0E)               /* Pin 14 selected */
#define AFIO_EVCR_PIN_PX15                   ((uint8_t)0x0F)               /* Pin 15 selected */

#define AFIO_EVCR_PORT                       ((uint8_t)0x70)               /* PORT[2:0] bits (Port selection) */
#define AFIO_EVCR_PORT_0                     ((uint8_t)0x10)               /* Bit 0 */
#define AFIO_EVCR_PORT_1                     ((uint8_t)0x20)               /* Bit 1 */
#define AFIO_EVCR_PORT_2                     ((uint8_t)0x40)               /* Bit 2 */

/* PORT configuration */
#define AFIO_EVCR_PORT_PA                    ((uint8_t)0x00)               /* Port A selected */
#define AFIO_EVCR_PORT_PB                    ((uint8_t)0x10)               /* Port B selected */
#define AFIO_EVCR_PORT_PC                    ((uint8_t)0x20)               /* Port C selected */
#define AFIO_EVCR_PORT_PD                    ((uint8_t)0x30)               /* Port D selected */
#define AFIO_EVCR_PORT_PE                    ((uint8_t)0x40)               /* Port E selected */

#define AFIO_EVCR_EVOE                       ((uint8_t)0x80)               /* Event Output Enable */


/******************  Bit definition for AFIO_MAPR register  *******************/
#define AFIO_MAPR_SPI1 _REMAP                ((uint32_t)0x00000001)        /* SPI1 remapping */
#define AFIO_MAPR_I2C1_REMAP                 ((uint32_t)0x00000002)        /* I2C1 remapping */
#define AFIO_MAPR_USART1_REMAP               ((uint32_t)0x00000004)        /* USART1 remapping */
#define AFIO_MAPR_USART2_REMAP               ((uint32_t)0x00000008)        /* USART2 remapping */

#define AFIO_MAPR_USART3_REMAP               ((uint32_t)0x00000030)        /* USART3_REMAP[1:0] bits (USART3 remapping) */
#define AFIO_MAPR_USART3_REMAP_0             ((uint32_t)0x00000010)        /* Bit 0 */
#define AFIO_MAPR_USART3_REMAP_1             ((uint32_t)0x00000020)        /* Bit 1 */

/* USART3_REMAP configuration */
#define AFIO_MAPR_USART3_REMAP_NOREMAP       ((uint32_t)0x00000000)        /* No remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14) */
#define AFIO_MAPR_USART3_REMAP_PARTIALREMAP  ((uint32_t)0x00000010)        /* Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14) */
#define AFIO_MAPR_USART3_REMAP_FULLREMAP     ((uint32_t)0x00000030)        /* Full remap (TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12) */

#define AFIO_MAPR_TIM1_REMAP                 ((uint32_t)0x000000C0)        /* TIM1_REMAP[1:0] bits (TIM1 remapping) */
#define AFIO_MAPR_TIM1_REMAP_0               ((uint32_t)0x00000040)        /* Bit 0 */
#define AFIO_MAPR_TIM1_REMAP_1               ((uint32_t)0x00000080)        /* Bit 1 */

/* TIM1_REMAP configuration */
#define AFIO_MAPR_TIM1_REMAP_NOREMAP         ((uint32_t)0x00000000)        /* No remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15) */
#define AFIO_MAPR_TIM1_REMAP_PARTIALREMAP    ((uint32_t)0x00000040)        /* Partial remap (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1) */
#define AFIO_MAPR_TIM1_REMAP_FULLREMAP       ((uint32_t)0x000000C0)        /* Full remap (ETR/PE7, CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12) */

#define AFIO_MAPR_TIM2_REMAP                 ((uint32_t)0x00000300)        /* TIM2_REMAP[1:0] bits (TIM2 remapping) */
#define AFIO_MAPR_TIM2_REMAP_0               ((uint32_t)0x00000100)        /* Bit 0 */
#define AFIO_MAPR_TIM2_REMAP_1               ((uint32_t)0x00000200)        /* Bit 1 */

/* TIM2_REMAP configuration */
#define AFIO_MAPR_TIM2_REMAP_NOREMAP         ((uint32_t)0x00000000)        /* No remap (CH1/ETR/PA0, CH2/PA1, CH3/PA2, CH4/PA3) */
#define AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1   ((uint32_t)0x00000100)        /* Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3) */
#define AFIO_MAPR_TIM2_REMAP_PARTIALREMAP2   ((uint32_t)0x00000200)        /* Partial remap (CH1/ETR/PA0, CH2/PA1, CH3/PB10, CH4/PB11) */
#define AFIO_MAPR_TIM2_REMAP_FULLREMAP       ((uint32_t)0x00000300)        /* Full remap (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11) */

#define AFIO_MAPR_TIM3_REMAP                 ((uint32_t)0x00000C00)        /* TIM3_REMAP[1:0] bits (TIM3 remapping) */
#define AFIO_MAPR_TIM3_REMAP_0               ((uint32_t)0x00000400)        /* Bit 0 */
#define AFIO_MAPR_TIM3_REMAP_1               ((uint32_t)0x00000800)        /* Bit 1 */

/* TIM3_REMAP configuration */
#define AFIO_MAPR_TIM3_REMAP_NOREMAP         ((uint32_t)0x00000000)        /* No remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1) */
#define AFIO_MAPR_TIM3_REMAP_PARTIALREMAP    ((uint32_t)0x00000800)        /* Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1) */
#define AFIO_MAPR_TIM3_REMAP_FULLREMAP       ((uint32_t)0x00000C00)        /* Full remap (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9) */

#define AFIO_MAPR_TIM4_REMAP                 ((uint32_t)0x00001000)        /* Port D0/Port D1 mapping on OSC_IN/OSC_OUT */

#define AFIO_MAPR_CAN_REMAP                  ((uint32_t)0x00006000)        /* CAN_REMAP[1:0] bits (CAN Alternate function remapping) */
#define AFIO_MAPR_CAN_REMAP_0                ((uint32_t)0x00002000)        /* Bit 0 */
#define AFIO_MAPR_CAN_REMAP_1                ((uint32_t)0x00004000)        /* Bit 1 */

/* CAN_REMAP configuration */
#define AFIO_MAPR_CAN_REMAP_REMAP1           ((uint32_t)0x00000000)        /* CANRX mapped to PA11, CANTX mapped to PA12 */
#define AFIO_MAPR_CAN_REMAP_REMAP2           ((uint32_t)0x00004000)        /* CANRX mapped to PB8, CANTX mapped to PB9 */
#define AFIO_MAPR_CAN_REMAP_REMAP3           ((uint32_t)0x00006000)        /* CANRX mapped to PD0, CANTX mapped to PD1 */

#define AFIO_MAPR_PD01_REMAP                 ((uint32_t)0x00008000)        /* Port D0/Port D1 mapping on OSC_IN/OSC_OUT */
#define AFIO_MAPR_TIM5CH4_IREMAP             ((uint32_t)0x00010000)        /* TIM5 Channel4 Internal Remap */
#define AFIO_MAPR_ADC1_ETRGINJ_REMAP         ((uint32_t)0x00020000)        /* ADC 1 External Trigger Injected Conversion remapping */
#define AFIO_MAPR_ADC1_ETRGREG_REMAP         ((uint32_t)0x00040000)        /* ADC 1 External Trigger Regular Conversion remapping */
#define AFIO_MAPR_ADC2_ETRGINJ_REMAP         ((uint32_t)0x00080000)        /* ADC 2 External Trigger Injected Conversion remapping */
#define AFIO_MAPR_ADC2_ETRGREG_REMAP         ((uint32_t)0x00100000)        /* ADC 2 External Trigger Regular Conversion remapping */

#define AFIO_MAPR_SWJ_CFG                    ((uint32_t)0x07000000)        /* SWJ_CFG[2:0] bits (Serial Wire JTAG configuration) */
#define AFIO_MAPR_SWJ_CFG_0                  ((uint32_t)0x01000000)        /* Bit 0 */
#define AFIO_MAPR_SWJ_CFG_1                  ((uint32_t)0x02000000)        /* Bit 1 */
#define AFIO_MAPR_SWJ_CFG_2                  ((uint32_t)0x04000000)        /* Bit 2 */

/* SWJ_CFG configuration */
#define AFIO_MAPR_SWJ_CFG_RESET              ((uint32_t)0x00000000)        /* Full SWJ (JTAG-DP + SW-DP) : Reset State */
#define AFIO_MAPR_SWJ_CFG_NOJNTRST           ((uint32_t)0x01000000)        /* Full SWJ (JTAG-DP + SW-DP) but without JNTRST */
#define AFIO_MAPR_SWJ_CFG_JTAGDISABLE        ((uint32_t)0x02000000)        /* JTAG-DP Disabled and SW-DP Enabled */
#define AFIO_MAPR_SWJ_CFG_DISABLE            ((uint32_t)0x04000000)        /* JTAG-DP Disabled and SW-DP Disabled */


/*****************  Bit definition for AFIO_EXTICR1 register  *****************/
#define AFIO_EXTICR1_EXTI0                   ((uint16_t)0x000F)            /* EXTI 0 configuration */
#define AFIO_EXTICR1_EXTI1                   ((uint16_t)0x00F0)            /* EXTI 1 configuration */
#define AFIO_EXTICR1_EXTI2                   ((uint16_t)0x0F00)            /* EXTI 2 configuration */
#define AFIO_EXTICR1_EXTI3                   ((uint16_t)0xF000)            /* EXTI 3 configuration */

/* EXTI0 configuration */
#define AFIO_EXTICR1_EXTI0_PA                ((uint16_t)0x0000)            /* PA[0] pin */
#define AFIO_EXTICR1_EXTI0_PB                ((uint16_t)0x0001)            /* PB[0] pin */
#define AFIO_EXTICR1_EXTI0_PC                ((uint16_t)0x0002)            /* PC[0] pin */
#define AFIO_EXTICR1_EXTI0_PD                ((uint16_t)0x0003)            /* PD[0] pin */
#define AFIO_EXTICR1_EXTI0_PE                ((uint16_t)0x0004)            /* PE[0] pin */
#define AFIO_EXTICR1_EXTI0_PF                ((uint16_t)0x0005)            /* PF[0] pin */
#define AFIO_EXTICR1_EXTI0_PG                ((uint16_t)0x0006)            /* PG[0] pin */

/* EXTI1 configuration */
#define AFIO_EXTICR1_EXTI1_PA                ((uint16_t)0x0000)            /* PA[1] pin */
#define AFIO_EXTICR1_EXTI1_PB                ((uint16_t)0x0010)            /* PB[1] pin */
#define AFIO_EXTICR1_EXTI1_PC                ((uint16_t)0x0020)            /* PC[1] pin */
#define AFIO_EXTICR1_EXTI1_PD                ((uint16_t)0x0030)            /* PD[1] pin */
#define AFIO_EXTICR1_EXTI1_PE                ((uint16_t)0x0040)            /* PE[1] pin */
#define AFIO_EXTICR1_EXTI1_PF                ((uint16_t)0x0050)            /* PF[1] pin */
#define AFIO_EXTICR1_EXTI1_PG                ((uint16_t)0x0060)            /* PG[1] pin */

/* EXTI2 configuration */  
#define AFIO_EXTICR1_EXTI2_PA                ((uint16_t)0x0000)            /* PA[2] pin */
#define AFIO_EXTICR1_EXTI2_PB                ((uint16_t)0x0100)            /* PB[2] pin */
#define AFIO_EXTICR1_EXTI2_PC                ((uint16_t)0x0200)            /* PC[2] pin */
#define AFIO_EXTICR1_EXTI2_PD                ((uint16_t)0x0300)            /* PD[2] pin */
#define AFIO_EXTICR1_EXTI2_PE                ((uint16_t)0x0400)            /* PE[2] pin */
#define AFIO_EXTICR1_EXTI2_PF                ((uint16_t)0x0500)            /* PF[2] pin */
#define AFIO_EXTICR1_EXTI2_PG                ((uint16_t)0x0600)            /* PG[2] pin */

/* EXTI3 configuration */
#define AFIO_EXTICR1_EXTI3_PA                ((uint16_t)0x0000)            /* PA[3] pin */
#define AFIO_EXTICR1_EXTI3_PB                ((uint16_t)0x1000)            /* PB[3] pin */
#define AFIO_EXTICR1_EXTI3_PC                ((uint16_t)0x2000)            /* PC[3] pin */
#define AFIO_EXTICR1_EXTI3_PD                ((uint16_t)0x3000)            /* PD[3] pin */
#define AFIO_EXTICR1_EXTI3_PE                ((uint16_t)0x4000)            /* PE[3] pin */
#define AFIO_EXTICR1_EXTI3_PF                ((uint16_t)0x5000)            /* PF[3] pin */
#define AFIO_EXTICR1_EXTI3_PG                ((uint16_t)0x6000)            /* PG[3] pin */


/*****************  Bit definition for AFIO_EXTICR2 register  *****************/
#define AFIO_EXTICR2_EXTI4                   ((uint16_t)0x000F)            /* EXTI 4 configuration */
#define AFIO_EXTICR2_EXTI5                   ((uint16_t)0x00F0)            /* EXTI 5 configuration */
#define AFIO_EXTICR2_EXTI6                   ((uint16_t)0x0F00)            /* EXTI 6 configuration */
#define AFIO_EXTICR2_EXTI7                   ((uint16_t)0xF000)            /* EXTI 7 configuration */

/* EXTI4 configuration */
#define AFIO_EXTICR2_EXTI4_PA                ((uint16_t)0x0000)            /* PA[4] pin */
#define AFIO_EXTICR2_EXTI4_PB                ((uint16_t)0x0001)            /* PB[4] pin */
#define AFIO_EXTICR2_EXTI4_PC                ((uint16_t)0x0002)            /* PC[4] pin */
#define AFIO_EXTICR2_EXTI4_PD                ((uint16_t)0x0003)            /* PD[4] pin */
#define AFIO_EXTICR2_EXTI4_PE                ((uint16_t)0x0004)            /* PE[4] pin */
#define AFIO_EXTICR2_EXTI4_PF                ((uint16_t)0x0005)            /* PF[4] pin */
#define AFIO_EXTICR2_EXTI4_PG                ((uint16_t)0x0006)            /* PG[4] pin */

/* EXTI5 configuration */
#define AFIO_EXTICR2_EXTI5_PA                ((uint16_t)0x0000)            /* PA[5] pin */
#define AFIO_EXTICR2_EXTI5_PB                ((uint16_t)0x0010)            /* PB[5] pin */
#define AFIO_EXTICR2_EXTI5_PC                ((uint16_t)0x0020)            /* PC[5] pin */
#define AFIO_EXTICR2_EXTI5_PD                ((uint16_t)0x0030)            /* PD[5] pin */
#define AFIO_EXTICR2_EXTI5_PE                ((uint16_t)0x0040)            /* PE[5] pin */
#define AFIO_EXTICR2_EXTI5_PF                ((uint16_t)0x0050)            /* PF[5] pin */
#define AFIO_EXTICR2_EXTI5_PG                ((uint16_t)0x0060)            /* PG[5] pin */

/* EXTI6 configuration */  
#define AFIO_EXTICR2_EXTI6_PA                ((uint16_t)0x0000)            /* PA[6] pin */
#define AFIO_EXTICR2_EXTI6_PB                ((uint16_t)0x0100)            /* PB[6] pin */
#define AFIO_EXTICR2_EXTI6_PC                ((uint16_t)0x0200)            /* PC[6] pin */
#define AFIO_EXTICR2_EXTI6_PD                ((uint16_t)0x0300)            /* PD[6] pin */
#define AFIO_EXTICR2_EXTI6_PE                ((uint16_t)0x0400)            /* PE[6] pin */
#define AFIO_EXTICR2_EXTI6_PF                ((uint16_t)0x0500)            /* PF[6] pin */
#define AFIO_EXTICR2_EXTI6_PG                ((uint16_t)0x0600)            /* PG[6] pin */

/* EXTI7 configuration */
#define AFIO_EXTICR2_EXTI7_PA                ((uint16_t)0x0000)            /* PA[7] pin */
#define AFIO_EXTICR2_EXTI7_PB                ((uint16_t)0x1000)            /* PB[7] pin */
#define AFIO_EXTICR2_EXTI7_PC                ((uint16_t)0x2000)            /* PC[7] pin */
#define AFIO_EXTICR2_EXTI7_PD                ((uint16_t)0x3000)            /* PD[7] pin */
#define AFIO_EXTICR2_EXTI7_PE                ((uint16_t)0x4000)            /* PE[7] pin */
#define AFIO_EXTICR2_EXTI7_PF                ((uint16_t)0x5000)            /* PF[7] pin */
#define AFIO_EXTICR2_EXTI7_PG                ((uint16_t)0x6000)            /* PG[7] pin */


/*****************  Bit definition for AFIO_EXTICR3 register  *****************/
#define AFIO_EXTICR3_EXTI8                   ((uint16_t)0x000F)            /* EXTI 8 configuration */
#define AFIO_EXTICR3_EXTI9                   ((uint16_t)0x00F0)            /* EXTI 9 configuration */
#define AFIO_EXTICR3_EXTI10                  ((uint16_t)0x0F00)            /* EXTI 10 configuration */
#define AFIO_EXTICR3_EXTI11                  ((uint16_t)0xF000)            /* EXTI 11 configuration */

/* EXTI8 configuration */
#define AFIO_EXTICR3_EXTI8_PA                ((uint16_t)0x0000)            /* PA[8] pin */
#define AFIO_EXTICR3_EXTI8_PB                ((uint16_t)0x0001)            /* PB[8] pin */
#define AFIO_EXTICR3_EXTI8_PC                ((uint16_t)0x0002)            /* PC[8] pin */
#define AFIO_EXTICR3_EXTI8_PD                ((uint16_t)0x0003)            /* PD[8] pin */
#define AFIO_EXTICR3_EXTI8_PE                ((uint16_t)0x0004)            /* PE[8] pin */
#define AFIO_EXTICR3_EXTI8_PF                ((uint16_t)0x0005)            /* PF[8] pin */
#define AFIO_EXTICR3_EXTI8_PG                ((uint16_t)0x0006)            /* PG[8] pin */

/* EXTI9 configuration */
#define AFIO_EXTICR3_EXTI9_PA                ((uint16_t)0x0000)            /* PA[9] pin */
#define AFIO_EXTICR3_EXTI9_PB                ((uint16_t)0x0010)            /* PB[9] pin */
#define AFIO_EXTICR3_EXTI9_PC                ((uint16_t)0x0020)            /* PC[9] pin */
#define AFIO_EXTICR3_EXTI9_PD                ((uint16_t)0x0030)            /* PD[9] pin */
#define AFIO_EXTICR3_EXTI9_PE                ((uint16_t)0x0040)            /* PE[9] pin */
#define AFIO_EXTICR3_EXTI9_PF                ((uint16_t)0x0050)            /* PF[9] pin */
#define AFIO_EXTICR3_EXTI9_PG                ((uint16_t)0x0060)            /* PG[9] pin */

/* EXTI10 configuration */  
#define AFIO_EXTICR3_EXTI10_PA               ((uint16_t)0x0000)            /* PA[10] pin */
#define AFIO_EXTICR3_EXTI10_PB               ((uint16_t)0x0100)            /* PB[10] pin */
#define AFIO_EXTICR3_EXTI10_PC               ((uint16_t)0x0200)            /* PC[10] pin */
#define AFIO_EXTICR3_EXTI10_PD               ((uint16_t)0x0300)            /* PD[10] pin */
#define AFIO_EXTICR3_EXTI10_PE               ((uint16_t)0x0400)            /* PE[10] pin */
#define AFIO_EXTICR3_EXTI10_PF               ((uint16_t)0x0500)            /* PF[10] pin */
#define AFIO_EXTICR3_EXTI10_PG               ((uint16_t)0x0600)            /* PG[10] pin */

/* EXTI11 configuration */
#define AFIO_EXTICR3_EXTI11_PA               ((uint16_t)0x0000)            /* PA[11] pin */
#define AFIO_EXTICR3_EXTI11_PB               ((uint16_t)0x1000)            /* PB[11] pin */
#define AFIO_EXTICR3_EXTI11_PC               ((uint16_t)0x2000)            /* PC[11] pin */
#define AFIO_EXTICR3_EXTI11_PD               ((uint16_t)0x3000)            /* PD[11] pin */
#define AFIO_EXTICR3_EXTI11_PE               ((uint16_t)0x4000)            /* PE[11] pin */
#define AFIO_EXTICR3_EXTI11_PF               ((uint16_t)0x5000)            /* PF[11] pin */
#define AFIO_EXTICR3_EXTI11_PG               ((uint16_t)0x6000)            /* PG[11] pin */


/*****************  Bit definition for AFIO_EXTICR4 register  *****************/
#define AFIO_EXTICR4_EXTI12                  ((uint16_t)0x000F)            /* EXTI 12 configuration */
#define AFIO_EXTICR4_EXTI13                  ((uint16_t)0x00F0)            /* EXTI 13 configuration */
#define AFIO_EXTICR4_EXTI14                  ((uint16_t)0x0F00)            /* EXTI 14 configuration */
#define AFIO_EXTICR4_EXTI15                  ((uint16_t)0xF000)            /* EXTI 15 configuration */

/* EXTI12 configuration */
#define AFIO_EXTICR4_EXTI12_PA               ((uint16_t)0x0000)            /* PA[12] pin */
#define AFIO_EXTICR4_EXTI12_PB               ((uint16_t)0x0001)            /* PB[12] pin */
#define AFIO_EXTICR4_EXTI12_PC               ((uint16_t)0x0002)            /* PC[12] pin */
#define AFIO_EXTICR4_EXTI12_PD               ((uint16_t)0x0003)            /* PD[12] pin */
#define AFIO_EXTICR4_EXTI12_PE               ((uint16_t)0x0004)            /* PE[12] pin */
#define AFIO_EXTICR4_EXTI12_PF               ((uint16_t)0x0005)            /* PF[12] pin */
#define AFIO_EXTICR4_EXTI12_PG               ((uint16_t)0x0006)            /* PG[12] pin */

/* EXTI13 configuration */
#define AFIO_EXTICR4_EXTI13_PA               ((uint16_t)0x0000)            /* PA[13] pin */
#define AFIO_EXTICR4_EXTI13_PB               ((uint16_t)0x0010)            /* PB[13] pin */
#define AFIO_EXTICR4_EXTI13_PC               ((uint16_t)0x0020)            /* PC[13] pin */
#define AFIO_EXTICR4_EXTI13_PD               ((uint16_t)0x0030)            /* PD[13] pin */
#define AFIO_EXTICR4_EXTI13_PE               ((uint16_t)0x0040)            /* PE[13] pin */
#define AFIO_EXTICR4_EXTI13_PF               ((uint16_t)0x0050)            /* PF[13] pin */
#define AFIO_EXTICR4_EXTI13_PG               ((uint16_t)0x0060)            /* PG[13] pin */

/* EXTI14 configuration */  
#define AFIO_EXTICR4_EXTI14_PA               ((uint16_t)0x0000)            /* PA[14] pin */
#define AFIO_EXTICR4_EXTI14_PB               ((uint16_t)0x0100)            /* PB[14] pin */
#define AFIO_EXTICR4_EXTI14_PC               ((uint16_t)0x0200)            /* PC[14] pin */
#define AFIO_EXTICR4_EXTI14_PD               ((uint16_t)0x0300)            /* PD[14] pin */
#define AFIO_EXTICR4_EXTI14_PE               ((uint16_t)0x0400)            /* PE[14] pin */
#define AFIO_EXTICR4_EXTI14_PF               ((uint16_t)0x0500)            /* PF[14] pin */
#define AFIO_EXTICR4_EXTI14_PG               ((uint16_t)0x0600)            /* PG[14] pin */

/* EXTI15 configuration */
#define AFIO_EXTICR4_EXTI15_PA               ((uint16_t)0x0000)            /* PA[15] pin */
#define AFIO_EXTICR4_EXTI15_PB               ((uint16_t)0x1000)            /* PB[15] pin */
#define AFIO_EXTICR4_EXTI15_PC               ((uint16_t)0x2000)            /* PC[15] pin */
#define AFIO_EXTICR4_EXTI15_PD               ((uint16_t)0x3000)            /* PD[15] pin */
#define AFIO_EXTICR4_EXTI15_PE               ((uint16_t)0x4000)            /* PE[15] pin */
#define AFIO_EXTICR4_EXTI15_PF               ((uint16_t)0x5000)            /* PF[15] pin */
#define AFIO_EXTICR4_EXTI15_PG               ((uint16_t)0x6000)            /* PG[15] pin */


/******************************************************************************/
/*                                                                            */
/*                             DMA Controller                                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMA_ISR register  ********************/
#define  DMA_ISR_GIF1                        ((uint32_t)0x00000001)        /* Channel 1 Global interrupt flag */
#define  DMA_ISR_TCIF1                       ((uint32_t)0x00000002)        /* Channel 1 Transfer Complete flag */
#define  DMA_ISR_HTIF1                       ((uint32_t)0x00000004)        /* Channel 1 Half Transfer flag */
#define  DMA_ISR_TEIF1                       ((uint32_t)0x00000008)        /* Channel 1 Transfer Error flag */
#define  DMA_ISR_GIF2                        ((uint32_t)0x00000010)        /* Channel 2 Global interrupt flag */
#define  DMA_ISR_TCIF2                       ((uint32_t)0x00000020)        /* Channel 2 Transfer Complete flag */
#define  DMA_ISR_HTIF2                       ((uint32_t)0x00000040)        /* Channel 2 Half Transfer flag */
#define  DMA_ISR_TEIF2                       ((uint32_t)0x00000080)        /* Channel 2 Transfer Error flag */
#define  DMA_ISR_GIF3                        ((uint32_t)0x00000100)        /* Channel 3 Global interrupt flag */
#define  DMA_ISR_TCIF3                       ((uint32_t)0x00000200)        /* Channel 3 Transfer Complete flag */
#define  DMA_ISR_HTIF3                       ((uint32_t)0x00000400)        /* Channel 3 Half Transfer flag */
#define  DMA_ISR_TEIF3                       ((uint32_t)0x00000800)        /* Channel 3 Transfer Error flag */
#define  DMA_ISR_GIF4                        ((uint32_t)0x00001000)        /* Channel 4 Global interrupt flag */
#define  DMA_ISR_TCIF4                       ((uint32_t)0x00002000)        /* Channel 4 Transfer Complete flag */
#define  DMA_ISR_HTIF4                       ((uint32_t)0x00004000)        /* Channel 4 Half Transfer flag */
#define  DMA_ISR_TEIF4                       ((uint32_t)0x00008000)        /* Channel 4 Transfer Error flag */
#define  DMA_ISR_GIF5                        ((uint32_t)0x00010000)        /* Channel 5 Global interrupt flag */
#define  DMA_ISR_TCIF5                       ((uint32_t)0x00020000)        /* Channel 5 Transfer Complete flag */
#define  DMA_ISR_HTIF5                       ((uint32_t)0x00040000)        /* Channel 5 Half Transfer flag */
#define  DMA_ISR_TEIF5                       ((uint32_t)0x00080000)        /* Channel 5 Transfer Error flag */
#define  DMA_ISR_GIF6                        ((uint32_t)0x00100000)        /* Channel 6 Global interrupt flag */
#define  DMA_ISR_TCIF6                       ((uint32_t)0x00200000)        /* Channel 6 Transfer Complete flag */
#define  DMA_ISR_HTIF6                       ((uint32_t)0x00400000)        /* Channel 6 Half Transfer flag */
#define  DMA_ISR_TEIF6                       ((uint32_t)0x00800000)        /* Channel 6 Transfer Error flag */
#define  DMA_ISR_GIF7                        ((uint32_t)0x01000000)        /* Channel 7 Global interrupt flag */
#define  DMA_ISR_TCIF7                       ((uint32_t)0x02000000)        /* Channel 7 Transfer Complete flag */
#define  DMA_ISR_HTIF7                       ((uint32_t)0x04000000)        /* Channel 7 Half Transfer flag */
#define  DMA_ISR_TEIF7                       ((uint32_t)0x08000000)        /* Channel 7 Transfer Error flag */


/*******************  Bit definition for DMA_IFCR register  *******************/
#define  DMA_IFCR_CGIF1                      ((uint32_t)0x00000001)        /* Channel 1 Global interrupt clearr */
#define  DMA_IFCR_CTCIF1                     ((uint32_t)0x00000002)        /* Channel 1 Transfer Complete clear */
#define  DMA_IFCR_CHTIF1                     ((uint32_t)0x00000004)        /* Channel 1 Half Transfer clear */
#define  DMA_IFCR_CTEIF1                     ((uint32_t)0x00000008)        /* Channel 1 Transfer Error clear */
#define  DMA_IFCR_CGIF2                      ((uint32_t)0x00000010)        /* Channel 2 Global interrupt clear */
#define  DMA_IFCR_CTCIF2                     ((uint32_t)0x00000020)        /* Channel 2 Transfer Complete clear */
#define  DMA_IFCR_CHTIF2                     ((uint32_t)0x00000040)        /* Channel 2 Half Transfer clear */
#define  DMA_IFCR_CTEIF2                     ((uint32_t)0x00000080)        /* Channel 2 Transfer Error clear */
#define  DMA_IFCR_CGIF3                      ((uint32_t)0x00000100)        /* Channel 3 Global interrupt clear */
#define  DMA_IFCR_CTCIF3                     ((uint32_t)0x00000200)        /* Channel 3 Transfer Complete clear */
#define  DMA_IFCR_CHTIF3                     ((uint32_t)0x00000400)        /* Channel 3 Half Transfer clear */
#define  DMA_IFCR_CTEIF3                     ((uint32_t)0x00000800)        /* Channel 3 Transfer Error clear */
#define  DMA_IFCR_CGIF4                      ((uint32_t)0x00001000)        /* Channel 4 Global interrupt clear */
#define  DMA_IFCR_CTCIF4                     ((uint32_t)0x00002000)        /* Channel 4 Transfer Complete clear */
#define  DMA_IFCR_CHTIF4                     ((uint32_t)0x00004000)        /* Channel 4 Half Transfer clear */
#define  DMA_IFCR_CTEIF4                     ((uint32_t)0x00008000)        /* Channel 4 Transfer Error clear */
#define  DMA_IFCR_CGIF5                      ((uint32_t)0x00010000)        /* Channel 5 Global interrupt clear */
#define  DMA_IFCR_CTCIF5                     ((uint32_t)0x00020000)        /* Channel 5 Transfer Complete clear */
#define  DMA_IFCR_CHTIF5                     ((uint32_t)0x00040000)        /* Channel 5 Half Transfer clear */
#define  DMA_IFCR_CTEIF5                     ((uint32_t)0x00080000)        /* Channel 5 Transfer Error clear */
#define  DMA_IFCR_CGIF6                      ((uint32_t)0x00100000)        /* Channel 6 Global interrupt clear */
#define  DMA_IFCR_CTCIF6                     ((uint32_t)0x00200000)        /* Channel 6 Transfer Complete clear */
#define  DMA_IFCR_CHTIF6                     ((uint32_t)0x00400000)        /* Channel 6 Half Transfer clear */
#define  DMA_IFCR_CTEIF6                     ((uint32_t)0x00800000)        /* Channel 6 Transfer Error clear */
#define  DMA_IFCR_CGIF7                      ((uint32_t)0x01000000)        /* Channel 7 Global interrupt clear */
#define  DMA_IFCR_CTCIF7                     ((uint32_t)0x02000000)        /* Channel 7 Transfer Complete clear */
#define  DMA_IFCR_CHTIF7                     ((uint32_t)0x04000000)        /* Channel 7 Half Transfer clear */
#define  DMA_IFCR_CTEIF7                     ((uint32_t)0x08000000)        /* Channel 7 Transfer Error clear */


/*******************  Bit definition for DMA_CCR1 register  *******************/
#define  DMA_CCR1_EN                         ((uint16_t)0x0001)            /* Channel enable*/
#define  DMA_CCR1_TCIE                       ((uint16_t)0x0002)            /* Transfer complete interrupt enable */
#define  DMA_CCR1_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR1_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR1_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR1_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR1_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR1_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR1_PSIZE                      ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR1_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR1_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR1_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR1_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR1_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR1_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits(Channel Priority level) */
#define  DMA_CCR1_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR1_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR1_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode */


/*******************  Bit definition for DMA_CCR2 register  *******************/
#define  DMA_CCR2_EN                         ((uint16_t)0x0001)            /* Channel enable */
#define  DMA_CCR2_TCIE                       ((uint16_t)0x0002)            /* ransfer complete interrupt enable */
#define  DMA_CCR2_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR2_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR2_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR2_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR2_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR2_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR2_PSIZE                      ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR2_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR2_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR2_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR2_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR2_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR2_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR2_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR2_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR2_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode */


/*******************  Bit definition for DMA_CCR3 register  *******************/
#define  DMA_CCR3_EN                         ((uint16_t)0x0001)            /* Channel enable */
#define  DMA_CCR3_TCIE                       ((uint16_t)0x0002)            /* Transfer complete interrupt enable */
#define  DMA_CCR3_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR3_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR3_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR3_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR3_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR3_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR3_PSIZE                      ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR3_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR3_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR3_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR3_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR3_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR3_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR3_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR3_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR3_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode */


/*******************  Bit definition for DMA_CCR4 register  *******************/
#define  DMA_CCR4_EN                         ((uint16_t)0x0001)            /* Channel enable */
#define  DMA_CCR4_TCIE                       ((uint16_t)0x0002)            /* Transfer complete interrupt enable */
#define  DMA_CCR4_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR4_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR4_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR4_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR4_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR4_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR4_PSIZE                      ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR4_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR4_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR4_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR4_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR4_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR4_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR4_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR4_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR4_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode */


/******************  Bit definition for DMA_CCR5 register  *******************/
#define  DMA_CCR5_EN                         ((uint16_t)0x0001)            /* Channel enable */
#define  DMA_CCR5_TCIE                       ((uint16_t)0x0002)            /* Transfer complete interrupt enable */
#define  DMA_CCR5_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR5_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR5_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR5_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR5_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR5_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR5_PSIZE                      ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR5_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR5_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR5_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR5_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR5_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR5_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR5_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR5_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR5_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode enable */


/*******************  Bit definition for DMA_CCR6 register  *******************/
#define  DMA_CCR6_EN                         ((uint16_t)0x0001)            /* Channel enable */
#define  DMA_CCR6_TCIE                       ((uint16_t)0x0002)            /* Transfer complete interrupt enable */
#define  DMA_CCR6_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR6_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR6_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR6_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR6_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR6_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR6_PSIZE                      ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR6_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR6_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR6_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR6_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR6_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR6_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR6_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR6_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR6_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode */


/*******************  Bit definition for DMA_CCR7 register  *******************/
#define  DMA_CCR7_EN                         ((uint16_t)0x0001)            /* Channel enable */
#define  DMA_CCR7_TCIE                       ((uint16_t)0x0002)            /* Transfer complete interrupt enable */
#define  DMA_CCR7_HTIE                       ((uint16_t)0x0004)            /* Half Transfer interrupt enable */
#define  DMA_CCR7_TEIE                       ((uint16_t)0x0008)            /* Transfer error interrupt enable */
#define  DMA_CCR7_DIR                        ((uint16_t)0x0010)            /* Data transfer direction */
#define  DMA_CCR7_CIRC                       ((uint16_t)0x0020)            /* Circular mode */
#define  DMA_CCR7_PINC                       ((uint16_t)0x0040)            /* Peripheral increment mode */
#define  DMA_CCR7_MINC                       ((uint16_t)0x0080)            /* Memory increment mode */

#define  DMA_CCR7_PSIZE            ,         ((uint16_t)0x0300)            /* PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR7_PSIZE_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  DMA_CCR7_PSIZE_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  DMA_CCR7_MSIZE                      ((uint16_t)0x0C00)            /* MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR7_MSIZE_0                    ((uint16_t)0x0400)            /* Bit 0 */
#define  DMA_CCR7_MSIZE_1                    ((uint16_t)0x0800)            /* Bit 1 */

#define  DMA_CCR7_PL                         ((uint16_t)0x3000)            /* PL[1:0] bits (Channel Priority level) */
#define  DMA_CCR7_PL_0                       ((uint16_t)0x1000)            /* Bit 0 */
#define  DMA_CCR7_PL_1                       ((uint16_t)0x2000)            /* Bit 1 */

#define  DMA_CCR7_MEM2MEM                    ((uint16_t)0x4000)            /* Memory to memory mode enable */


/******************  Bit definition for DMA_CNDTR1 register  ******************/
#define  DMA_CNDTR1_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CNDTR2 register  ******************/
#define  DMA_CNDTR2_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CNDTR3 register  ******************/
#define  DMA_CNDTR3_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CNDTR4 register  ******************/
#define  DMA_CNDTR4_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CNDTR5 register  ******************/
#define  DMA_CNDTR5_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CNDTR6 register  ******************/
#define  DMA_CNDTR6_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CNDTR7 register  ******************/
#define  DMA_CNDTR7_NDT                      ((uint16_t)0xFFFF)            /* Number of data to Transfer */


/******************  Bit definition for DMA_CPAR1 register  *******************/
#define  DMA_CPAR1_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CPAR2 register  *******************/
#define  DMA_CPAR2_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CPAR3 register  *******************/
#define  DMA_CPAR3_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CPAR4 register  *******************/
#define  DMA_CPAR4_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CPAR5 register  *******************/
#define  DMA_CPAR5_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CPAR6 register  *******************/
#define  DMA_CPAR6_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CPAR7 register  *******************/
#define  DMA_CPAR7_PA                        ((uint32_t)0xFFFFFFFF)        /* Peripheral Address */


/******************  Bit definition for DMA_CMAR1 register  *******************/
#define  DMA_CMAR1_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */


/******************  Bit definition for DMA_CMAR2 register  *******************/
#define  DMA_CMAR2_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */


/******************  Bit definition for DMA_CMAR3 register  *******************/
#define  DMA_CMAR3_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */


/******************  Bit definition for DMA_CMAR4 register  *******************/
#define  DMA_CMAR4_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */


/******************  Bit definition for DMA_CMAR5 register  *******************/
#define  DMA_CMAR5_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */


/******************  Bit definition for DMA_CMAR6 register  *******************/
#define  DMA_CMAR6_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */


/******************  Bit definition for DMA_CMAR7 register  *******************/
#define  DMA_CMAR7_MA                        ((uint32_t)0xFFFFFFFF)        /* Memory Address */



/******************************************************************************/
/*                                                                            */
/*                        Analog to Digital Converter                         */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for ADC_SR register  ********************/
#define  ADC_SR_AWD                          ((uint8_t)0x01)               /* Analog watchdog flag */
#define  ADC_SR_EOC                          ((uint8_t)0x02)               /* End of conversion */
#define  ADC_SR_JEOC                         ((uint8_t)0x04)               /* Injected channel end of conversion */
#define  ADC_SR_JSTRT                        ((uint8_t)0x08)               /* Injected channel Start flag */
#define  ADC_SR_STRT                         ((uint8_t)0x10)               /* Regular channel Start flag */


/*******************  Bit definition for ADC_CR1 register  ********************/
#define  ADC_CR1_AWDCH                       ((uint32_t)0x0000001F)        /* AWDCH[4:0] bits (Analog watchdog channel select bits) */
#define  ADC_CR1_AWDCH_0                     ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_CR1_AWDCH_1                     ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_CR1_AWDCH_2                     ((uint32_t)0x00000004)        /* Bit 2 */
#define  ADC_CR1_AWDCH_3                     ((uint32_t)0x00000008)        /* Bit 3 */
#define  ADC_CR1_AWDCH_4                     ((uint32_t)0x00000010)        /* Bit 4 */

#define  ADC_CR1_EOCIE                       ((uint32_t)0x00000020)        /* Interrupt enable for EOC */
#define  ADC_CR1_AWDIE                       ((uint32_t)0x00000040)        /* AAnalog Watchdog interrupt enable */
#define  ADC_CR1_JEOCIE                      ((uint32_t)0x00000080)        /* Interrupt enable for injected channels */
#define  ADC_CR1_SCAN                        ((uint32_t)0x00000100)        /* Scan mode */
#define  ADC_CR1_AWDSGL                      ((uint32_t)0x00000200)        /* Enable the watchdog on a single channel in scan mode */
#define  ADC_CR1_JAUTO                       ((uint32_t)0x00000400)        /* Automatic injected group conversion */
#define  ADC_CR1_DISCEN                      ((uint32_t)0x00000800)        /* Discontinuous mode on regular channels */
#define  ADC_CR1_JDISCEN                     ((uint32_t)0x00001000)        /* Discontinuous mode on injected channels */

#define  ADC_CR1_DISCNUM                     ((uint32_t)0x0000E000)        /* DISCNUM[2:0] bits (Discontinuous mode channel count) */
#define  ADC_CR1_DISCNUM_0                   ((uint32_t)0x00002000)        /* Bit 0 */
#define  ADC_CR1_DISCNUM_1                   ((uint32_t)0x00004000)        /* Bit 1 */
#define  ADC_CR1_DISCNUM_2                   ((uint32_t)0x00008000)        /* Bit 2 */

#define  ADC_CR1_DUALMOD                     ((uint32_t)0x000F0000)        /* DUALMOD[3:0] bits (Dual mode selection) */
#define  ADC_CR1_DUALMOD_0                   ((uint32_t)0x00010000)        /* Bit 0 */
#define  ADC_CR1_DUALMOD_1                   ((uint32_t)0x00020000)        /* Bit 1 */
#define  ADC_CR1_DUALMOD_2                   ((uint32_t)0x00040000)        /* Bit 2 */
#define  ADC_CR1_DUALMOD_3                   ((uint32_t)0x00080000)        /* Bit 3 */

#define  ADC_CR1_JAWDEN                      ((uint32_t)0x00400000)        /* Analog watchdog enable on injected channels */
#define  ADC_CR1_AWDEN                       ((uint32_t)0x00800000)        /* Analog watchdog enable on regular channels */

#define  ADC_CR1_CLKDIV                      ((uint32_t)0xFF000000)        /* ADCCLK divider */
  
/*******************  Bit definition for ADC_CR2 register  ********************/
#define  ADC_CR2_ADON                        ((uint32_t)0x00000001)        /* A/D Converter ON / OFF */
#define  ADC_CR2_CONT                        ((uint32_t)0x00000002)        /* Continuous Conversion */
#define  ADC_CR2_CAL                         ((uint32_t)0x00000004)        /* A/D Calibration */
#define  ADC_CR2_RSTCAL                      ((uint32_t)0x00000008)        /* Reset Calibration */
#define  ADC_CR2_DMA                         ((uint32_t)0x00000100)        /* Direct Memory access mode */
#define  ADC_CR2_ALIGN                       ((uint32_t)0x00000800)        /* Data Alignment */

#define  ADC_CR2_JEXTSEL                     ((uint32_t)0x00007000)        /* JEXTSEL[2:0] bits (External event select for injected group) */
#define  ADC_CR2_JEXTSEL_0                   ((uint32_t)0x00001000)        /* Bit 0 */
#define  ADC_CR2_JEXTSEL_1                   ((uint32_t)0x00002000)        /* Bit 1 */
#define  ADC_CR2_JEXTSEL_2                   ((uint32_t)0x00004000)        /* Bit 2 */

#define  ADC_CR2_JEXTTRIG                    ((uint32_t)0x00008000)        /* External Trigger Conversion mode for injected channels */

#define  ADC_CR2_EXTSEL                      ((uint32_t)0x000E0000)        /* EXTSEL[2:0] bits (External Event Select for regular group) */
#define  ADC_CR2_EXTSEL_0                    ((uint32_t)0x00020000)        /* Bit 0 */
#define  ADC_CR2_EXTSEL_1                    ((uint32_t)0x00040000)        /* Bit 1 */
#define  ADC_CR2_EXTSEL_2                    ((uint32_t)0x00080000)        /* Bit 2 */

#define  ADC_CR2_EXTTRIG                     ((uint32_t)0x00100000)        /* External Trigger Conversion mode for regular channels */
#define  ADC_CR2_JSWSTART                    ((uint32_t)0x00200000)        /* Start Conversion of injected channels */
#define  ADC_CR2_SWSTART                     ((uint32_t)0x00400000)        /* Start Conversion of regular channels */
#define  ADC_CR2_TSVREFE                     ((uint32_t)0x00800000)        /* Temperature Sensor and VREFINT Enable */
#define  ADC_CR2_JTRIGMOD                    ((uint32_t)0x03000000)        /* Injected trigger mode */


/******************  Bit definition for ADC_SMPR1 register  *******************/
#define  ADC_SMPR1_SMP10                     ((uint32_t)0x00000007)        /* SMP10[2:0] bits (Channel 10 Sample time selection) */
#define  ADC_SMPR1_SMP10_0                   ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_SMPR1_SMP10_1                   ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_SMPR1_SMP10_2                   ((uint32_t)0x00000004)        /* Bit 2 */

#define  ADC_SMPR1_SMP11                     ((uint32_t)0x00000038)        /* SMP11[2:0] bits (Channel 11 Sample time selection) */
#define  ADC_SMPR1_SMP11_0                   ((uint32_t)0x00000008)        /* Bit 0 */
#define  ADC_SMPR1_SMP11_1                   ((uint32_t)0x00000010)        /* Bit 1 */
#define  ADC_SMPR1_SMP11_2                   ((uint32_t)0x00000020)        /* Bit 2 */

#define  ADC_SMPR1_SMP12                     ((uint32_t)0x000001C0)        /* SMP12[2:0] bits (Channel 12 Sample time selection) */
#define  ADC_SMPR1_SMP12_0                   ((uint32_t)0x00000040)        /* Bit 0 */
#define  ADC_SMPR1_SMP12_1                   ((uint32_t)0x00000080)        /* Bit 1 */
#define  ADC_SMPR1_SMP12_2                   ((uint32_t)0x00000100)        /* Bit 2 */

#define  ADC_SMPR1_SMP13                     ((uint32_t)0x00000E00)        /* SMP13[2:0] bits (Channel 13 Sample time selection) */
#define  ADC_SMPR1_SMP13_0                   ((uint32_t)0x00000200)        /* Bit 0 */
#define  ADC_SMPR1_SMP13_1                   ((uint32_t)0x00000400)        /* Bit 1 */
#define  ADC_SMPR1_SMP13_2                   ((uint32_t)0x00000800)        /* Bit 2 */

#define  ADC_SMPR1_SMP14                     ((uint32_t)0x00007000)        /* SMP14[2:0] bits (Channel 14 Sample time selection) */
#define  ADC_SMPR1_SMP14_0                   ((uint32_t)0x00001000)        /* Bit 0 */
#define  ADC_SMPR1_SMP14_1                   ((uint32_t)0x00002000)        /* Bit 1 */
#define  ADC_SMPR1_SMP14_2                   ((uint32_t)0x00004000)        /* Bit 2 */

#define  ADC_SMPR1_SMP15                     ((uint32_t)0x00038000)        /* SMP15[2:0] bits (Channel 15 Sample time selection) */
#define  ADC_SMPR1_SMP15_0                   ((uint32_t)0x00008000)        /* Bit 0 */
#define  ADC_SMPR1_SMP15_1                   ((uint32_t)0x00010000)        /* Bit 1 */
#define  ADC_SMPR1_SMP15_2                   ((uint32_t)0x00020000)        /* Bit 2 */

#define  ADC_SMPR1_SMP16                     ((uint32_t)0x001C0000)        /* SMP16[2:0] bits (Channel 16 Sample time selection) */
#define  ADC_SMPR1_SMP16_0                   ((uint32_t)0x00040000)        /* Bit 0 */
#define  ADC_SMPR1_SMP16_1                   ((uint32_t)0x00080000)        /* Bit 1 */
#define  ADC_SMPR1_SMP16_2                   ((uint32_t)0x00100000)        /* Bit 2 */

#define  ADC_SMPR1_SMP17                     ((uint32_t)0x00E00000)        /* SMP17[2:0] bits (Channel 17 Sample time selection) */
#define  ADC_SMPR1_SMP17_0                   ((uint32_t)0x00200000)        /* Bit 0 */
#define  ADC_SMPR1_SMP17_1                   ((uint32_t)0x00400000)        /* Bit 1 */
#define  ADC_SMPR1_SMP17_2                   ((uint32_t)0x00800000)        /* Bit 2 */


/******************  Bit definition for ADC_SMPR2 register  *******************/
#define  ADC_SMPR2_SMP0                      ((uint32_t)0x00000007)        /* SMP0[2:0] bits (Channel 0 Sample time selection) */
#define  ADC_SMPR2_SMP0_0                    ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_SMPR2_SMP0_1                    ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_SMPR2_SMP0_2                    ((uint32_t)0x00000004)        /* Bit 2 */

#define  ADC_SMPR2_SMP1                      ((uint32_t)0x00000038)        /* SMP1[2:0] bits (Channel 1 Sample time selection) */
#define  ADC_SMPR2_SMP1_0                    ((uint32_t)0x00000008)        /* Bit 0 */
#define  ADC_SMPR2_SMP1_1                    ((uint32_t)0x00000010)        /* Bit 1 */
#define  ADC_SMPR2_SMP1_2                    ((uint32_t)0x00000020)        /* Bit 2 */

#define  ADC_SMPR2_SMP2                      ((uint32_t)0x000001C0)        /* SMP2[2:0] bits (Channel 2 Sample time selection) */
#define  ADC_SMPR2_SMP2_0                    ((uint32_t)0x00000040)        /* Bit 0 */
#define  ADC_SMPR2_SMP2_1                    ((uint32_t)0x00000080)        /* Bit 1 */
#define  ADC_SMPR2_SMP2_2                    ((uint32_t)0x00000100)        /* Bit 2 */

#define  ADC_SMPR2_SMP3                      ((uint32_t)0x00000E00)        /* SMP3[2:0] bits (Channel 3 Sample time selection) */
#define  ADC_SMPR2_SMP3_0                    ((uint32_t)0x00000200)        /* Bit 0 */
#define  ADC_SMPR2_SMP3_1                    ((uint32_t)0x00000400)        /* Bit 1 */
#define  ADC_SMPR2_SMP3_2                    ((uint32_t)0x00000800)        /* Bit 2 */

#define  ADC_SMPR2_SMP4                      ((uint32_t)0x00007000)        /* SMP4[2:0] bits (Channel 4 Sample time selection) */
#define  ADC_SMPR2_SMP4_0                    ((uint32_t)0x00001000)        /* Bit 0 */
#define  ADC_SMPR2_SMP4_1                    ((uint32_t)0x00002000)        /* Bit 1 */
#define  ADC_SMPR2_SMP4_2                    ((uint32_t)0x00004000)        /* Bit 2 */

#define  ADC_SMPR2_SMP5                      ((uint32_t)0x00038000)        /* SMP5[2:0] bits (Channel 5 Sample time selection) */
#define  ADC_SMPR2_SMP5_0                    ((uint32_t)0x00008000)        /* Bit 0 */
#define  ADC_SMPR2_SMP5_1                    ((uint32_t)0x00010000)        /* Bit 1 */
#define  ADC_SMPR2_SMP5_2                    ((uint32_t)0x00020000)        /* Bit 2 */

#define  ADC_SMPR2_SMP6                      ((uint32_t)0x001C0000)        /* SMP6[2:0] bits (Channel 6 Sample time selection) */
#define  ADC_SMPR2_SMP6_0                    ((uint32_t)0x00040000)        /* Bit 0 */
#define  ADC_SMPR2_SMP6_1                    ((uint32_t)0x00080000)        /* Bit 1 */
#define  ADC_SMPR2_SMP6_2                    ((uint32_t)0x00100000)        /* Bit 2 */

#define  ADC_SMPR2_SMP7                      ((uint32_t)0x00E00000)        /* SMP7[2:0] bits (Channel 7 Sample time selection) */
#define  ADC_SMPR2_SMP7_0                    ((uint32_t)0x00200000)        /* Bit 0 */
#define  ADC_SMPR2_SMP7_1                    ((uint32_t)0x00400000)        /* Bit 1 */
#define  ADC_SMPR2_SMP7_2                    ((uint32_t)0x00800000)        /* Bit 2 */

#define  ADC_SMPR2_SMP8                      ((uint32_t)0x07000000)        /* SMP8[2:0] bits (Channel 8 Sample time selection) */
#define  ADC_SMPR2_SMP8_0                    ((uint32_t)0x01000000)        /* Bit 0 */
#define  ADC_SMPR2_SMP8_1                    ((uint32_t)0x02000000)        /* Bit 1 */
#define  ADC_SMPR2_SMP8_2                    ((uint32_t)0x04000000)        /* Bit 2 */

#define  ADC_SMPR2_SMP9                      ((uint32_t)0x38000000)        /* SMP9[2:0] bits (Channel 9 Sample time selection) */
#define  ADC_SMPR2_SMP9_0                    ((uint32_t)0x08000000)        /* Bit 0 */
#define  ADC_SMPR2_SMP9_1                    ((uint32_t)0x10000000)        /* Bit 1 */
#define  ADC_SMPR2_SMP9_2                    ((uint32_t)0x20000000)        /* Bit 2 */


/******************  Bit definition for ADC_JOFR1 register  *******************/
#define  ADC_JOFR1_JOFFSET1                  ((uint16_t)0x0FFF)            /* Data offset for injected channel 1 */


/******************  Bit definition for ADC_JOFR2 register  *******************/
#define  ADC_JOFR2_JOFFSET2                  ((uint16_t)0x0FFF)            /* Data offset for injected channel 2 */


/******************  Bit definition for ADC_JOFR3 register  *******************/
#define  ADC_JOFR3_JOFFSET3                  ((uint16_t)0x0FFF)            /* Data offset for injected channel 3 */


/******************  Bit definition for ADC_JOFR4 register  *******************/
#define  ADC_JOFR4_JOFFSET4                  ((uint16_t)0x0FFF)            /* Data offset for injected channel 4 */


/*******************  Bit definition for ADC_HTR register  ********************/
#define  ADC_HTR_HT                          ((uint16_t)0x0FFF)            /* Analog watchdog high threshold */


/*******************  Bit definition for ADC_LTR register  ********************/
#define  ADC_LTR_LT                          ((uint16_t)0x0FFF)            /* Analog watchdog low threshold */


/*******************  Bit definition for ADC_SQR1 register  *******************/
#define  ADC_SQR1_SQ13                       ((uint32_t)0x0000001F)        /* SQ13[4:0] bits (13th conversion in regular sequence) */
#define  ADC_SQR1_SQ13_0                     ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_SQR1_SQ13_1                     ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_SQR1_SQ13_2                     ((uint32_t)0x00000004)        /* Bit 2 */
#define  ADC_SQR1_SQ13_3                     ((uint32_t)0x00000008)        /* Bit 3 */
#define  ADC_SQR1_SQ13_4                     ((uint32_t)0x00000010)        /* Bit 4 */

#define  ADC_SQR1_SQ14                       ((uint32_t)0x000003E0)        /* SQ14[4:0] bits (14th conversion in regular sequence) */
#define  ADC_SQR1_SQ14_0                     ((uint32_t)0x00000020)        /* Bit 0 */
#define  ADC_SQR1_SQ14_1                     ((uint32_t)0x00000040)        /* Bit 1 */
#define  ADC_SQR1_SQ14_2                     ((uint32_t)0x00000080)        /* Bit 2 */
#define  ADC_SQR1_SQ14_3                     ((uint32_t)0x00000100)        /* Bit 3 */
#define  ADC_SQR1_SQ14_4                     ((uint32_t)0x00000200)        /* Bit 4 */

#define  ADC_SQR1_SQ15                       ((uint32_t)0x00007C00)        /* SQ15[4:0] bits (15th conversion in regular sequence) */
#define  ADC_SQR1_SQ15_0                     ((uint32_t)0x00000400)        /* Bit 0 */
#define  ADC_SQR1_SQ15_1                     ((uint32_t)0x00000800)        /* Bit 1 */
#define  ADC_SQR1_SQ15_2                     ((uint32_t)0x00001000)        /* Bit 2 */
#define  ADC_SQR1_SQ15_3                     ((uint32_t)0x00002000)        /* Bit 3 */
#define  ADC_SQR1_SQ15_4                     ((uint32_t)0x00004000)        /* Bit 4 */

#define  ADC_SQR1_SQ16                       ((uint32_t)0x000F8000)        /* SQ16[4:0] bits (16th conversion in regular sequence) */
#define  ADC_SQR1_SQ16_0                     ((uint32_t)0x00008000)        /* Bit 0 */
#define  ADC_SQR1_SQ16_1                     ((uint32_t)0x00010000)        /* Bit 1 */
#define  ADC_SQR1_SQ16_2                     ((uint32_t)0x00020000)        /* Bit 2 */
#define  ADC_SQR1_SQ16_3                     ((uint32_t)0x00040000)        /* Bit 3 */
#define  ADC_SQR1_SQ16_4                     ((uint32_t)0x00080000)        /* Bit 4 */

#define  ADC_SQR1_L                          ((uint32_t)0x00F00000)        /* L[3:0] bits (Regular channel sequence length) */
#define  ADC_SQR1_L_0                        ((uint32_t)0x00100000)        /* Bit 0 */
#define  ADC_SQR1_L_1                        ((uint32_t)0x00200000)        /* Bit 1 */
#define  ADC_SQR1_L_2                        ((uint32_t)0x00400000)        /* Bit 2 */
#define  ADC_SQR1_L_3                        ((uint32_t)0x00800000)        /* Bit 3 */


/*******************  Bit definition for ADC_SQR2 register  *******************/
#define  ADC_SQR2_SQ7                        ((uint32_t)0x0000001F)        /* SQ7[4:0] bits (7th conversion in regular sequence) */
#define  ADC_SQR2_SQ7_0                      ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_SQR2_SQ7_1                      ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_SQR2_SQ7_2                      ((uint32_t)0x00000004)        /* Bit 2 */
#define  ADC_SQR2_SQ7_3                      ((uint32_t)0x00000008)        /* Bit 3 */
#define  ADC_SQR2_SQ7_4                      ((uint32_t)0x00000010)        /* Bit 4 */

#define  ADC_SQR2_SQ8                        ((uint32_t)0x000003E0)        /* SQ8[4:0] bits (8th conversion in regular sequence) */
#define  ADC_SQR2_SQ8_0                      ((uint32_t)0x00000020)        /* Bit 0 */
#define  ADC_SQR2_SQ8_1                      ((uint32_t)0x00000040)        /* Bit 1 */
#define  ADC_SQR2_SQ8_2                      ((uint32_t)0x00000080)        /* Bit 2 */
#define  ADC_SQR2_SQ8_3                      ((uint32_t)0x00000100)        /* Bit 3 */
#define  ADC_SQR2_SQ8_4                      ((uint32_t)0x00000200)        /* Bit 4 */

#define  ADC_SQR2_SQ9                        ((uint32_t)0x00007C00)        /* SQ9[4:0] bits (9th conversion in regular sequence) */
#define  ADC_SQR2_SQ9_0                      ((uint32_t)0x00000400)        /* Bit 0 */
#define  ADC_SQR2_SQ9_1                      ((uint32_t)0x00000800)        /* Bit 1 */
#define  ADC_SQR2_SQ9_2                      ((uint32_t)0x00001000)        /* Bit 2 */
#define  ADC_SQR2_SQ9_3                      ((uint32_t)0x00002000)        /* Bit 3 */
#define  ADC_SQR2_SQ9_4                      ((uint32_t)0x00004000)        /* Bit 4 */

#define  ADC_SQR2_SQ10                       ((uint32_t)0x000F8000)        /* SQ10[4:0] bits (10th conversion in regular sequence) */
#define  ADC_SQR2_SQ10_0                     ((uint32_t)0x00008000)        /* Bit 0 */
#define  ADC_SQR2_SQ10_1                     ((uint32_t)0x00010000)        /* Bit 1 */
#define  ADC_SQR2_SQ10_2                     ((uint32_t)0x00020000)        /* Bit 2 */
#define  ADC_SQR2_SQ10_3                     ((uint32_t)0x00040000)        /* Bit 3 */
#define  ADC_SQR2_SQ10_4                     ((uint32_t)0x00080000)        /* Bit 4 */

#define  ADC_SQR2_SQ11                       ((uint32_t)0x01F00000)        /* SQ11[4:0] bits (11th conversion in regular sequence) */
#define  ADC_SQR2_SQ11_0                     ((uint32_t)0x00100000)        /* Bit 0 */
#define  ADC_SQR2_SQ11_1                     ((uint32_t)0x00200000)        /* Bit 1 */
#define  ADC_SQR2_SQ11_2                     ((uint32_t)0x00400000)        /* Bit 2 */
#define  ADC_SQR2_SQ11_3                     ((uint32_t)0x00800000)        /* Bit 3 */
#define  ADC_SQR2_SQ11_4                     ((uint32_t)0x01000000)        /* Bit 4 */

#define  ADC_SQR2_SQ12                       ((uint32_t)0x3E000000)        /* SQ12[4:0] bits (12th conversion in regular sequence) */
#define  ADC_SQR2_SQ12_0                     ((uint32_t)0x02000000)        /* Bit 0 */
#define  ADC_SQR2_SQ12_1                     ((uint32_t)0x04000000)        /* Bit 1 */
#define  ADC_SQR2_SQ12_2                     ((uint32_t)0x08000000)        /* Bit 2 */
#define  ADC_SQR2_SQ12_3                     ((uint32_t)0x10000000)        /* Bit 3 */
#define  ADC_SQR2_SQ12_4                     ((uint32_t)0x20000000)        /* Bit 4 */


/*******************  Bit definition for ADC_SQR3 register  *******************/
#define  ADC_SQR3_SQ1                        ((uint32_t)0x0000001F)        /* SQ1[4:0] bits (1st conversion in regular sequence) */
#define  ADC_SQR3_SQ1_0                      ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_SQR3_SQ1_1                      ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_SQR3_SQ1_2                      ((uint32_t)0x00000004)        /* Bit 2 */
#define  ADC_SQR3_SQ1_3                      ((uint32_t)0x00000008)        /* Bit 3 */
#define  ADC_SQR3_SQ1_4                      ((uint32_t)0x00000010)        /* Bit 4 */

#define  ADC_SQR3_SQ2                        ((uint32_t)0x000003E0)        /* SQ2[4:0] bits (2nd conversion in regular sequence) */
#define  ADC_SQR3_SQ2_0                      ((uint32_t)0x00000020)        /* Bit 0 */
#define  ADC_SQR3_SQ2_1                      ((uint32_t)0x00000040)        /* Bit 1 */
#define  ADC_SQR3_SQ2_2                      ((uint32_t)0x00000080)        /* Bit 2 */
#define  ADC_SQR3_SQ2_3                      ((uint32_t)0x00000100)        /* Bit 3 */
#define  ADC_SQR3_SQ2_4                      ((uint32_t)0x00000200)        /* Bit 4 */

#define  ADC_SQR3_SQ3                        ((uint32_t)0x00007C00)        /* SQ3[4:0] bits (3rd conversion in regular sequence) */
#define  ADC_SQR3_SQ3_0                      ((uint32_t)0x00000400)        /* Bit 0 */
#define  ADC_SQR3_SQ3_1                      ((uint32_t)0x00000800)        /* Bit 1 */
#define  ADC_SQR3_SQ3_2                      ((uint32_t)0x00001000)        /* Bit 2 */
#define  ADC_SQR3_SQ3_3                      ((uint32_t)0x00002000)        /* Bit 3 */
#define  ADC_SQR3_SQ3_4                      ((uint32_t)0x00004000)        /* Bit 4 */

#define  ADC_SQR3_SQ4                        ((uint32_t)0x000F8000)        /* SQ4[4:0] bits (4th conversion in regular sequence) */
#define  ADC_SQR3_SQ4_0                      ((uint32_t)0x00008000)        /* Bit 0 */
#define  ADC_SQR3_SQ4_1                      ((uint32_t)0x00010000)        /* Bit 1 */
#define  ADC_SQR3_SQ4_2                      ((uint32_t)0x00020000)        /* Bit 2 */
#define  ADC_SQR3_SQ4_3                      ((uint32_t)0x00040000)        /* Bit 3 */
#define  ADC_SQR3_SQ4_4                      ((uint32_t)0x00080000)        /* Bit 4 */

#define  ADC_SQR3_SQ5                        ((uint32_t)0x01F00000)        /* SQ5[4:0] bits (5th conversion in regular sequence) */
#define  ADC_SQR3_SQ5_0                      ((uint32_t)0x00100000)        /* Bit 0 */
#define  ADC_SQR3_SQ5_1                      ((uint32_t)0x00200000)        /* Bit 1 */
#define  ADC_SQR3_SQ5_2                      ((uint32_t)0x00400000)        /* Bit 2 */
#define  ADC_SQR3_SQ5_3                      ((uint32_t)0x00800000)        /* Bit 3 */
#define  ADC_SQR3_SQ5_4                      ((uint32_t)0x01000000)        /* Bit 4 */

#define  ADC_SQR3_SQ6                        ((uint32_t)0x3E000000)        /* SQ6[4:0] bits (6th conversion in regular sequence) */
#define  ADC_SQR3_SQ6_0                      ((uint32_t)0x02000000)        /* Bit 0 */
#define  ADC_SQR3_SQ6_1                      ((uint32_t)0x04000000)        /* Bit 1 */
#define  ADC_SQR3_SQ6_2                      ((uint32_t)0x08000000)        /* Bit 2 */
#define  ADC_SQR3_SQ6_3                      ((uint32_t)0x10000000)        /* Bit 3 */
#define  ADC_SQR3_SQ6_4                      ((uint32_t)0x20000000)        /* Bit 4 */


/*******************  Bit definition for ADC_JSQR register  *******************/
#define  ADC_JSQR_JSQ1                       ((uint32_t)0x0000001F)        /* JSQ1[4:0] bits (1st conversion in injected sequence) */  
#define  ADC_JSQR_JSQ1_0                     ((uint32_t)0x00000001)        /* Bit 0 */
#define  ADC_JSQR_JSQ1_1                     ((uint32_t)0x00000002)        /* Bit 1 */
#define  ADC_JSQR_JSQ1_2                     ((uint32_t)0x00000004)        /* Bit 2 */
#define  ADC_JSQR_JSQ1_3                     ((uint32_t)0x00000008)        /* Bit 3 */
#define  ADC_JSQR_JSQ1_4                     ((uint32_t)0x00000010)        /* Bit 4 */

#define  ADC_JSQR_JSQ2                       ((uint32_t)0x000003E0)        /* JSQ2[4:0] bits (2nd conversion in injected sequence) */
#define  ADC_JSQR_JSQ2_0                     ((uint32_t)0x00000020)        /* Bit 0 */
#define  ADC_JSQR_JSQ2_1                     ((uint32_t)0x00000040)        /* Bit 1 */
#define  ADC_JSQR_JSQ2_2                     ((uint32_t)0x00000080)        /* Bit 2 */
#define  ADC_JSQR_JSQ2_3                     ((uint32_t)0x00000100)        /* Bit 3 */
#define  ADC_JSQR_JSQ2_4                     ((uint32_t)0x00000200)        /* Bit 4 */

#define  ADC_JSQR_JSQ3                       ((uint32_t)0x00007C00)        /* JSQ3[4:0] bits (3rd conversion in injected sequence) */
#define  ADC_JSQR_JSQ3_0                     ((uint32_t)0x00000400)        /* Bit 0 */
#define  ADC_JSQR_JSQ3_1                     ((uint32_t)0x00000800)        /* Bit 1 */
#define  ADC_JSQR_JSQ3_2                     ((uint32_t)0x00001000)        /* Bit 2 */
#define  ADC_JSQR_JSQ3_3                     ((uint32_t)0x00002000)        /* Bit 3 */
#define  ADC_JSQR_JSQ3_4                     ((uint32_t)0x00004000)        /* Bit 4 */

#define  ADC_JSQR_JSQ4                       ((uint32_t)0x000F8000)        /* JSQ4[4:0] bits (4th conversion in injected sequence) */
#define  ADC_JSQR_JSQ4_0                     ((uint32_t)0x00008000)        /* Bit 0 */
#define  ADC_JSQR_JSQ4_1                     ((uint32_t)0x00010000)        /* Bit 1 */
#define  ADC_JSQR_JSQ4_2                     ((uint32_t)0x00020000)        /* Bit 2 */
#define  ADC_JSQR_JSQ4_3                     ((uint32_t)0x00040000)        /* Bit 3 */
#define  ADC_JSQR_JSQ4_4                     ((uint32_t)0x00080000)        /* Bit 4 */

#define  ADC_JSQR_JL                         ((uint32_t)0x00300000)        /* JL[1:0] bits (Injected Sequence length) */
#define  ADC_JSQR_JL_0                       ((uint32_t)0x00100000)        /* Bit 0 */
#define  ADC_JSQR_JL_1                       ((uint32_t)0x00200000)        /* Bit 1 */


/*******************  Bit definition for ADC_JDR1 register  *******************/
#define  ADC_JDR1_JDATA                      ((uint16_t)0xFFFF)            /* Injected data */


/*******************  Bit definition for ADC_JDR2 register  *******************/
#define  ADC_JDR2_JDATA                      ((uint16_t)0xFFFF)            /* Injected data */


/*******************  Bit definition for ADC_JDR3 register  *******************/
#define  ADC_JDR3_JDATA                      ((uint16_t)0xFFFF)            /* Injected data */


/*******************  Bit definition for ADC_JDR4 register  *******************/
#define  ADC_JDR4_JDATA                      ((uint16_t)0xFFFF)            /* Injected data */


/********************  Bit definition for ADC_DR register  ********************/
#define  ADC_DR_DATA                         ((uint32_t)0x0000FFFF)        /* Regular data */
#define  ADC_DR_ADC2DATA                     ((uint32_t)0xFFFF0000)        /* ADC2 data */


/******************************************************************************/
/*                                                                            */
/*                                    TIM                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                         ((uint16_t)0x0001)            /* Counter enable */
#define  TIM_CR1_UDIS                        ((uint16_t)0x0002)            /* Update disable */
#define  TIM_CR1_URS                         ((uint16_t)0x0004)            /* Update request source */
#define  TIM_CR1_OPM                         ((uint16_t)0x0008)            /* One pulse mode */
#define  TIM_CR1_DIR                         ((uint16_t)0x0010)            /* Direction */

#define  TIM_CR1_CMS                         ((uint16_t)0x0060)            /* CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                       ((uint16_t)0x0020)            /* Bit 0 */
#define  TIM_CR1_CMS_1                       ((uint16_t)0x0040)            /* Bit 1 */

#define  TIM_CR1_ARPE                        ((uint16_t)0x0080)            /* Auto-reload preload enable */

#define  TIM_CR1_CKD                         ((uint16_t)0x0300)            /* CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                       ((uint16_t)0x0100)            /* Bit 0 */
#define  TIM_CR1_CKD_1                       ((uint16_t)0x0200)            /* Bit 1 */


/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCPC                        ((uint16_t)0x0001)            /* Capture/Compare Preloaded Control */
#define  TIM_CR2_CCUS                        ((uint16_t)0x0004)            /* Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        ((uint16_t)0x0008)            /* Capture/Compare DMA Selection */

#define  TIM_CR2_MMS                         ((uint16_t)0x0070)            /* MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint16_t)0x0010)            /* Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint16_t)0x0020)            /* Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint16_t)0x0040)            /* Bit 2 */

#define  TIM_CR2_TI1S                        ((uint16_t)0x0080)            /* TI1 Selection */
#define  TIM_CR2_OIS1                        ((uint16_t)0x0100)            /* Output Idle state 1 (OC1 output) */
#define  TIM_CR2_OIS1N                       ((uint16_t)0x0200)            /* Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        ((uint16_t)0x0400)            /* Output Idle state 2 (OC2 output) */
#define  TIM_CR2_OIS2N                       ((uint16_t)0x0800)            /* Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        ((uint16_t)0x1000)            /* Output Idle state 3 (OC3 output) */
#define  TIM_CR2_OIS3N                       ((uint16_t)0x2000)            /* Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        ((uint16_t)0x4000)            /* Output Idle state 4 (OC4 output) */


/*******************  Bit definition for TIM_SMCR register  *******************/
#define  TIM_SMCR_SMS                        ((uint16_t)0x0007)            /* SMS[2:0] bits (Slave mode selection) */
#define  TIM_SMCR_SMS_0                      ((uint16_t)0x0001)            /* Bit 0 */
#define  TIM_SMCR_SMS_1                      ((uint16_t)0x0002)            /* Bit 1 */
#define  TIM_SMCR_SMS_2                      ((uint16_t)0x0004)            /* Bit 2 */

#define  TIM_SMCR_TS                         ((uint16_t)0x0070)            /* TS[2:0] bits (Trigger selection) */
#define  TIM_SMCR_TS_0                       ((uint16_t)0x0010)            /* Bit 0 */
#define  TIM_SMCR_TS_1                       ((uint16_t)0x0020)            /* Bit 1 */
#define  TIM_SMCR_TS_2                       ((uint16_t)0x0040)            /* Bit 2 */

#define  TIM_SMCR_MSM                        ((uint16_t)0x0080)            /* Master/slave mode */

#define  TIM_SMCR_ETF                        ((uint16_t)0x0F00)            /* ETF[3:0] bits (External trigger filter) */
#define  TIM_SMCR_ETF_0                      ((uint16_t)0x0100)            /* Bit 0 */
#define  TIM_SMCR_ETF_1                      ((uint16_t)0x0200)            /* Bit 1 */
#define  TIM_SMCR_ETF_2                      ((uint16_t)0x0400)            /* Bit 2 */
#define  TIM_SMCR_ETF_3                      ((uint16_t)0x0800)            /* Bit 3 */

#define  TIM_SMCR_ETPS                       ((uint16_t)0x3000)            /* ETPS[1:0] bits (External trigger prescaler) */
#define  TIM_SMCR_ETPS_0                     ((uint16_t)0x1000)            /* Bit 0 */
#define  TIM_SMCR_ETPS_1                     ((uint16_t)0x2000)            /* Bit 1 */

#define  TIM_SMCR_ECE                        ((uint16_t)0x4000)            /* External clock enable */
#define  TIM_SMCR_ETP                        ((uint16_t)0x8000)            /* External trigger polarity */


/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        ((uint16_t)0x0001)            /* Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint16_t)0x0002)            /* Capture/Compare 1 interrupt enable */
#define  TIM_DIER_CC2IE                      ((uint16_t)0x0004)            /* Capture/Compare 2 interrupt enable */
#define  TIM_DIER_CC3IE                      ((uint16_t)0x0008)            /* Capture/Compare 3 interrupt enable */
#define  TIM_DIER_CC4IE                      ((uint16_t)0x0010)            /* Capture/Compare 4 interrupt enable */
#define  TIM_DIER_COMIE                      ((uint16_t)0x0020)            /* COM interrupt enable */
#define  TIM_DIER_TIE                        ((uint16_t)0x0040)            /* Trigger interrupt enable */
#define  TIM_DIER_BIE                        ((uint16_t)0x0080)            /* Break interrupt enable */
#define  TIM_DIER_UDE                        ((uint16_t)0x0100)            /* Update DMA request enable */
#define  TIM_DIER_CC1DE                      ((uint16_t)0x0200)            /* Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint16_t)0x0400)            /* Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint16_t)0x0800)            /* Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint16_t)0x1000)            /* Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint16_t)0x2000)            /* COM DMA request enable */
#define  TIM_DIER_TDE                        ((uint16_t)0x4000)            /* Trigger DMA request enable */


/********************  Bit definition for TIM_SR register  ********************/
#define  TIM_SR_UIF                          ((uint16_t)0x0001)            /* Update interrupt Flag */
#define  TIM_SR_CC1IF                        ((uint16_t)0x0002)            /* Capture/Compare 1 interrupt Flag */
#define  TIM_SR_CC2IF                        ((uint16_t)0x0004)            /* Capture/Compare 2 interrupt Flag */
#define  TIM_SR_CC3IF                        ((uint16_t)0x0008)            /* Capture/Compare 3 interrupt Flag */
#define  TIM_SR_CC4IF                        ((uint16_t)0x0010)            /* Capture/Compare 4 interrupt Flag */
#define  TIM_SR_COMIF                        ((uint16_t)0x0020)            /* COM interrupt Flag */
#define  TIM_SR_TIF                          ((uint16_t)0x0040)            /* Trigger interrupt Flag */
#define  TIM_SR_BIF                          ((uint16_t)0x0080)            /* Break interrupt Flag */
#define  TIM_SR_CC1OF                        ((uint16_t)0x0200)            /* Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint16_t)0x0400)            /* Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint16_t)0x0800)            /* Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint16_t)0x1000)            /* Capture/Compare 4 Overcapture Flag */


/*******************  Bit definition for TIM_EGR register  ********************/
#define  TIM_EGR_UG                          ((uint8_t)0x01)               /* Update Generation */
#define  TIM_EGR_CC1G                        ((uint8_t)0x02)               /* Capture/Compare 1 Generation */
#define  TIM_EGR_CC2G                        ((uint8_t)0x04)               /* Capture/Compare 2 Generation */
#define  TIM_EGR_CC3G                        ((uint8_t)0x08)               /* Capture/Compare 3 Generation */
#define  TIM_EGR_CC4G                        ((uint8_t)0x10)               /* Capture/Compare 4 Generation */
#define  TIM_EGR_COMG                        ((uint8_t)0x20)               /* Capture/Compare Control Update Generation */
#define  TIM_EGR_TG                          ((uint8_t)0x40)               /* Trigger Generation */
#define  TIM_EGR_BG                          ((uint8_t)0x80)               /* Break Generation */


/******************  Bit definition for TIM_CCMR1 register  *******************/
#define  TIM_CCMR1_CC1S                      ((uint16_t)0x0003)            /* CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint16_t)0x0001)            /* Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint16_t)0x0002)            /* Bit 1 */

#define  TIM_CCMR1_OC1FE                     ((uint16_t)0x0004)            /* Output Compare 1 Fast enable */
#define  TIM_CCMR1_OC1PE                     ((uint16_t)0x0008)            /* Output Compare 1 Preload enable */

#define  TIM_CCMR1_OC1M                      ((uint16_t)0x0070)            /* OC1M[2:0] bits (Output Compare 1 Mode) */
#define  TIM_CCMR1_OC1M_0                    ((uint16_t)0x0010)            /* Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint16_t)0x0020)            /* Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint16_t)0x0040)            /* Bit 2 */

#define  TIM_CCMR1_OC1CE                     ((uint16_t)0x0080)            /* Output Compare 1Clear Enable */

#define  TIM_CCMR1_CC2S                      ((uint16_t)0x0300)            /* CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  TIM_CCMR1_OC2FE                     ((uint16_t)0x0400)            /* Output Compare 2 Fast enable */
#define  TIM_CCMR1_OC2PE                     ((uint16_t)0x0800)            /* Output Compare 2 Preload enable */

#define  TIM_CCMR1_OC2M                      ((uint16_t)0x7000)            /* OC2M[2:0] bits (Output Compare 2 Mode) */
#define  TIM_CCMR1_OC2M_0                    ((uint16_t)0x1000)            /* Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint16_t)0x2000)            /* Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint16_t)0x4000)            /* Bit 2 */

#define  TIM_CCMR1_OC2CE                     ((uint16_t)0x8000)            /* Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR1_IC1PSC                    ((uint16_t)0x000C)            /* IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  ((uint16_t)0x0004)            /* Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  ((uint16_t)0x0008)            /* Bit 1 */

#define  TIM_CCMR1_IC1F                      ((uint16_t)0x00F0)            /* IC1F[3:0] bits (Input Capture 1 Filter) */
#define  TIM_CCMR1_IC1F_0                    ((uint16_t)0x0010)            /* Bit 0 */
#define  TIM_CCMR1_IC1F_1                    ((uint16_t)0x0020)            /* Bit 1 */
#define  TIM_CCMR1_IC1F_2                    ((uint16_t)0x0040)            /* Bit 2 */
#define  TIM_CCMR1_IC1F_3                    ((uint16_t)0x0080)            /* Bit 3 */

#define  TIM_CCMR1_IC2PSC                    ((uint16_t)0x0C00)            /* IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define  TIM_CCMR1_IC2PSC_0                  ((uint16_t)0x0400)            /* Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  ((uint16_t)0x0800)            /* Bit 1 */

#define  TIM_CCMR1_IC2F                      ((uint16_t)0xF000)            /* IC2F[3:0] bits (Input Capture 2 Filter) */
#define  TIM_CCMR1_IC2F_0                    ((uint16_t)0x1000)            /* Bit 0 */
#define  TIM_CCMR1_IC2F_1                    ((uint16_t)0x2000)            /* Bit 1 */
#define  TIM_CCMR1_IC2F_2                    ((uint16_t)0x4000)            /* Bit 2 */
#define  TIM_CCMR1_IC2F_3                    ((uint16_t)0x8000)            /* Bit 3 */


/******************  Bit definition for TIM_CCMR2 register  *******************/
#define  TIM_CCMR2_CC3S                      ((uint16_t)0x0003)            /* CC3S[1:0] bits (Capture/Compare 3 Selection) */
#define  TIM_CCMR2_CC3S_0                    ((uint16_t)0x0001)            /* Bit 0 */
#define  TIM_CCMR2_CC3S_1                    ((uint16_t)0x0002)            /* Bit 1 */

#define  TIM_CCMR2_OC3FE                     ((uint16_t)0x0004)            /* Output Compare 3 Fast enable */
#define  TIM_CCMR2_OC3PE                     ((uint16_t)0x0008)            /* Output Compare 3 Preload enable */

#define  TIM_CCMR2_OC3M                      ((uint16_t)0x0070)            /* OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    ((uint16_t)0x0010)            /* Bit 0 */
#define  TIM_CCMR2_OC3M_1                    ((uint16_t)0x0020)            /* Bit 1 */
#define  TIM_CCMR2_OC3M_2                    ((uint16_t)0x0040)            /* Bit 2 */

#define  TIM_CCMR2_OC3CE                     ((uint16_t)0x0080)            /* Output Compare 3 Clear Enable */

#define  TIM_CCMR2_CC4S                      ((uint16_t)0x0300)            /* CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    ((uint16_t)0x0100)            /* Bit 0 */
#define  TIM_CCMR2_CC4S_1                    ((uint16_t)0x0200)            /* Bit 1 */

#define  TIM_CCMR2_OC4FE                     ((uint16_t)0x0400)            /* Output Compare 4 Fast enable */
#define  TIM_CCMR2_OC4PE                     ((uint16_t)0x0800)            /* Output Compare 4 Preload enable */

#define  TIM_CCMR2_OC4M                      ((uint16_t)0x7000)            /* OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    ((uint16_t)0x1000)            /* Bit 0 */
#define  TIM_CCMR2_OC4M_1                    ((uint16_t)0x2000)            /* Bit 1 */
#define  TIM_CCMR2_OC4M_2                    ((uint16_t)0x4000)            /* Bit 2 */

#define  TIM_CCMR2_OC4CE                     ((uint16_t)0x8000)            /* Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR2_IC3PSC                    ((uint16_t)0x000C)            /* IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC3PSC_0                  ((uint16_t)0x0004)            /* Bit 0 */
#define  TIM_CCMR2_IC3PSC_1                  ((uint16_t)0x0008)            /* Bit 1 */

#define  TIM_CCMR2_IC3F                      ((uint16_t)0x00F0)            /* IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3F_0                    ((uint16_t)0x0010)            /* Bit 0 */
#define  TIM_CCMR2_IC3F_1                    ((uint16_t)0x0020)            /* Bit 1 */
#define  TIM_CCMR2_IC3F_2                    ((uint16_t)0x0040)            /* Bit 2 */
#define  TIM_CCMR2_IC3F_3                    ((uint16_t)0x0080)            /* Bit 3 */

#define  TIM_CCMR2_IC4PSC                    ((uint16_t)0x0C00)            /* IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define  TIM_CCMR2_IC4PSC_0                  ((uint16_t)0x0400)            /* Bit 0 */
#define  TIM_CCMR2_IC4PSC_1                  ((uint16_t)0x0800)            /* Bit 1 */

#define  TIM_CCMR2_IC4F                      ((uint16_t)0xF000)            /* IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4F_0                    ((uint16_t)0x1000)            /* Bit 0 */
#define  TIM_CCMR2_IC4F_1                    ((uint16_t)0x2000)            /* Bit 1 */
#define  TIM_CCMR2_IC4F_2                    ((uint16_t)0x4000)            /* Bit 2 */
#define  TIM_CCMR2_IC4F_3                    ((uint16_t)0x8000)            /* Bit 3 */


/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       ((uint16_t)0x0001)            /* Capture/Compare 1 output enable */
#define  TIM_CCER_CC1P                       ((uint16_t)0x0002)            /* Capture/Compare 1 output Polarity */
#define  TIM_CCER_CC1NE                      ((uint16_t)0x0004)            /* Capture/Compare 1 Complementary output enable */
#define  TIM_CCER_CC1NP                      ((uint16_t)0x0008)            /* Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint16_t)0x0010)            /* Capture/Compare 2 output enable */
#define  TIM_CCER_CC2P                       ((uint16_t)0x0020)            /* Capture/Compare 2 output Polarity */
#define  TIM_CCER_CC2NE                      ((uint16_t)0x0040)            /* Capture/Compare 2 Complementary output enable */
#define  TIM_CCER_CC2NP                      ((uint16_t)0x0080)            /* Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint16_t)0x0100)            /* Capture/Compare 3 output enable */
#define  TIM_CCER_CC3P                       ((uint16_t)0x0200)            /* Capture/Compare 3 output Polarity */
#define  TIM_CCER_CC3NE                      ((uint16_t)0x0400)            /* Capture/Compare 3 Complementary output enable */
#define  TIM_CCER_CC3NP                      ((uint16_t)0x0800)            /* Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint16_t)0x1000)            /* Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       ((uint16_t)0x2000)            /* Capture/Compare 4 output Polarity */


/*******************  Bit definition for TIM_CNT register  ********************/
#define  TIM_CNT_CNT                         ((uint16_t)0xFFFF)            /* Counter Value */


/*******************  Bit definition for TIM_PSC register  ********************/
#define  TIM_PSC_PSC                         ((uint16_t)0xFFFF)            /* Prescaler Value */


/*******************  Bit definition for TIM_ARR register  ********************/
#define  TIM_ARR_ARR                         ((uint16_t)0xFFFF)            /* actual auto-reload Value */


/*******************  Bit definition for TIM_RCR register  ********************/
#define  TIM_RCR_REP                         ((uint8_t)0xFF)               /* Repetition Counter Value */


/*******************  Bit definition for TIM_CCR1 register  *******************/
#define  TIM_CCR1_CCR1                       ((uint16_t)0xFFFF)            /* Capture/Compare 1 Value */


/*******************  Bit definition for TIM_CCR2 register  *******************/
#define  TIM_CCR2_CCR2                       ((uint16_t)0xFFFF)            /* Capture/Compare 2 Value */


/*******************  Bit definition for TIM_CCR3 register  *******************/
#define  TIM_CCR3_CCR3                       ((uint16_t)0xFFFF)            /* Capture/Compare 3 Value */


/*******************  Bit definition for TIM_CCR4 register  *******************/
#define  TIM_CCR4_CCR4                       ((uint16_t)0xFFFF)            /* Capture/Compare 4 Value */


/*******************  Bit definition for TIM_BDTR register  *******************/
#define  TIM_BDTR_DTG                        ((uint16_t)0x00FF)            /* DTG[0:7] bits (Dead-Time Generator set-up) */
#define  TIM_BDTR_DTG_0                      ((uint16_t)0x0001)            /* Bit 0 */
#define  TIM_BDTR_DTG_1                      ((uint16_t)0x0002)            /* Bit 1 */
#define  TIM_BDTR_DTG_2                      ((uint16_t)0x0004)            /* Bit 2 */
#define  TIM_BDTR_DTG_3                      ((uint16_t)0x0008)            /* Bit 3 */
#define  TIM_BDTR_DTG_4                      ((uint16_t)0x0010)            /* Bit 4 */
#define  TIM_BDTR_DTG_5                      ((uint16_t)0x0020)            /* Bit 5 */
#define  TIM_BDTR_DTG_6                      ((uint16_t)0x0040)            /* Bit 6 */
#define  TIM_BDTR_DTG_7                      ((uint16_t)0x0080)            /* Bit 7 */

#define  TIM_BDTR_LOCK                       ((uint16_t)0x0300)            /* LOCK[1:0] bits (Lock Configuration) */
#define  TIM_BDTR_LOCK_0                     ((uint16_t)0x0100)            /* Bit 0 */
#define  TIM_BDTR_LOCK_1                     ((uint16_t)0x0200)            /* Bit 1 */

#define  TIM_BDTR_OSSI                       ((uint16_t)0x0400)            /* Off-State Selection for Idle mode */
#define  TIM_BDTR_OSSR                       ((uint16_t)0x0800)            /* Off-State Selection for Run mode */
#define  TIM_BDTR_BKE                        ((uint16_t)0x1000)            /* Break enable */
#define  TIM_BDTR_BKP                        ((uint16_t)0x2000)            /* Break Polarity */
#define  TIM_BDTR_AOE                        ((uint16_t)0x4000)            /* Automatic Output enable */
#define  TIM_BDTR_MOE                        ((uint16_t)0x8000)            /* Main Output enable */


/*******************  Bit definition for TIM_DCR register  ********************/
#define  TIM_DCR_DBA                         ((uint16_t)0x001F)            /* DBA[4:0] bits (DMA Base Address) */
#define  TIM_DCR_DBA_0                       ((uint16_t)0x0001)            /* Bit 0 */
#define  TIM_DCR_DBA_1                       ((uint16_t)0x0002)            /* Bit 1 */
#define  TIM_DCR_DBA_2                       ((uint16_t)0x0004)            /* Bit 2 */
#define  TIM_DCR_DBA_3                       ((uint16_t)0x0008)            /* Bit 3 */
#define  TIM_DCR_DBA_4                       ((uint16_t)0x0010)            /* Bit 4 */

#define  TIM_DCR_DBL                         ((uint16_t)0x1F00)            /* DBL[4:0] bits (DMA Burst Length) */
#define  TIM_DCR_DBL_0                       ((uint16_t)0x0100)            /* Bit 0 */
#define  TIM_DCR_DBL_1                       ((uint16_t)0x0200)            /* Bit 1 */
#define  TIM_DCR_DBL_2                       ((uint16_t)0x0400)            /* Bit 2 */
#define  TIM_DCR_DBL_3                       ((uint16_t)0x0800)            /* Bit 3 */
#define  TIM_DCR_DBL_4                       ((uint16_t)0x1000)            /* Bit 4 */


/*******************  Bit definition for TIM_DMAR register  *******************/
#define  TIM_DMAR_DMAB                       ((uint16_t)0xFFFF)            /* DMA register for burst accesses */


/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface                    */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for I2C_CR1 register  ********************/
#define  I2C_CR1_PE                          ((uint16_t)0x0001)            /* Peripheral Enable */
#define  I2C_CR1_SMBUS                       ((uint16_t)0x0002)            /* SMBus Mode */
#define  I2C_CR1_SMBTYPE                     ((uint16_t)0x0008)            /* SMBus Type */
#define  I2C_CR1_ENARP                       ((uint16_t)0x0010)            /* ARP Enable */
#define  I2C_CR1_ENPEC                       ((uint16_t)0x0020)            /* PEC Enable */
#define  I2C_CR1_ENGC                        ((uint16_t)0x0040)            /* General Call Enable */
#define  I2C_CR1_NOSTRETCH                   ((uint16_t)0x0080)            /* Clock Stretching Disable (Slave mode) */
#define  I2C_CR1_START                       ((uint16_t)0x0100)            /* Start Generation */
#define  I2C_CR1_STOP                        ((uint16_t)0x0200)            /* Stop Generation */
#define  I2C_CR1_ACK                         ((uint16_t)0x0400)            /* Acknowledge Enable */
#define  I2C_CR1_POS                         ((uint16_t)0x0800)            /* Acknowledge/PEC Position (for data reception) */
#define  I2C_CR1_PEC                         ((uint16_t)0x1000)            /* Packet Error Checking */
#define  I2C_CR1_ALERT                       ((uint16_t)0x2000)            /* SMBus Alert */
#define  I2C_CR1_SWRST                       ((uint16_t)0x8000)            /* Software Reset */


/*******************  Bit definition for I2C_CR2 register  ********************/
#define  I2C_CR2_FREQ                        ((uint16_t)0x003F)            /* FREQ[5:0] bits (Peripheral Clock Frequency) */
#define  I2C_CR2_FREQ_0                      ((uint16_t)0x0001)            /* Bit 0 */
#define  I2C_CR2_FREQ_1                      ((uint16_t)0x0002)            /* Bit 1 */
#define  I2C_CR2_FREQ_2                      ((uint16_t)0x0004)            /* Bit 2 */
#define  I2C_CR2_FREQ_3                      ((uint16_t)0x0008)            /* Bit 3 */
#define  I2C_CR2_FREQ_4                      ((uint16_t)0x0010)            /* Bit 4 */
#define  I2C_CR2_FREQ_5                      ((uint16_t)0x0020)            /* Bit 5 */

#define  I2C_CR2_ITERREN                     ((uint16_t)0x0100)            /* Error Interrupt Enable */
#define  I2C_CR2_ITEVTEN                     ((uint16_t)0x0200)            /* Event Interrupt Enable */
#define  I2C_CR2_ITBUFEN                     ((uint16_t)0x0400)            /* Buffer Interrupt Enable */
#define  I2C_CR2_DMAEN                       ((uint16_t)0x0800)            /* DMA Requests Enable */
#define  I2C_CR2_LAST                        ((uint16_t)0x1000)            /* DMA Last Transfer */


/*******************  Bit definition for I2C_OAR1 register  *******************/
#define  I2C_OAR1_ADD1_7                     ((uint16_t)0x00FE)            /* Interface Address */
#define  I2C_OAR1_ADD8_9                     ((uint16_t)0x0300)            /* Interface Address */

#define  I2C_OAR1_ADD0                       ((uint16_t)0x0001)            /* Bit 0 */
#define  I2C_OAR1_ADD1                       ((uint16_t)0x0002)            /* Bit 1 */
#define  I2C_OAR1_ADD2                       ((uint16_t)0x0004)            /* Bit 2 */
#define  I2C_OAR1_ADD3                       ((uint16_t)0x0008)            /* Bit 3 */
#define  I2C_OAR1_ADD4                       ((uint16_t)0x0010)            /* Bit 4 */
#define  I2C_OAR1_ADD5                       ((uint16_t)0x0020)            /* Bit 5 */
#define  I2C_OAR1_ADD6                       ((uint16_t)0x0040)            /* Bit 6 */
#define  I2C_OAR1_ADD7                       ((uint16_t)0x0080)            /* Bit 7 */
#define  I2C_OAR1_ADD8                       ((uint16_t)0x0100)            /* Bit 8 */
#define  I2C_OAR1_ADD9                       ((uint16_t)0x0200)            /* Bit 9 */

#define  I2C_OAR1_ADDMODE                    ((uint16_t)0x8000)            /* Addressing Mode (Slave mode) */


/*******************  Bit definition for I2C_OAR2 register  *******************/
#define  I2C_OAR2_ENDUAL                     ((uint8_t)0x01)               /* Dual addressing mode enable */
#define  I2C_OAR2_ADD2                       ((uint8_t)0xFE)               /* Interface address */


/********************  Bit definition for I2C_DR register  ********************/
#define  I2C_DR_DR                           ((uint8_t)0xFF)               /* 8-bit Data Register */


/*******************  Bit definition for I2C_SR1 register  ********************/
#define  I2C_SR1_SB                          ((uint16_t)0x0001)            /* Start Bit (Master mode) */
#define  I2C_SR1_ADDR                        ((uint16_t)0x0002)            /* Address sent (master mode)/matched (slave mode) */
#define  I2C_SR1_BTF                         ((uint16_t)0x0004)            /* Byte Transfer Finished */
#define  I2C_SR1_ADD10                       ((uint16_t)0x0008)            /* 10-bit header sent (Master mode) */
#define  I2C_SR1_STOPF                       ((uint16_t)0x0010)            /* Stop detection (Slave mode) */
#define  I2C_SR1_RXNE                        ((uint16_t)0x0040)            /* Data Register not Empty (receivers) */
#define  I2C_SR1_TXE                         ((uint16_t)0x0080)            /* Data Register Empty (transmitters) */
#define  I2C_SR1_BERR                        ((uint16_t)0x0100)            /* Bus Error */
#define  I2C_SR1_ARLO                        ((uint16_t)0x0200)            /* Arbitration Lost (master mode) */
#define  I2C_SR1_AF                          ((uint16_t)0x0400)            /* Acknowledge Failure */
#define  I2C_SR1_OVR                         ((uint16_t)0x0800)            /* Overrun/Underrun */
#define  I2C_SR1_PECERR                      ((uint16_t)0x1000)            /* PEC Error in reception */
#define  I2C_SR1_TIMEOUT                     ((uint16_t)0x4000)            /* Timeout or Tlow Error */
#define  I2C_SR1_SMBALERT                    ((uint16_t)0x8000)            /* SMBus Alert */


/*******************  Bit definition for I2C_SR2 register  ********************/
#define  I2C_SR2_MSL                         ((uint16_t)0x0001)            /* Master/Slave */
#define  I2C_SR2_BUSY                        ((uint16_t)0x0002)            /* Bus Busy */
#define  I2C_SR2_TRA                         ((uint16_t)0x0004)            /* Transmitter/Receiver */
#define  I2C_SR2_GENCALL                     ((uint16_t)0x0010)            /* General Call Address (Slave mode) */
#define  I2C_SR2_SMBDEFAULT                  ((uint16_t)0x0020)            /* SMBus Device Default Address (Slave mode) */
#define  I2C_SR2_SMBHOST                     ((uint16_t)0x0040)            /* SMBus Host Header (Slave mode) */
#define  I2C_SR2_DUALF                       ((uint16_t)0x0080)            /* Dual Flag (Slave mode) */
#define  I2C_SR2_PEC                         ((uint16_t)0xFF00)            /* Packet Error Checking Register */


/*******************  Bit definition for I2C_CCR register  ********************/
#define  I2C_CCR_CCR                         ((uint16_t)0x0FFF)            /* Clock Control Register in Fast/Standard mode (Master mode) */
#define  I2C_CCR_DUTY                        ((uint16_t)0x4000)            /* Fast Mode Duty Cycle */
#define  I2C_CCR_FS                          ((uint16_t)0x8000)            /* I2C Master Mode Selection */


/******************  Bit definition for I2C_TRISE register  *******************/
#define  I2C_TRISE_TRISE                     ((uint8_t)0x3F)               /* Maximum Rise Time in Fast/Standard mode (Master mode) */



/* Exported functions ------------------------------------------------------- */


#endif 

