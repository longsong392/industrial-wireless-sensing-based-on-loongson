#ifndef _Interrupt_H_
#define _Interrupt_H_

#define DMA_INT_EN      0x80
#define VPWM_INT_EN     0x40
#define SPI_INT_EN      0x20
#define FLASH_INT_EN    0x10
#define UART0_INT_EN    0x08
#define UART1_INT_EN    0x04
#define I2C_INT_EN      0x02
#define TIMER_INT_EN    0x01

#define DMA_INT_CLR     0x80
#define VPWM_INT_CLR    0x40
#define SPI_INT_CLR     0x20
#define FLASH_INT_CLR   0x10
#define UART0_INT_CLR   0x08
#define UART1_INT_CLR   0x04
#define I2C_INT_CLR     0x02
#define TIMER_INT_CLR   0x01

#define DMA_INT_OUT     0x80
#define VPWM_INT_OUT    0x40
#define SPI_INT_OUT     0x20
#define FLASH_INT_OUT   0x10
#define UART0_INT_OUT   0x08
#define UART1_INT_OUT   0x04
#define I2C_INT_OUT     0x02
#define TIMER_INT_OUT   0x01

#define DMA_INT_EDGE    0x80
#define VPWM_INT_EDGE   0x40
#define SPI_INT_EDGE    0x20
#define FLASH_INT_EDGE  0x10
#define UART0_INT_EDGE  0x08
#define UART1_INT_EDGE  0x04
#define I2C_INT_EDGE    0x02
#define TIMER_INT_EDGE  0x01

#define DMA_INT_POL     0x80
#define VPWM_INT_POL    0x40
#define SPI_INT_POL     0x20
#define FLASH_INT_POL   0x10
#define UART0_INT_POL   0x08
#define UART1_INT_POL   0x04
#define I2C_INT_POL     0x02
#define TIMER_INT_POL   0x01

#define DMA_INT_SET     0x80
#define VPWM_INT_SET    0x40
#define SPI_INT_SET     0x20
#define FLASH_INT_SET   0x10
#define UART0_INT_SET   0x08
#define UART1_INT_SET   0x04
#define I2C_INT_SET     0x02
#define TIMER_INT_SET   0x01

//void			SOFT_INT_Handler(void);
//void			TIMER_8M_INT_Handler(void);
//void			TIMER_WAKE_INT_Handler(void);
//void			TOUCH_Handler(void);
//void			UART2_INT_Handler(void);
//void			BAT_FAIL_Handler(void);
//void			INTC_Handler(void);
//void			RING_Handler(void);
//void 			IRQ_Exception();
/////////////////////////////////////////////////////////////////////////
///////////////////////////////Interrupt/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void		SOFT_INT(void);
void		TIMER_8M_INT(void);
void		TIMER_WAKE_INT(void);
void		TOUCH(void);
void		UART2_INT(void);
void		BAT_FAIL(void);
void		INTC(void);
void		RING(void);

#endif

