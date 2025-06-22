#ifndef __LS1C103_I2CREGS_H__
#define __LS1C103_I2CREGS_H__

//#include "ls1c103.h"
#include "ls1x.h"

#define I2C_CR1   0x0
#define I2C_CR2   0x4
#define I2C_OAR   0x8
#define I2C_DR    0x10
#define I2C_SR1   0x14
#define I2C_SR2   0x18
#define I2C_CCR   0x1c
#define I2C_TRISE 0x20
    
#define CR1_PE      0x1
#define CR1_RECOVER 0x4000
#define CR1_START   0x100
#define CR1_STOP    0x200
#define CR1_SWRST   0x8000
#define CR1_ACK     0x400
#define CR1_POS     0x800

#define CR2_DMAEN   0x800
    
#define SR1_SB      0x1
#define SR1_ADDR    0x2
#define SR1_BTF     0x4
#define SR1_STOPF   0x10
#define SR1_RXNE    0x40
#define SR1_TXE     0x80
#define SR1_BERR    0x100
#define SR1_ARLO    0x200
#define SR1_AF      0x400
#define SR1_OVR     0x800
    
#define SR2_MSL     0x1
#define SR2_BUSY    0x2
#define SR2_TRA     0x4
#define SR2_GENCALL 0x10

unsigned rdata;

void apb_w(int addr, unsigned data);
void apb_r(int addr);
void wait_reg0(int addr, unsigned flag);
void wait_reg1(int addr, unsigned flag);

#endif
