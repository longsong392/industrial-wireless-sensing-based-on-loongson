/*
 * SHT30.h
 *
 * created: 2025/5/5
 *  author:
 */

#ifndef _SHT30_H
#define _SHT30_H

#ifdef __cplusplus
extern "C" {
#endif

void gpio_SHT30_init(void);
void SHT30_Start();
void SHT30_Stop();
void SHT30_SendACK();
unsigned char SHT30_RecvACK();
void SHT30_SendByte(unsigned char dat);
unsigned char SHT30_ReadByte();
void SHT30_Init();
int SHT30_temp(void);
int SHT30_humi(void);
void SHT30_Get_Temp();
int SHT30_Get_Humi();

#ifdef __cplusplus
}
#endif

#endif // _SHT30_H

