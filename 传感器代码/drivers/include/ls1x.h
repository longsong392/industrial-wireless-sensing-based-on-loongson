#ifndef _LS1X_H_
#define _LS1X_H_

#include "test.h"

/*****************************************************/
/******************  NORMAL ADDRESS SPACE  ******************/
#if defined(LS1C102)
#define UNCACHED_MEMORY_ADDR 	0xa0000000
#define UNCACHED_TO_PHYS(x)     ((x) & 0x1fffffff)
#define PHYS_TO_UNCACHED(x)     ((x) | UNCACHED_MEMORY_ADDR)
#define RAM0_BASE			    PHYS_TO_UNCACHED(0x00000000)            //iram
#define RAM1_BASE			    PHYS_TO_UNCACHED(0x00001000)            //dram
#define SPI_MEM_BASE			PHYS_TO_UNCACHED(0x1e000000)            //spi_flash
#define FLASH_MEM_BASE   		PHYS_TO_UNCACHED(0x1f000000)            //on-chip flash
#define FLASH_BASE		        PHYS_TO_UNCACHED(0x1fe60000)            //flash regs
#define SPI_BASE	        	PHYS_TO_UNCACHED(0x1fe70000)            //spi regs
#define UART0_BASEADDR  		PHYS_TO_UNCACHED(0x1fe80000)            //uart0
#define UART1_BASEADDR			PHYS_TO_UNCACHED(0x1fe88000)            //uart1
#define UART2_BASEADDR			PHYS_TO_UNCACHED(0x1fe8c000)            //uart2
#define I2C_BASE			    PHYS_TO_UNCACHED(0x1fe90000)            //i2c
#define INTC_BASE			    PHYS_TO_UNCACHED(0x1fea0000)            //Interrupt_Regs_Baseadd
#define PMU_BASE			    PHYS_TO_UNCACHED(0x1feb0000)            //PMU
#define TSENSOR_BASE		    PHYS_TO_UNCACHED(0x1feb4000)            //tsensor
#define RTC_BASE			    PHYS_TO_UNCACHED(0x1feb8000)            //rtc
#define DMA_BASE		        PHYS_TO_UNCACHED(0x1fec0000)            //DMA
#define VPWM_BASE		        PHYS_TO_UNCACHED(0x1fec0020)            //vpwm
#define TIMER_BASE		        PHYS_TO_UNCACHED(0x1fed0000)            //timer

#define ATE_Freq			   (*(volatile int *)0xbf0201b0)


/******************  PMU REGS  ******************/
typedef struct
{
    volatile uint32_t CHIPCTRL;
    volatile uint32_t CMDSTS;
    volatile uint32_t COUNT;
    volatile uint32_t COMPARE;
    volatile uint32_t IOSEL0;
    volatile uint32_t IOSEL1;
    volatile uint32_t IOSEL2;
    volatile uint32_t IOSEL3;
    volatile uint32_t EXTINT_EN;
    volatile uint32_t EXINT_POL;
    volatile uint32_t EXINT_EDGE;
    volatile uint32_t EXINT_SRC;
    volatile uint32_t WDT_CFG;
    volatile uint32_t WDT_FEED;
    volatile uint32_t POWER_CFG;
    volatile uint32_t CMDW;
    volatile uint32_t GPIOA_OE;
    volatile uint32_t GPIOA_O;
    volatile uint32_t GPIOA_I;
    volatile uint32_t RESERVED0;
    volatile uint32_t GPIOB_OE;
    volatile uint32_t GPIOB_O;
    volatile uint32_t GPIOB_I;
    volatile uint32_t RESERVED1;
    volatile uint32_t PULSE0;
    volatile uint32_t PULSE1;
    volatile uint32_t USER_DAT0;
    volatile uint32_t ADC_CTRL;
    volatile uint32_t ADC_DAT;
} PMU_TypeDef;

#define PMU_ChipCtrl               0x00
#define PMU_Command                0x04
#define PMU_Count                  0x08
#define PMU_Compare                0x0c
#define PMU_WdtCfg                 0x30
#define PMU_WdtFeed                0x34
#define PMU_PowerCfg               0x38
#define PMU_CommandW               0x3c
#define PMU_GPIOAOE_OFFSET         0x40
#define PMU_GPIOAO_OFFSET          0x44
#define PMU_GPIOAI_OFFSET          0x48
#define PMU_GPIOBOE_OFFSET         0x50
#define PMU_GPIOBO_OFFSET          0x54
#define PMU_GPIOBI_OFFSET          0x58
#define PMU_PULSE0_OFFSET          0x60
#define PMU_PULSE1_OFFSET          0x64
#define PMU_ADCCTRL_OFFSET         0x6c
#define PMU_ADCDAT_OFFSET          0x70
//PMU access by 8-bit
#define PMU_GPIOBIT_OFFSET         0x80
#define PMU_SoftRst		           0x10
#define PMU_ClkOff		           0x14
#define PMU_SrProt		           0x18
#define PMU_UserDat1		       0x64
#define PMU_UserDat2		       0x68
#define PMU_UserDat3		       0x6c
/******************  GPIO REGS  ******************/
typedef struct
{
    volatile uint32_t IOSEL;
} AFIO_TypeDef;

