/*
 * iic_bus.h
 *
 * created: 2024/5/23
 *  author: 
 */

#ifndef _IIC_BUS_H
#define _IIC_BUS_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ls1x.h"
#include "test.h"
#include "ls1x_latimer.h"
#include "ls1x_gpio.h"

#define Soft_I2C_SCL_0 gpio_write_pin(GPIO_PIN_30,0)
#define Soft_I2C_SCL_1 gpio_write_pin(GPIO_PIN_30,1)
#define Soft_I2C_SDA_0 gpio_write_pin(GPIO_PIN_31,0)
#define Soft_I2C_SDA_1 gpio_write_pin(GPIO_PIN_31,1)

#define I2CT_FLAG_TIMEOUT         ((INT32U)0x1000)
#define I2CT_LONG_TIMEOUT         ((INT32U)(10 * I2CT_FLAG_TIMEOUT))

void I2C_Bus_Init(void);
void Set_I2C_Retry(unsigned short ml_sec);
INT16U Get_I2C_Retry(void);
int32_t Sensors_I2C_ReadRegister(INT8U slave_addr,INT8U reg_addr,INT16U len,INT8U *data_ptr);
uint32_t Sensors_I2C_WriteRegister(uchar_t slave_addr,uchar_t reg_addr,uchar_t len,ucint8_t *data_ptr);


#ifdef __cplusplus
}
#endif

#endif // _IIC_BUS_H
