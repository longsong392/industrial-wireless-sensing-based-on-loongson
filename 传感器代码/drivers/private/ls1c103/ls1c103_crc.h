#ifndef __LS1C103_CRC_H__
#define __LS1C103_CRC_H__

#include "test.h"

#define CRC_DR  *(volatile unsigned int*)(CRC_BASE + 0x0)
#define CRC_IDR *(volatile unsigned int*)(CRC_BASE + 0x4)
#define CRC_RST *(volatile unsigned int*)(CRC_BASE + 0x8)

unsigned crc32(unsigned, unsigned);
void crc_rst();

#endif