typedef struct
{
    volatile uint32_t GPIO_OE;                    /* GPIO输出使能寄存器 */
    volatile uint32_t GPIO_O;                             /* GPIO输出电平寄存器 */
    volatile uint32_t GPIO_I;                             /* GPIO输入电平寄存器 */
} GPIO_TypeDef;

#define PMU_UserDat0               0x68
#define AFIO_Sela  		           0x10
#define AFIO_Selb                  0x14
#define AFIO_Selc                  0x18
#define AFIO_Seld                  0x1c
#define EXTI_En 	               0x20
#define EXTI_Pol                   0x24
#define EXTI_Edge                  0x28
#define EXTI_Src                   0x2c

#define PMU_CHIPCTRL  	       *(volatile unsigned int *)(PMU_BASE+0x00)    //全局配置
#define PMU_CMDSTS  	       *(volatile unsigned int *)(PMU_BASE+0x04)    //命令与状态
#define PMU_COUNT   	       *(volatile unsigned int *)(PMU_BASE+0x08)    //时间计数器
#define PMU_COMPARE  	       *(volatile unsigned int *)(PMU_BASE+0x0c)    //唤醒时间配置
#define AFIO_PORTA	           *(volatile unsigned int *)(PMU_BASE+0x10)    //IO复用选择0
#define AFIO_PORTB	           *(volatile unsigned int *)(PMU_BASE+0x14)    //IO复用选择1
#define AFIO_PORTC	           *(volatile unsigned int *)(PMU_BASE+0x18)    //IO复用选择2
#define AFIO_PORTD	           *(volatile unsigned int *)(PMU_BASE+0x1c)    //IO复用选择3
#define EXTI_EN 	           *(volatile unsigned int *)(PMU_BASE+0x20)    //外部中断使能
#define EXTI_POL               *(volatile unsigned int *)(PMU_BASE+0x24)    //外部中断极性
#define EXTI_EDGE              *(volatile unsigned int *)(PMU_BASE+0x28)    //外部中断边沿
#define EXTI_SRC               *(volatile unsigned int *)(PMU_BASE+0x2c)    //外部中断状态
#define PMU_WDTCFG             *(volatile unsigned int *)(PMU_BASE+0x30)    //看门狗配置
#define PMU_WDTFEED            *(volatile unsigned int *)(PMU_BASE+0x34)    //看门狗重置
#define PMU_POWERCFG 	       *(volatile unsigned int *)(PMU_BASE+0x38)    //电源配置
#define PMU_GPIOA_OE 	       *(volatile unsigned int *)(PMU_BASE+0x40)    //GPIOA输出使能
#define PMU_GPIOA_O	           *(volatile unsigned int *)(PMU_BASE+0x44)    //GPIOA输出电平
#define PMU_GPIOA_I	           *(volatile unsigned int *)(PMU_BASE+0x48)    //GPIOA输入电平
#define PMU_GPIOB_OE 	       *(volatile unsigned int *)(PMU_BASE+0x50)    //GPIOB输出使能
#define PMU_GPIOB_O	           *(volatile unsigned int *)(PMU_BASE+0x54)    //GPIOB输出电平
#define PMU_GPIOB_I	           *(volatile unsigned int *)(PMU_BASE+0x58)    //GPIOB输入电平
#define PMU_Pulse0 	           *(volatile unsigned int *)(PMU_BASE+0x60)    //脉冲输出配置0
#define PMU_Pulse1 	           *(volatile unsigned int *)(PMU_BASE+0x64)    //脉冲输出配置1
#define PMU_UserDAT            *(volatile unsigned int *)(PMU_BASE+0x68)    //用户数据
#define PMU_AdcCtrl 	       *(volatile unsigned int *)(PMU_BASE+0x6c)    //ADC控制
#define PMU_AdcDat  	       *(volatile unsigned int *)(PMU_BASE+0x70)    //ADC数据
#define PMU_GPIOBit(i) 	       *(volatile unsigned char *)(PMU_BASE+0x80+i) //GPIO位访问
#define PMU_CMDW               *(volatile unsigned int *) (PMU_BASE+0x3c)   //Command写端口



#define AFIOA_BASE	(PMU_BASE + AFIO_Sela)
#define AFIOB_BASE	(PMU_BASE + AFIO_Selb)
#define AFIOC_BASE	(PMU_BASE + AFIO_Selc)
#define AFIOD_BASE	(PMU_BASE + AFIO_Seld)
#define GPIOA_BASE	(PMU_BASE + 0x40)
#define GPIOB_BASE	(PMU_BASE + 0x50)

#define PMU	        ((PMU_TypeDef *) PMU_BASE)
#define AFIOA		((AFIO_TypeDef *) AFIOA_BASE)
#define AFIOB		((AFIO_TypeDef *) AFIOB_BASE)
#define AFIOC		((AFIO_TypeDef *) AFIOC_BASE)
#define AFIOD		((AFIO_TypeDef *) AFIOD_BASE)
#define GPIOA		((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB		((GPIO_TypeDef *) GPIOB_BASE)

/******************  INTC REGS  ******************/
typedef struct
{
    volatile uint8_t INTC_EN;					/* 中断使能寄存器 */
    volatile uint8_t INTC_EDGE;				/* 中断边沿寄存器 */
    volatile uint8_t INTC_POL;					/* 中断极性寄存器 */
    volatile uint8_t INTC_CLR;					/* 中断清除寄存器 */
    volatile uint8_t INTC_SET;					/* 中断置位寄存器 */
    volatile uint8_t INTC_OUT;					/* 中断输出寄存器 */
    volatile uint8_t SRPRPT;					/* 运行状态及保护寄存器 */
} INT_TypeDef;

