
//#include"ls1c101.h"
#include "ls1x.h"
#include "Config.h"
#include "test.h"

/***********************************************************************
函数功能:		开门
@param:		无
@other:     无
@return:	无
***********************************************************************/
typedef struct
{
    int index;								//要播放语音的下标
    int size;								//语音的大小
} Audio_TypeDef;
/*

**********************************************************************
函数功能:		语音初始化
@param:		无
@other:     无
@return:	无
**********************************************************************
void vpwm_init_dma_bd(void)
{
#if 0      							//外部8M
    VPWM_CFG = 0x48bb1f41;
#else
    INT32U sum_width;
    INT32U cfg;
    INT32U freq = (*(volatile unsigned int *)0xbf0201b0) * 1000;     	//在芯片地址bf0201b0处获取内部8M时钟的大小
    freq <<= 2;          											// busclk * 4 = osc clk
#if 0
    sum_width = (freq + (freq >> 6) + (freq >> 7)) >> (14 + 1); 		// 16k sample rate, interpol 1
    printf("sum_width1=%x\n", sum_width);
#else
    unsigned int rate = 16000 * 4;
    sum_width = (freq + rate - 1) / rate;
#endif
    cfg = 0x42ab0000 | (sum_width << 4);                  			// bit[27]  0: 32m 1: busclk
    VPWM_CFG = cfg;
#endif
}

**********************************************************************
函数功能:		语音menling初始化
@param:		无
@other:     无
@return:	无
**********************************************************************
void vpwm_init_dma_bell(void)
{
//#if 0      							//外部8M
//    VPWM_CFG = 0x48bb1f41;
//#else
//    INT32U sum_width;
//    INT32U cfg;
//    INT32U freq = (*(volatile unsigned int *)0xbf0201b0) * 1000;     	//在芯片地址bf0201b0处获取内部8M时钟的大小
//    freq <<= 2;          											// busclk * 4 = osc clk
//#if 0
//    sum_width = (freq + (freq >> 6) + (freq >> 7)) >> (14 + 1); 		// 16k sample rate, interpol 1
//    printf("sum_width1=%x\n", sum_width);
//#else
//    unsigned int rate = 16000 * 10;
//    sum_width = (freq + rate - 1) / rate;
//#endif
//    cfg = 0x43ab0000 | (sum_width << 4);                  			// bit[27]  0: 32m 1: busclk
//    VPWM_CFG = cfg;
//#endif

    INT32U sum_width;
    INT32U freq = (*(volatile unsigned int *)0xbf0201b0) * 1000;     	//在芯片地址bf0201b0处获取内部8M时钟的大小
    freq <<= 2;          											// busclk * 4 = osc clk   32M
    unsigned char insert = 3;
	unsigned int rate = 10000 * (1 << insert);
    INT32U cfg;
    sum_width = (freq + rate - 1) / rate;
//	cfg = 0x40cb0000 | ((sum_width & 0xfff) << 4); // bit[27]  0: 32m 1: busclk
	cfg = 0x40ab0000 | ((sum_width & 0xfff) << 4); // bit[27]  0: 32m 1: busclk
	cfg |= (insert & 0x0f) << 24;
	VPWM_CFG = cfg;

}

void vpwm_init_dma_tips(void)
{
#if 0      							//外部8M
    VPWM_CFG = 0x48bb1f41;
#else
    INT32U sum_width;
    INT32U cfg;
    INT32U freq = (*(volatile unsigned int *)0xbf0201b0) * 1000;     	//在芯片地址bf0201b0处获取内部8M时钟的大小
    freq <<= 2;          											// busclk * 4 = osc clk
#if 0
    sum_width = (freq + (freq >> 6) + (freq >> 7)) >> (14 + 1); 		// 16k sample rate, interpol 1
    printf("sum_width1=%x\n", sum_width);
#else
    unsigned int rate = 16000 * 8;
    sum_width = (freq + rate - 1) / rate;
#endif
    cfg = 0x43ab0000 | (sum_width << 4);                  			// bit[27]  0: 32m 1: busclk
    VPWM_CFG = cfg;
#endif
}



**********************************************************************
function:	DMA复位
@param	:	无
@other	:	无
@return :	无
@auther :	朱晓宇
@time	:	2020年4月9日
**********************************************************************
void dma_reset(void)
{
    unsigned int status;
    DMA_CMD_STATUS = 0x80000000;    //DMA控制器写1复位
    status = DMA_CMD_STATUS;        //读DMA控制器，为0则表示软复位完成
    while (status != 0x1)
        status = DMA_CMD_STATUS;
}
**********************************************************************
function:	DMA搬运数据
@param	:	source		存放语音的地址
			count		数据大小
			mode		是否使能DMA
@other	:	无
@return :	无
@auther :	朱晓宇
@time	:	2020年4月9日
**********************************************************************
void dma_start(INT32U source, INT32U count, INT32U mode)
{
    unsigned int status;
    status = DMA_CMD_STATUS;
    while(status != 0x1)            //命令可写
        status = DMA_CMD_STATUS;
//    DMA_SOURCE = ((unsigned int)source) & 0x00ffffff;
    DMA_SOURCE = ((unsigned int)source) & 0x1fffffff;
    DMA_COUNT  = count;

    if(mode == 0x1)
        DMA_CMD_STATUS = 0x3;       	//使能DMA
    else
        DMA_CMD_STATUS = 0x1;
}
*/

