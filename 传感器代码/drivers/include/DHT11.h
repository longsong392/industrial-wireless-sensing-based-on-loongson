/*
 * DHT11.h
 *
 * created: 2025/4/21
 *  author: 
 */

#ifndef _DHT11_H
#define _DHT11_H

void DHT11_Rst(void);
char DHT11_Check(void);
char DHT11_Read_Bit(void);
char DHT11_Read_Byte(void);
char DHT11_Read_Data(char *temp,char *humi);



#endif // _DHT11_H