#define INTC_EN_OFFSET             0x0
#define INTC_EDGE_OFFSET           0x1
#define INTC_POL_OFFSET            0x2
#define INTC_CLR_OFFSET            0x3
#define INTC_SET_OFFSET            0x4
#define INTC_OUT_OFFSET            0x5
#define INTC_SRPROT_OFFSET         0x6
#define INTC_CKGATE_OFFSET         0x7
#define INT_EN	               *(volatile unsigned char *)(INTC_BASE+0x00)     //中断使能寄存器
#define INT_EGDE               *(volatile unsigned char *)(INTC_BASE+0x01)     //中断边沿寄存器
#define INT_POL                *(volatile unsigned char *)(INTC_BASE+0x02)     //中断极性寄存器
#define INT_CLR                *(volatile unsigned char *)(INTC_BASE+0x03)     //中断清除寄存器
#define INT_SET                *(volatile unsigned char *)(INTC_BASE+0x04)     //中断置位寄存器
#define INT_OUT                *(volatile unsigned char *)(INTC_BASE+0x05)     //中断输出寄存器
#define INT_SRPROT             *(volatile unsigned char *)(INTC_BASE+0x06)     //运行状态及保护寄存

#define SRPROT	INT_SRPROT
#define INT					((INT_TypeDef *) INTC_BASE)


/******************  UART REGS  ******************/
typedef struct
{
    volatile uint8_t DAT_DL_L ;			/* 发送数据寄存器 或 分频值低字节寄存器 */
    volatile uint8_t IER_DL_H;			/* 中断使能寄存器 或 分频值高字节寄存器 */
    volatile uint8_t IIR_FCR_DL_D;		/* 中断状态寄存（只读） FIFO控制寄存器 （只写）  或   分频值小数寄存器  */
    volatile uint8_t LCR;					/* 线路控制寄存器  */
    volatile uint8_t SAMPLE_CTRL;				/* bit窗口划分和采样控制寄存器  */
    volatile uint8_t LSR;					/* 线路状态寄存器  */
    volatile uint8_t TF_CNT;				/* 发送队列数据存量  */
    volatile uint8_t STATUS;				/* 状态寄存器  */

} UART_TypeDef;
#define UART0_TxData           *(volatile unsigned char *)(UART0_BASEADDR+0x00)   //发送数据寄存器
#define UART0_RxData           *(volatile unsigned char *)(UART0_BASEADDR+0x00)   //接收数据寄存器
#define UART0_IER              *(volatile unsigned char *)(UART0_BASEADDR+0x01)   //中断使能寄存器
#define UART0_IIR   	       *(volatile unsigned char *)(UART0_BASEADDR+0x02)   //中断状态寄存
#define UART0_FCR   	       *(volatile unsigned char *)(UART0_BASEADDR+0x02)	  //FIFO控制寄存器
#define UART0_LCR   	       *(volatile unsigned char *)(UART0_BASEADDR+0x03)   //线路控制寄存器
#define UART0_SAMPLE_CTRL      *(volatile unsigned char *)(UART0_BASEADDR+0x04)   //bit窗口划分和采样控制寄存器
#define UART0_LSR  	           *(volatile unsigned char *)(UART0_BASEADDR+0x05)   //线路状态寄存器
#define UART0_TF_CNT   	       *(volatile unsigned char *)(UART0_BASEADDR+0x06)   //发送队列数据存量
#define UART0_STATUS   	       *(volatile unsigned char *)(UART0_BASEADDR+0x07)   //状态寄存器

#define UART0_DL_L   	       *(volatile unsigned char *)(UART0_BASEADDR+0x00)   //分频值低字节寄存器
#define UART0_DL_H   	       *(volatile unsigned char *)(UART0_BASEADDR+0x01)   //分频值高字节寄存器
#define UART0_DL_D   	       *(volatile unsigned char *)(UART0_BASEADDR+0x02)   //分频值小数寄存器

/*****************************UART1 REGS********************************/
#define UART1_TxData           *(volatile unsigned char *)(UART1_BASEADDR+0x00)   //发送数据寄存器
#define UART1_RxData           *(volatile unsigned char *)(UART1_BASEADDR+0x00)   //接收数据寄存器
#define UART1_IER              *(volatile unsigned char *)(UART1_BASEADDR+0x01)   //中断使能寄存器
#define UART1_IIR   	       *(volatile unsigned char *)(UART1_BASEADDR+0x02)   //中断状态寄存
#define UART1_FCR   	       *(volatile unsigned char *)(UART1_BASEADDR+0x02)	  //FIFO控制寄存器
#define UART1_LCR   	       *(volatile unsigned char *)(UART1_BASEADDR+0x03)   //线路控制寄存器
#define UART1_SAMPLE_CTRL      *(volatile unsigned char *)(UART1_BASEADDR+0x04)   //bit窗口划分和采样控制寄存器
#define UART1_LSR  	           *(volatile unsigned char *)(UART1_BASEADDR+0x05)   //线路状态寄存器
#define UART1_TF_CNT   	       *(volatile unsigned char *)(UART1_BASEADDR+0x06)   //发送队列数据存量
#define UART1_STATUS   	       *(volatile unsigned char *)(UART1_BASEADDR+0x07)   //状态寄存器

