/*
 * BH1750.h
 *
 * created: 2024/5/19
 *  author: 
 */
#ifndef _BH1750_H
#define _BH1750_H

#ifdef __cplusplus
extern "C" {
#endif
#define BYTE unsigned int
void gpio_BH1750_init(void);
void BH1750_Start();
void BH1750_Stop();
void BH1750_SendACK(unsigned char ack);
int BH1750_RecvACK();
void BH1750_SendByte(BYTE dat);
BYTE BH1750_RecvByte();
void Single_Write_BH1750(int command);
void Init_BH1750();
int Multiple_Read_BH1750(void);
#include "UserGpio.h"


#ifdef __cplusplus
}
#endif

#endif // _BH1750_H

