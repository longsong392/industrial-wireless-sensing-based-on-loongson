#include "UserGpio.h"

#include "ls1x.h"



#if defined (LS1C102)
//#define	 UART0_RX_PIN 				GPIO_Pin_14
//#define	 UART0_TX_PIN 				GPIO_Pin_15
//#define	 UART0_PORT 				GPIOA
//#define	 UART0_AFIO 				AFIOA
//#define	 UART0_PIN_FUNC 			GPIO_FUNC_MAIN
//
//#define	 SPI_CLK                    GPIO_Pin_4
//#define	 SPI_MOSI                   GPIO_Pin_5
//#define	 SPI_MISO                   GPIO_Pin_6
//#define	 SPI_CSB                    GPIO_Pin_7
//#define	 SPI_PORT                   GPIOB
//#define	 SPI_AFIO                   FIOB
//#define	 SPI_PIN_FUNC 				GPIO_FUNC_MAIN
#elif defined (LS1C103)
//#define	 UART0_RX_PIN 				GPIO_Pin_14
//#define	 UART0_TX_PIN 				GPIO_Pin_15
//#define	 UART0_PORT 				GPIOA
//#define	 UART0_AFIO 				AFIOA
//#define	 UART0_PIN_FUNC 			GPIO_FUNC_MAIN
#define	 UART0_RX_PIN 			       	GPIO_Pin_9
#define	 UART0_TX_PIN 			       	GPIO_Pin_10
#define	 UART0_PORT 				    GPIOA
#define	 UART0_AFIO 			     	AFIOA
#define	 UART0_PIN_FUNC 		      	GPIO_FUNC_FIRST

#define	 SPI_CLK                        GPIO_Pin_4
#define	 SPI_MOSI                       GPIO_Pin_5
#define	 SPI_MISO                       GPIO_Pin_6
#define	 SPI_CSB                        GPIO_Pin_7
#define	 SPI_PORT                       GPIOB
#define	 SPI_AFIO                       AFIOB
#define	 SPI_PIN_FUNC 			    	GPIO_FUNC_MAIN

#define	 JTAG_TCK 		            	GPIO_Pin_8
#define	 JTAG_TDO 		            	GPIO_Pin_9
#define	 JTAG_TDI 			            GPIO_Pin_10
#define	 JTAG_TMS 			            GPIO_Pin_11
#define	 JTAG_TRST 			            GPIO_Pin_12
#define	 JTAG_PORT 			          	GPIOB
#define	 JTAG_AFIO 				        AFIOB
#define	 JTAG_PIN_FUNC 		            GPIO_FUNC_MAIN
#endif

void GPIOInit(void)
{
#if defined (LS1C102)
    AFIO_RemapConfig(AFIOA, GPIO_Pin_0 | GPIO_Pin_1 | \
                     GPIO_Pin_2 | GPIO_Pin_3 | \
                     GPIO_Pin_4 | GPIO_Pin_5 | \
                     GPIO_Pin_6 | GPIO_Pin_7 | \
                     GPIO_Pin_8 | GPIO_Pin_9 | \
                     GPIO_Pin_10 | GPIO_Pin_11 | \
                     GPIO_Pin_12 | GPIO_Pin_13 | \
                     GPIO_Pin_14 | GPIO_Pin_15, GPIO_FUNC_MAIN);

    AFIO_RemapConfig(AFIOB, GPIO_Pin_0 | GPIO_Pin_1 | \
                     GPIO_Pin_2 | GPIO_Pin_3 | \
                     GPIO_Pin_4 | GPIO_Pin_5 | \
                     GPIO_Pin_6 | GPIO_Pin_7 | \
                     GPIO_Pin_8 | GPIO_Pin_9 | \
                     GPIO_Pin_10 | GPIO_Pin_11 | \
                     GPIO_Pin_12 | GPIO_Pin_13 | \
                     GPIO_Pin_14 | GPIO_Pin_15, GPIO_FUNC_MAIN);

    AFIO_RemapConfig(AFIOC, GPIO_Pin_0 | GPIO_Pin_1 | \
                     GPIO_Pin_2 | GPIO_Pin_3 | \
                     GPIO_Pin_4 | GPIO_Pin_5 | \
                     GPIO_Pin_8 | GPIO_Pin_9 | \
                     GPIO_Pin_10 | GPIO_Pin_11 | \
                     GPIO_Pin_12 | GPIO_Pin_13 | \
                     GPIO_Pin_14 | GPIO_Pin_15, GPIO_FUNC_MAIN);

    AFIO_RemapConfig(AFIOC, GPIO_Pin_6 | GPIO_Pin_7, GPIO_FUNC_FIRST);

    AFIO_RemapConfig(AFIOD, GPIO_Pin_0 | GPIO_Pin_1 | \
                     GPIO_Pin_2 | GPIO_Pin_3 | \
                     GPIO_Pin_4 | GPIO_Pin_5 | \
                     GPIO_Pin_6 | GPIO_Pin_7 | \
                     GPIO_Pin_8 | GPIO_Pin_9 | \
                     GPIO_Pin_10 | GPIO_Pin_11 | \
                     GPIO_Pin_12 | GPIO_Pin_13 | \
                     GPIO_Pin_14 | GPIO_Pin_15, GPIO_FUNC_MAIN);
#elif defined (LS1C103)
    GPIO_InitTypeDef GPIO_InitStruct;
    /*************UART0 CONFIG************************/
    AFIO_RemapConfig(UART0_AFIO, UART0_RX_PIN | UART0_TX_PIN, UART0_PIN_FUNC);
    AFIO_RemapConfig(UART0_AFIO, GPIO_Pin_7| GPIO_Pin_8, GPIO_FUNC_SECOND);

    GPIO_InitStruct.GPIO_Pin = UART0_TX_PIN;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin =  UART0_RX_PIN;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_7;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART0_PORT, &GPIO_InitStruct);



    /*************SPI CONFIG************************/
    AFIO_RemapConfig(SPI_AFIO, SPI_CLK | SPI_MOSI | SPI_MISO  | SPI_CSB, SPI_PIN_FUNC);

    GPIO_InitStruct.GPIO_Pin = SPI_CLK | SPI_MOSI /*| SPI_MISO */ | SPI_CSB;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = SPI_MISO;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStruct);

///*************JTAG CONFIG************************/
//  AFIO_RemapConfig(JTAG_AFIO, JTAG_TCK | JTAG_TDO | JTAG_TDI | JTAG_TMS | JTAG_TRST, JTAG_PIN_FUNC);
//
//  GPIO_InitStruct.GPIO_Pin = JTAG_TCK | JTAG_TDO/* | JTAG_TDI*/ | JTAG_TMS | JTAG_TRST;
//
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(JTAG_PORT, &GPIO_InitStruct);
//
//  GPIO_InitStruct.GPIO_Pin = JTAG_TDI;
//
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(JTAG_PORT, &GPIO_InitStruct);
#endif
}