#define UART1_DL_L   	       *(volatile unsigned char *)(UART1_BASEADDR+0x00)   //分频值低字节寄存器
#define UART1_DL_H   	       *(volatile unsigned char *)(UART1_BASEADDR+0x01)   //分频值高字节寄存器
#define UART1_DL_D   	       *(volatile unsigned char *)(UART1_BASEADDR+0x02)   //分频值小数寄存器

/*****************************UART2 REGS********************************/
#define UART2_TxData           *(volatile unsigned char *)(UART2_BASEADDR+0x00)   //发送数据寄存器
#define UART2_RxData           *(volatile unsigned char *)(UART2_BASEADDR+0x00)   //接收数据寄存器
#define UART2_IER              *(volatile unsigned char *)(UART2_BASEADDR+0x01)   //中断使能寄存器
#define UART2_IIR   	       *(volatile unsigned char *)(UART2_BASEADDR+0x02)   //中断状态寄存
#define UART2_FCR   	       *(volatile unsigned char *)(UART2_BASEADDR+0x02)	//FIFO控制寄存器
#define UART2_LCR   	       *(volatile unsigned char *)(UART2_BASEADDR+0x03)   //线路控制寄存器
#define UART2_SAMPLE_CTRL      *(volatile unsigned char *)(UART2_BASEADDR+0x04)   //bit窗口划分和采样控制寄存器
#define UART2_LSR  	           *(volatile unsigned char *)(UART2_BASEADDR+0x05)   //线路状态寄存器
#define UART2_TF_CNT   	       *(volatile unsigned char *)(UART2_BASEADDR+0x06)   //发送队列数据存量
#define UART2_STATUS   	       *(volatile unsigned char *)(UART2_BASEADDR+0x07)   //状态寄存器

#define UART2_DL_L       	   *(volatile unsigned char *)(UART2_BASEADDR+0x00)   //分频值低字节寄存器
#define UART2_DL_H      	   *(volatile unsigned char *)(UART2_BASEADDR+0x01)   //分频值高字节寄存器
#define UART2_DL_D      	   *(volatile unsigned char *)(UART2_BASEADDR+0x02)   //分频值小数寄存器


#define UART0              ((UART_TypeDef *) UART0_BASEADDR)
#define UART1              ((UART_TypeDef *) UART1_BASEADDR)
#define UART2              ((UART_TypeDef *) UART2_BASEADDR)
#define UART_PRINT				    UART0
//#define UART_PRINT				UART1
//#define UART_PRINT				UART2


/******************  EXTI REGS  ******************/
typedef struct
{
    uint32_t EXINT_EN;
    uint32_t EXINT_POL;
    uint32_t EXINT_EDGE;
    uint32_t EXINT_SRC;
} EXTI_TypeDef;
#define PMU_Exti                    (0x20)
#define EXTI_BASE             (PMU_BASE + PMU_Exti)
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)

/******************  SPI REGS  ******************/

typedef struct
{
    volatile uint8_t SPCR;			/* 控制寄存器 */
    volatile uint8_t SPSR;			/* 状态寄存器 */
    volatile uint8_t DATA;			/* 数据寄存器 */
    volatile uint8_t SPER;			/* 外部寄存器 */
    volatile uint8_t PARAM;			/* 参数控制寄存器 */
    volatile uint8_t SOFTCS;			/* 片选控制寄存器 */
    volatile uint8_t TIMING;			/* 时序控制寄存器 */
} SPI_TypeDef;
#define SPI                             ((SPI_TypeDef *) SPI_BASE)
#define SPI_SPCR		                 *(volatile unsigned char *)(SPI_BASE+0x00) //控制寄存器
#define SPI_SPSR		                 *(volatile unsigned char *)(SPI_BASE+0x01) //状态寄存器
#define SPI_TxFIFO		                 *(volatile unsigned char *)(SPI_BASE+0x02) //数据寄存器
#define SPI_RxFIFO		                 *(volatile unsigned char *)(SPI_BASE+0x02) //数据寄存器
#define SPI_SPER		                 *(volatile unsigned char *)(SPI_BASE+0x03) //外部寄存器
#define SPI_SFC_PARAM	                 *(volatile unsigned char *)(SPI_BASE+0x04) //参数控制寄存器
#define SPI_SFC_SOFTCS	                 *(volatile unsigned char *)(SPI_BASE+0x05) //片选控制寄存器
#define SPI_SFC_TIMING	                 *(volatile unsigned char *)(SPI_BASE+0x06) //时序控制寄存器


/*******************************TIMER REGS*********************************/
#define TIMER_CFG  	 	       *(volatile unsigned int *)(TIMER_BASE+0x00)   //配置寄存器
#define TIMER_CNT		       *(volatile unsigned int *)(TIMER_BASE+0x04)   //计数值寄存器
#define TIMER_CMP		       *(volatile unsigned int *)(TIMER_BASE+0x08)   //比较值寄存器
#define TIMER_STP              *(volatile unsigned int *)(TIMER_BASE+0x0C)   //步进值寄存器

