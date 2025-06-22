#ifndef _LS1X_UART_H_
#define _LS1X_UART_H_

#include "ls1x.h"
#include "test.h"


/**
 * @说明  GPIO 初始化定义
 */
typedef enum
{
	UART_CLOCK_8M = 8000000,
	UART_CLOCK_11M = 10667000
}UARTClock_TypeDef;

typedef enum
{
	UART_TYPE_UART0=0,
	UART_TYPE_UART1,
	UART_TYPE_UART2
}teUartType;

typedef struct
{
	teUartType UartType;					//数据接收的类型
	INT8U UartRec;					//接受到的数据
}tsUsartType;

INT8U uart_rec[10];
INT8U uart_rec_len;

#define IS_UART_CLOCK(CLOCK) (((CLOCK) == UART_CLOCK_8M) || ((LENGTH) == UART_CLOCK_11M))
/**
  * @说明  UART初始化结构体定义
  */

typedef struct
{
  uint32_t UART_BaudRate;            	/* This member configures the UART communication baud rate.
                                        The baud rate is computed using the following formula:
                                        - IntegerDivider = ((PCLKx) / (16 * (UART_InitStruct->UART_BaudRate)))*/

  uint32_t Clock_Freq; 					/* Specifies uart clock freq*/

  uint16_t UART_Triger;

  uint8_t UART_WordLength;          	/* Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint8_t UART_StopBits;            /* Specifies the number of stop bits transmitted.
                                       This parameter can be a value of @ref UART_Stop_Bits */

  uint8_t UART_Parity_Control;              /* Specifies the parity mode whether Enable or disable */

  uint8_t UART_Parity;              	 	/* Specifies the parity mode */

  uint8_t UART_Parity_Bid_Control;				  /* Specifies the parity bid or not */

  uint8_t UART_Break_Control;		      /* Specifies the Beaak_Control bit state */

} UART_InitTypeDef;


/**
 * UART_Exported_Constants
 */
#define IS_UART_PERIPH(PERIPH) (((PERIPH) == UART0) || ((PERIPH) == UART1))


/**
 *@说明：UART_MODE
 */

typedef enum
{
	UART_Mode_Tx = 0,
	UART_Mode_Rx
}UARTMode_TypeDef;
#define IS_UART_MODE(MODE) (((MODE) == UART_Tx) || ((MODE) == UART_Rx))

/**
 *@说明：UART_Word_Length
 */

#define UART_Word_Length_5b                  ((uint8_t)0x00)
#define UART_Word_Length_6b                  ((uint8_t)0x01)
#define UART_Word_Length_7b                  ((uint8_t)0x02)
#define UART_Word_Length_8b                  ((uint8_t)0x03)

#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_Word_Length_5b) || \
									  ((LENGTH) == UART_Word_Length_6b) || \
									  ((LENGTH) == UART_Word_Length_7b) || \
                                      ((LENGTH) == UART_Word_Length_8b))


/**
 *@说明：UART_Stop_Bits
 * bec=5bit时，stopbit2=1.5bit
 */
#define UART_StopBits_1                     ((uint8_t)0x00)
#define UART_StopBits_2                     ((uint8_t)0x04)
#define IS_UART_STOPBITS(STOPBITS) (((STOPBITS) == UART_StopBits_1) || \
                                     ((STOPBITS) == UART_StopBits_2))



/**  UART_Parity
 *@说明：
 */


#define UART_Parity_Odd                     ((uint8_t)0x00)	/* 奇校验  */
#define UART_Parity_Even                    ((uint8_t)0x10)	/* 偶校验  */

#define IS_UART_PARITY(PARITY) (((PARITY) == UART_Parity_Even) || \
                                 ((PARITY) == UART_Parity_Odd))


#define UART_Parity_No                      ((uint8_t)0x00)	/* 不使能奇偶校验  */
#define UART_Parity_En                      ((uint8_t)0x08)	/* 使能奇偶校验  */

#define UART_Parity_No_Bid 					((uint8_t)0x00)
#define UART_Parity_Bid 					((uint8_t)0x20)

#define UART_Break_Control_NEn 				((uint8_t)0x00)
#define UART_Break_Control_En 				((uint8_t)0x40)

#define UART_Data_En 						((uint8_t)0x00)
#define UART_Lab_En 						((uint8_t)0x80)


/**
 * @说明： UART_Interrupt_definition
 */
#define UART_IT_IME                        	((uint8_t)0x08)/* 保留的 Modem中断使能 */
#define UART_IT_ILE                        	((uint8_t)0x04)/* 线路状态中断使能  */
#define UART_IT_ITE                         ((uint8_t)0x02)/* 发送状态中断使能   */
#define UART_IT_IRE                         ((uint8_t)0x01)/* 接收状态中断使能   */
#define IS_UART_CONFIG_IT(IT) (((IT) == UART_IT_IME) || ((IT) == UART_IT_ILE) || \
                  	  	  	  ((IT) == UART_IT_ITE) || ((IT) == UART_IT_IRE))



/**
 *@说明：  UART_Interrupt_state
 */
#define UART_IT_RXOT                        ((uint8_t)0x0C)  /* 接收超时，有数据有效且后续4个字符时间内无操作  */
#define UART_IT_IL                          ((uint8_t)0x06)  /* 奇偶 、 溢出 或 帧错误、 或打断中断 */
#define UART_IT_RXNE                        ((uint8_t)0x04)  /* 接收寄存器非空中断*/
#define UART_IT_TXE                         ((uint8_t)0x02)  /* 发送fifo空中断*/
#define UART_IT_MDM                         ((uint8_t)0x00)  /* 保留的 Modem寄存器，两线串口实现无此中断源*/
#define UART_IT_INTPn                       ((uint8_t)0x01)  /* 是否存在未处理的中断*/

