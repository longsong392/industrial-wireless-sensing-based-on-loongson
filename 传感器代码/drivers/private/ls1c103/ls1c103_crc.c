//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"
#include "ls1c103_crc.h"

unsigned crc32(unsigned in, unsigned rem)
{
    int i;
    unsigned t,r,rh;
    r = rem;

    for(i=31; i>=0; i--) {
        t  = (in >> i ) & 1;
        rh = (r  >> 31)    ;
        r  = (r  << 1 )    ;
        if(t ^ rh) r = r ^ 0x04c11db7;
    }
    return r;
}

void crc_rst()
{
    CRC_RST = 1;
}