/*******************************I2C REGS********************************/
#define I2C_PRERL	           *(volatile unsigned char *)(I2C_BASE+0x00)     //分频值低字节寄存器
#define I2C_PRERH 	           *(volatile unsigned char *)(I2C_BASE+0x01)     //分频值高字节寄存器
#define I2C_CTR   	           *(volatile unsigned char *)(I2C_BASE+0x02)     //控制寄存器
#define I2C_TXR   	           *(volatile unsigned char *)(I2C_BASE+0x03)     //数据寄存器
#define I2C_RXR   	           *(volatile unsigned char *)(I2C_BASE+0x03)     //数据寄存器
#define I2C_CR		           *(volatile unsigned char *)(I2C_BASE+0x04)     //命令寄存器
#define I2C_SR		           *(volatile unsigned char *)(I2C_BASE+0x04)     //状态寄存器
#define I2C_BLTOP		       *(volatile unsigned char *)(I2C_BASE+0x05)     //总线死锁时间寄存器
#define I2C_SADDR	           *(volatile unsigned char *)(I2C_BASE+0x07)     //从模式地址寄存器

/*****************************DMA REGS**********************************/
#define DMA_SOURCE             *(volatile unsigned int *)(DMA_BASE+0x00)    //DMA命令源地址写端口
#define DMA_COUNT              *(volatile unsigned int *)(DMA_BASE+0x04)    //DMA命令数据长度读写端口
#define DMA_CMD_STATUS         *(volatile unsigned int *)(DMA_BASE+0x08)    //命令和状态寄存器
#define DMA_INT_STATUS         *(volatile unsigned int *)(DMA_BASE+0x0c)    //中断和状态寄存器
#define DMA_SOURCE0   	       *(volatile unsigned int *)(DMA_BASE+0x10)    //命令队列项0的源地址参数
#define DMA_SOURCE1  	       *(volatile unsigned int *)(DMA_BASE+0x14)    //命令队列项1的源地址参数
#define DMA_COUNT0             *(volatile unsigned int *)(DMA_BASE+0x18)    //命令队列项0的DMA长度参数
#define DMA_COUNT1             *(volatile unsigned int *)(DMA_BASE+0x1c)    //命令队列项1的DMA长度参数

/*****************************VPWM REGS**********************************/
#define VPWM_CFG               *(volatile unsigned int *)(VPWM_BASE+0x00)   //算法配置
#define VPWM_WPORT_STATUS      *(volatile unsigned int *)(VPWM_BASE+0x08)   //数据写端口状态
#define VPWM_WPORT             *(volatile unsigned int *)(VPWM_BASE+0x0c)   //数据写端口

/*****************************TSENSOR REGS**********************************/
#define TS_CTRL                *(volatile unsigned int *)(TSENSOR_BASE+0x00)     //控制寄存器
#define TS_STAT                *(volatile unsigned int *)(TSENSOR_BASE+0x04)     //状态寄存器
#define TS_OSCTH       	       *(volatile unsigned int *)(TSENSOR_BASE+0x08)     //环振/阈值寄存器
#define TS_POLLTIM             *(volatile unsigned int *)(TSENSOR_BASE+0x0c)     //扫描时序寄存器
#define TS_CHNATTR(a)          *(volatile unsigned int *)(TSENSOR_BASE+0x40+a*4) //通道特征寄存器x12
#define TS_CNTRES(a)           *(volatile unsigned int *)(TSENSOR_BASE+0x80+a*4) //计数结果寄存器x12

/*****************************FLASH REGS********************************/
#define GPIO_LOW    0  //GPIO低
#define GPIO_HIGH   1 //GPIO高

#define RTC_FREQ   	           *(volatile unsigned int *)(RTC_BASE+0x00)    //分频值寄存器
#define RTC_CFG   	           *(volatile unsigned int *)(RTC_BASE+0x04)    //配置寄存器
#define RTC_RTC0   	           *(volatile unsigned int *)(RTC_BASE+0x08)    //时间值寄存器0
#define RTC_RTC1   	           *(volatile unsigned int *)(RTC_BASE+0x0c)    //时间值寄存器1

typedef struct
{
    INT32U	c32k_trim	:4;
    INT32U	c32k_speed	:1;
    INT32U	c32k_sel	:1;
    INT32U	osc8m_en	:1;
    INT32U	c8m_sel		:1;
    INT32U	clkup_dly	:2;
    INT32U	input_hold	:1;
    INT32U	fast_ram	:1;
    INT32U	tsensor_off	:1;
    INT32U	rtc_off		:1;
    INT32U	uart2_off	:1;
    INT32U	dram_pd		:1;
    INT32U	adc_on		:1;
    INT32U	adci0_ien	:1;
    INT32U	adci_pd		:1;
    INT32U	adci_pu		:1;
    INT32U	adc_en		:4;
    INT32U	batdet_sel	:2;
    INT32U	spi_start	:1;
    INT32U	compact_mem	:1;
    INT32U	soft_flag	:4;
} PMU_ChipCtrlStruct;

#define spPMU_ChipCtrl		((volatile PMU_ChipCtrlStruct *)(PMU_BASE))
#define WriteKeyReg(LeftData,ReghtData)\
    {\
        unsigned char GlobalIntFlag = IsGlobalIntOpen();\
        DisableInt();\
        LeftData     = ReghtData;\
        if(GlobalIntFlag){\
            EnableInt();\
        }\
    }


