/*
 * gpio.h
 *
 * created: 2024/5/16
 *  author: 
 */

#ifndef _GPIO_H
#define _GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1x.h"
void gpio_init(unsigned char gpio,unsigned char io);
void gpio_write(unsigned char gpio,unsigned char val);
unsigned char gpio_read(unsigned char gpio);
//void gpio_iosel(unsigned char gpio,unsigned char iosel);

#ifdef __cplusplus
}
#endif

#endif // _GPIO_H

