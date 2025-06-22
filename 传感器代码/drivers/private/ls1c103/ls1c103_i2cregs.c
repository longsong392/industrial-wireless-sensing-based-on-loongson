//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "ls1c103_i2cregs.h"

void apb_w(int addr, unsigned data)
{
    *(volatile unsigned*)(I2C_BASE + addr) = data;
}
void apb_r(int addr)
{
    rdata = *(volatile unsigned*)(I2C_BASE + addr);
}
void wait_reg0(int addr, unsigned flag)
{
    apb_r(addr);
    while((rdata & flag) != 0) apb_r(addr);
}
void wait_reg1(int addr, unsigned flag)
{
    apb_r(addr);
    while((rdata & flag) == 0) apb_r(addr);
}