#elif defined(LS1C103)
#define UNCACHED_MEMORY_ADDR 0x00000000
#define PHYS_TO_UNCACHED(x) ((x) | UNCACHED_MEMORY_ADDR)
#define RAM0_BASE	    0x10000000
#define RAM1_BASE           0x10001000
#define SPI_MEM_BASE        0x20000000
#define FLASH_MEM_BASE      0x18000000
#define ATIM_BASE           PHYS_TO_UNCACHED(0x00001000)
#define GTIM_BASE           PHYS_TO_UNCACHED(0x00002000)
#define BTIM_BASE           PHYS_TO_UNCACHED(0x00003000)
#define ADC_BASE            PHYS_TO_UNCACHED(0x00004000)
#define DMA_BASE            PHYS_TO_UNCACHED(0x00005000)
#define CRC_BASE            PHYS_TO_UNCACHED(0x00006000)
#define FLASH_BASE          PHYS_TO_UNCACHED(0x00007000)
#define SPI_BASE            PHYS_TO_UNCACHED(0x00008000)
#define UART0_BASEADDR      PHYS_TO_UNCACHED(0x00009000)
#define UART1_BASEADDR      PHYS_TO_UNCACHED(0x00009100)
#define I2C_BASE            PHYS_TO_UNCACHED(0x0000a000)
#define INTC_BASE           PHYS_TO_UNCACHED(0x0000b000)
#define PMU_BASE	    PHYS_TO_UNCACHED(0x0000c000)
#define RTC_BASE	    PHYS_TO_UNCACHED(0x0000c800)
#define IOCFG_BASE	    PHYS_TO_UNCACHED(0x0000d000)

#define ATE_Freq			   (*(volatile int *)0x180101b0 / 4 )

typedef struct
{
    volatile uint32_t CHIPCTRL;
    volatile uint32_t CMDSTS;
    volatile uint32_t COUNT;
    volatile uint32_t COMPARE;
    volatile uint32_t RST_CTRL;
    volatile uint32_t CLK_CTRL;
    volatile uint32_t SR_PROT;
    volatile uint32_t RESERVED0;
    volatile uint32_t RESERVED1;
    volatile uint32_t RESERVED2;
    volatile uint32_t RESERVED3;
    volatile uint32_t RESERVED4;
    volatile uint32_t WDT_CFG;
    volatile uint32_t WDT_FEED;
    volatile uint32_t POWER_CFG;
    volatile uint32_t CMDW;
    volatile uint32_t RESERVED5;
    volatile uint32_t RESERVED6;
    volatile uint32_t RESERVED7;
    volatile uint32_t RESERVED8;
    volatile uint32_t RESERVED9;
    volatile uint32_t RESERVED10;
    volatile uint32_t RESERVED11;
    volatile uint32_t RESERVED12;
    volatile uint32_t USER_DAT0;
    volatile uint32_t USER_DAT1;
    volatile uint32_t USER_DAT2;
    volatile uint32_t USER_DAT3;
} PMU_TypeDef;
#define PMU_ChipCtrl               0x00
#define PMU_Command                0x04
#define PMU_Count                  0x08
#define PMU_Compare                0x0c
#define PMU_WdtCfg                 0x30
#define PMU_WdtFeed                0x34
#define PMU_PowerCfg               0x38
#define PMU_CommandW               0x3c
#define PMU_GPIOAOE_OFFSET         0x40
#define PMU_GPIOAO_OFFSET          0x44
#define PMU_GPIOAI_OFFSET          0x48
#define PMU_GPIOBOE_OFFSET         0x50
#define PMU_GPIOBO_OFFSET          0x54
#define PMU_GPIOBI_OFFSET          0x58
#define PMU_PULSE0_OFFSET          0x60
#define PMU_PULSE1_OFFSET          0x64
#define PMU_ADCCTRL_OFFSET         0x6c
#define PMU_ADCDAT_OFFSET          0x70
//PMU access by 8-bit
#define PMU_GPIOBIT_OFFSET         0x80
#define PMU_SoftRst		    0x10
#define PMU_ClkOff		    0x14
#define PMU_SrProt		    0x18
#define PMU_UserDat1		    0x64
#define PMU_UserDat2		    0x68
#define PMU_UserDat3		    0x6c
#define PMU_UserDat0		    0x60
#define PMU_CHIPCTRL  	  *(volatile unsigned int*)(PMU_BASE+PMU_ChipCtrl)
#define PMU_CMDSTS        *(unsigned volatile int*)(PMU_BASE+PMU_Command)
#define PMU_COUNT         *(unsigned volatile int*)(PMU_BASE+PMU_Count)
#define PMU_COMPARE       *(unsigned volatile int*)(PMU_BASE+PMU_Compare)
//pmu
#define PMU_SOFTRST       *(unsigned volatile int*)(PMU_BASE+PMU_SoftRst)
#define PMU_SRPROT        *(volatile unsigned char *)(PMU_BASE+PMU_SrProt)
#define PMU_CLKOFF        *(unsigned volatile int*)(PMU_BASE+PMU_ClkOff)
#define PMU_WDTCFG        *(unsigned volatile int*)(PMU_BASE+PMU_WdtCfg)
#define PMU_WDTFEED       *(unsigned volatile int*)(PMU_BASE+PMU_WdtFeed)
#define PMU_CMDW       	  *(unsigned volatile int*)(PMU_BASE+PMU_CommandW)