#define IS_UART_GET_IT(IT) (((IT) == UART_IT_RXOT) || ((IT) == UART_IT_IL) || \
        					((IT) == UART_IT_RXNE) || ((IT) == UART_IT_INTPn) || \
							((IT) == UART_IT_TXE) || ((IT) == UART_IT_MDM))







#define	UART_FIFO_TRIGGER_1			         ((uint8_t)0x00) 	/* trigger at 1 chars */
#define	UART_FIFO_TRIGGER_2			         ((uint8_t)0x40)	/* trigger at 2 chars */
#define	UART_FIFO_TRIGGER_3			         ((uint8_t)0x80)	/* trigger at 3 chars */
#define	UART_FIFO_TRIGGER_4				     ((uint8_t)0xc0)	/* trigger at 4 chars */
#define IS_UART_FIFO_TRIGGER(TRIGGER) (((TRIGGER) == UART_FIFO_TRIGGER_1) || ((TRIGGER) == UART_FIFO_TRIGGER_2) || \
        					((TRIGGER) == UART_FIFO_TRIGGER_3)||((TRIGGER) == UART_FIFO_TRIGGER_4))


#define	UART_FIFO_RX_RST			            0x02	/* reset receive fifo */
#define	UART_FIFO_TX_RST			            0x04	/* reset transmit fifo */



/**
 *  UART_MCR_definition
 */

/**
  *UART_Modem_Infrared
  */
#define UART_Modem_Infrared_Disable                  ((uint8_t)0x00)
#define UART_Modem_Infrared_Enable                   ((uint8_t)0x80)
#define IS_UART_MODEM_IR(Modem_Infrared) (((Modem_Infrared) == UART_Modem_Infrared_Disable) || \
                               ((Modem_Infrared) == UART_Modem_Infrared_Enable))



/**
 * UART_Modem_CPOL
 */
#define UART_Modem_Cpol_Low                       ((uint8_t)0x00)
#define UART_Modem_Cpol_High                      ((uint8_t)0x40)
#define IS_UART_MODEM_CPOL(CPOL) (((CPOL) == UART_Modem_Cpol_Low) || ((CPOL) == UART_Modem_Cpol_High))


/**
 * UART_Modem_CPOL
 */
#define UART_Modem_Infrared_Cpol_Low                       ((uint8_t)0x00)
#define UART_Modem_Infrared_Cpol_High                      ((uint8_t)0x20)
#define IS_UART_MODEM_IR_CPOL(CPOL) (((CPOL) == UART_Modem_Infrared_Cpol_Low) || ((CPOL) == UART_Modem_Infrared_Cpol_High))


/**
 * UART_LSR_Flags
 */
#define UART_LSR_FLAG_ERR                       ((uint8_t)0x80) //Error flag
#define UART_LSR_FLAG_TE                        ((uint8_t)0x40)	//Transmit data register empty flag
#define UART_LSR_FLAG_TFE                       ((uint8_t)0x20) //Transmit fifo register empty flag
#define UART_LSR_FLAG_BI                        ((uint8_t)0x10) //break flag
#define UART_LSR_FLAG_FE                     	((uint8_t)0x08) //Framing Error flag
#define UART_LSR_FLAG_PE                      	((uint8_t)0x04) //Parity Error flag
#define UART_LSR_FLAG_OE                        ((uint8_t)0x02) //OverRun Error flag
#define UART_LSR_FLAG_RXNE                      ((uint8_t)0x01) //Receive data register not empty flag

#define IS_UART_LSR_FLAGS(FLAG) (((FLAG) == UART_LSR_FLAG_ERR)||\
								((FLAG) == UART_LSR_FLAG_TE)||\
								((FLAG) == UART_LSR_FLAG_TFE)||\
								((FLAG) == UART_LSR_FLAG_BI)||\
								((FLAG) == UART_LSR_FLAG_FE)||\
								((FLAG) == UART_LSR_FLAG_PE)||\
								((FLAG) == UART_LSR_FLAG_OE)||\
								((CPOL) == UART_LSR_FLAG_RXNE))

void UartInit(void);
void Uart1_init(uint32_t baud);
void Uart0_init(uint32_t baud);
void Uart0_send(unsigned char chr);
void Uart1_send(unsigned char chr);
void Uart2_send(unsigned char chr);
void Uart_init(UART_TypeDef* UARTx);
uint8_t UART_ReceiveData(UART_TypeDef* UARTx);

/**
 * UART_Exported_Functions
 */
void UART_SendData(UART_TypeDef* UARTx, uint8_t Data);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_ITConfig(UART_TypeDef* UARTx, uint8_t UART_IT, FunctionalState NewState);
//void UART_ITCmd(UART_TypeDef* UARTx, FunctionalState NewState);

uint8_t x_getchar(void);
uint8_t UART_ReceiveData(UART_TypeDef* UARTx);
uint8_t UART_ClearITPendingBit(UART_TypeDef* UARTx, uint16_t UART_IT);

FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint8_t UART_FLAG);
//void UART_ClearFlag(UART_TypeDef* UARTx, uint8_t UART_FLAG);

ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint8_t UART_IT_FLAG);
//void UART_ClearIT(UART_TypeDef* UARTx);



#endif

