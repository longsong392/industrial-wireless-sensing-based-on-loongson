#ifndef CONFIG_H_
#define CONFIG_H_

#define	DEBUG()  					printf("\r\n <  line: %d func:%s   >\r\n",__LINE__,__FUNCTION__)

void ls1x_logo(void);

#define PRINT 1               //是否开启打印  			0:关闭打印; 					1:开启打印

#if PRINT
extern int  myprintf(char *fmt, ...);
extern unsigned char mygetchar(void);
extern void myputchar(unsigned char chr);

#define log_printf(fmt, args...) 	myprintf(fmt, ##args)
#define log_putchar(chr) 			myputchar(chr)
#define log_getchar() 				mygetchar()
#define log_nb_printf(fmt, args...) log_printf(fmt, ##args)
#define log_rx_printf(fmt, args...) log_printf(fmt, ##args)
#define log_uart_printf(fmt, args...) log_printf(fmt, ##args)
#define printf(fmt, args...) 		log_printf(fmt, ##args)
#define putchar(fmt, args...) 		log_putchar(fmt, ##args)
#define getchar() 					log_getchar()
#else
#define log_printf(fmt, args...)
#define log_nb_printf(fmt, args...)
#define log_rx_printf(fmt, args...)
#define log_uart_printf(fmt, args...)
#define printf(fmt, args...)
#endif // PRINT

#endif // CONFIG_H_