#define SRPROT	PMU_SRPROT
#define PMU					((PMU_TypeDef *) PMU_BASE)

typedef struct
{
    volatile uint32_t INTC_EN;					/* 中断使能寄存器 */
    volatile uint32_t RESERVED0;
    volatile uint32_t INTC_OUT;
} INT_TypeDef;
#define INT_EN  *(volatile unsigned char*)(INTC_BASE + 0x0)
#define INT_OUT *(volatile unsigned char*)(INTC_BASE + 0x8)
#define INT					((INT_TypeDef *) INTC_BASE)


typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t IOSEL;
} AFIO_TypeDef;


#define AFIO_REG_BASE       (IOCFG_BASE + 0x00000000)
#define EXTI_REG_BASE       (IOCFG_BASE + 0x00000100)
#define GPIOA_REG_BASE      (IOCFG_BASE + 0x00000200)
#define GPIOB_REG_BASE      (IOCFG_BASE + 0x00000300)

#define AFIO_Sela      0x00
#define AFIO_Selb      0x04

#define AFIO_PORTA      *(unsigned volatile int*)(AFIO_REG_BASE+AFIO_Sela)
#define AFIO_PORTB      *(unsigned volatile int*)(AFIO_REG_BASE+AFIO_Selb)


#define EXTI_En            0x00
#define EXTI_Pol           0x04
#define EXTI_Edge          0x08
#define EXTI_Src           0x0c

#define EXTI_EN      *(unsigned volatile int*)(EXTI_REG_BASE+EXTI_En  )
#define EXTI_POL     *(unsigned volatile int*)(EXTI_REG_BASE+EXTI_Pol )
#define EXTI_EDGE    *(unsigned volatile int*)(EXTI_REG_BASE+EXTI_Edge)
#define EXTI_SRC     *(unsigned volatile int*)(EXTI_REG_BASE+EXTI_Src )


#define GPIO_CRL              0x00
#define GPIO_CRH              0x04
#define GPIOA_BASE            (GPIOA_REG_BASE)
#define GPIOB_BASE            (GPIOB_REG_BASE)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)



#define AFIOBASE	(IOCFG_BASE)
#define AFIOA_BASE	(AFIOBASE + AFIO_Sela)
#define AFIOB_BASE	(AFIOBASE + AFIO_Selb)




#define AFIOA		((AFIO_TypeDef *) AFIOA_BASE)
#define AFIOB		((AFIO_TypeDef *) AFIOB_BASE)


typedef struct
{
    volatile uint32_t DAT_DL_L ;			/* 发送数据寄存器 或 分频值低字节寄存器 */
    volatile uint32_t IER_DL_H;			/* 中断使能寄存器 或 分频值高字节寄存器 */
    volatile uint32_t IIR_FCR_DL_D;		/* 中断状态寄存（只读） FIFO控制寄存器 （只写）  或   分频值小数寄存器  */
    volatile uint32_t LCR;			/* 线路控制寄存器  */
    volatile uint32_t MCR;			/* MODEM控制寄存器  */
    volatile uint32_t LSR;			/* 线路状态寄存器  */

} UART_TypeDef;

#define UART0_TxData                  *(unsigned volatile int*)(UART0_BASEADDR+0x0)
#define UART0_RxData                  *(unsigned volatile int*)(UART0_BASEADDR+0x0)
#define UART0_IER                     *(unsigned volatile int*)(UART0_BASEADDR+0x4)
#define UART0_IIR                     *(unsigned volatile int*)(UART0_BASEADDR+0x8)
#define UART0_FCR                     *(unsigned volatile int*)(UART0_BASEADDR+0x8)
#define UART0_LCR                     *(unsigned volatile int*)(UART0_BASEADDR+0xc)
#define UART0_MCR                     *(unsigned volatile int*)(UART0_BASEADDR+0x10)
#define UART0_LSR                     *(unsigned volatile int*)(UART0_BASEADDR+0x14)
#define UART0_DLL                     *(unsigned volatile int*)(UART0_BASEADDR+0x0)
#define UART0_DLH                     *(unsigned volatile int*)(UART0_BASEADDR+0x4)
#define UART0_DLD                     *(unsigned volatile int*)(UART0_BASEADDR+0x8)

#define UART1_TxData                  *(unsigned volatile int*)(UART1_BASEADDR+0x0)
#define UART1_RxData                  *(unsigned volatile int*)(UART1_BASEADDR+0x0)
#define UART1_IER                     *(unsigned volatile int*)(UART1_BASEADDR+0x4)
#define UART1_IIR                     *(unsigned volatile int*)(UART1_BASEADDR+0x8)
#define UART1_FCR                     *(unsigned volatile int*)(UART1_BASEADDR+0x8)
#define UART1_LCR                     *(unsigned volatile int*)(UART1_BASEADDR+0xc)
#define UART1_MCR                     *(unsigned volatile int*)(UART1_BASEADDR+0x10)
#define UART1_LSR                     *(unsigned volatile int*)(UART1_BASEADDR+0x14)
#define UART1_DLL                     *(unsigned volatile int*)(UART1_BASEADDR+0x0)
#define UART1_DLH                     *(unsigned volatile int*)(UART1_BASEADDR+0x4)
#define UART1_DLD                     *(unsigned volatile int*)(UART1_BASEADDR+0x8)


#define UART0                  ((UART_TypeDef *) UART0_BASEADDR)
#define UART1                  ((UART_TypeDef *) UART1_BASEADDR)
#define UART_PRINT				UART0

#define RTC_FREQ   	           *(volatile unsigned int *)(RTC_BASE+0x00)    //分频值寄存器
#define RTC_CFG   	           *(volatile unsigned int *)(RTC_BASE+0x04)    //配置寄存器
#define RTC_RTC0   	           *(volatile unsigned int *)(RTC_BASE+0x08)    //时间值寄存器0
#define RTC_RTC1   	           *(volatile unsigned int *)(RTC_BASE+0x0c)    //时间值寄存器1

/******************  EXTI REGS  ******************/
typedef struct
{
    uint32_t EXINT_EN;
    uint32_t EXINT_POL;
    uint32_t EXINT_EDGE;
    uint32_t EXINT_SRC;
} EXTI_TypeDef;
#define EXTI_BASE             (EXTI_REG_BASE)
#define EXTI                  ((EXTI_TypeDef *) EXTI_BASE)

/******************  SPI REGS  ******************/

typedef struct
{
    volatile uint32_t SPCR;			/* 控制寄存器 */
    volatile uint32_t SPSR;			/* 状态寄存器 */
    volatile uint32_t DATA;			/* 数据寄存器 */
    volatile uint32_t SPER;			/* 外部寄存器 */
    volatile uint32_t PARAM;			/* 参数控制寄存器 */
    volatile uint32_t SOFTCS;			/* 片选控制寄存器 */
    volatile uint32_t TIMING;			/* 时序控制寄存器 */
} SPI_TypeDef;
#define SPI_SPCR                      *(unsigned volatile int*)(SPI_BASE+0x0)
#define SPI_SPSR                      *(unsigned volatile int*)(SPI_BASE+0x4)
#define SPI_DATA                      *(unsigned volatile int*)(SPI_BASE+0x8)
#define SPI_SPER                      *(unsigned volatile int*)(SPI_BASE+0xc)
#define SPI_PARAM                     *(unsigned volatile int*)(SPI_BASE+0x10)
#define SPI_SOFTCS                    *(unsigned volatile int*)(SPI_BASE+0x14)
#define SPI_TIMING                    *(unsigned volatile int*)(SPI_BASE+0x18)
#define SPI                           ((SPI_TypeDef *) SPI_BASE)


/******************  INTC REGS  ******************/
#define INTC_En      0x0
#define INTC_Clr     0x4
#define INTC_Out     0x8

#define PMU_COMMAND_Int_Wake      0x01
#define PMU_COMMAND_Int_BatFail   0x02
#define PMU_COMMAND_Int_C32kFail  0x04
#define PMU_COMMAND_Int_C8mFail   0x08
#define PMU_COMMAND_Int_RTC       0x10
#define PMU_COMMAND_Int_ExInt    0x100
#define PMU_COMMAND_Int_ALL      0x1ff

#define GPIO_OUTPUT 0x2
#define GPIO_INPUT  0x0
#define GPIO_HIGH   0x1
#define GPIO_LOW    0x0

#endif
/******************  FLASH REGS  ******************/
typedef struct
{
    volatile uint32_t CMD;				/* 命令寄存器 */
    volatile uint32_t CAH;				/* 加密地址上界寄存器 */
    volatile uint32_t CAL;				/* 加密地址下界寄存器 */
    volatile uint32_t RESERVED0;			/* 保留字节  */
    volatile uint32_t VRF;				/* 数据校验寄存器 */
    volatile uint32_t STS;				/* 状态寄存器 */
    volatile uint32_t PET;				/* 擦写时间寄存器 */
} FLASH_TypeDef;

#define FLASH                   ((FLASH_TypeDef *) FLASH_BASE)
#define FLASH_CMD_REG	       *(volatile unsigned int *)(FLASH_BASE+0x00)   //命令寄存器
#define FLASH_CAH_REG	       *(volatile unsigned int *)(FLASH_BASE+0x04)   //加密地址上界寄存器
#define FLASH_CAL_REG	       *(volatile unsigned int *)(FLASH_BASE+0x08)   //加密地址下界寄存器
#define FLASH_VRF_REG	       *(volatile unsigned int *)(FLASH_BASE+0x10)   //数据效验寄存器
#define FLASH_STS_REG	       *(volatile unsigned int *)(FLASH_BASE+0x14)   //状态寄存器
#define FLASH_PET_REG	       *(volatile unsigned int *)(FLASH_BASE+0x18)   //擦写时间寄存器
#ifdef  DEBUG
/*******************************************************************************
* Macro Name     : assert_param
* Description    : The assert_param macro is used for function's parameters check.
*                  It is used only if the library is compiled in DEBUG mode.
* Input          : - expr: If expr is false, it calls assert_failed function
*                    which reports the name of the source file and the source
*                    line number of the call that failed.
*                    If expr is true, it returns no value.
* Return         : None
*******************************************************************************/
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* DEBUG */

#endif /*_LS1X_H_ */

