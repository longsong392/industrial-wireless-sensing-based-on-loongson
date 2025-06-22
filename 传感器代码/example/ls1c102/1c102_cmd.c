#include "ls1c102_ptimer.h"
#include "ls1x_common.h"
#include "ls1x_gpio.h"
#include "ls1x_exti.h"
#include "ls1x_latimer.h"
#include "ls1x_rtc.h"
#include "ls1c102_touch.h"
#include "ls1x_string.h"
#include "ls1x_wdg.h"
#include "ls1x_uart.h"
#include "ls1x_spi.h"
#include "ls1c102_i2c.h"
#include "Config.h"

extern void cpu_sleep(void);
extern void m4_flash_4k(uint32_t tmp);
//extern void dma_start(INT32U source, INT32U count, INT32U mode);
//extern void dma_reset(void);
unsigned int str2num(unsigned char *);
int do_nothing()
{
    return 0;
}

int do_d1(int argc, void *argv[])
{
    if((argc < 2)||(argc > 3))
    {
        printf("\n usage: d1  <addr>  <num>\n\r");
        return 1;
    }
    uint32_t addr;
    int i, num;
    addr = str2num(argv[1]);
    if(argc == 2) num = 1;
    else num = str2num(argv[2]);

    for(i=0; i<num; i++)
    {
        if((i%8) == 0)
            printf("\n 0x%08x:\t", addr+i);
        printf(" %02x ", *(volatile uint8_t*)(addr+i));
    }

    return 0;
}

int do_d4(int argc, void *argv[])
{
    if((argc < 2)||(argc > 3))
    {
        printf("\n usage: d4 <addr> <num>\n\r");
        return 1;
    }
    unsigned int addr;
    int i, num;

    addr = str2num(argv[1]);
    if(argc == 2) num = 1;
    else num = str2num(argv[2]);

    for(i=0; i<num; i++)
    {
        if((i%4) == 0)
            printf("\n0x%08x:\t", addr+i*4);
        printf(" %08x ", *(volatile unsigned int*)(addr+i*4));
    }

    return 0;
}

int do_m1(int argc, void *argv[])
{
    unsigned int addr;
    unsigned char value;
    if(argc != 3)
    {
        printf("\n usage: m1 <addr> <value>\n\r");
        return 1;
    }
    addr = str2num(argv[1]);
    value = (unsigned char)str2num(argv[2]);
    *(volatile unsigned char*)(addr) = value;

    return 0;
}

int do_m4(int argc, void *argv[])
{
    unsigned int addr;
    unsigned int value;
    if(argc != 3)
    {
        printf("\n usage: m4 <addr> <value>\n\r");
        return 1;
    }
    addr = str2num(argv[1]);
    value = str2num(argv[2]);
    *(volatile unsigned int*)(addr) = value;

    return 0;
}

unsigned int str2num(unsigned char *s)
{
    unsigned int num = 0;
    unsigned int radix;
    unsigned int chr;

    unsigned int i;
    if((*(s)=='0') && (*(s+1)=='x' || *(s+1)=='X'))
    {
        i = 2;
        radix = 16;
    }
    else if(*(s)=='0')
    {
        i = 1;
        radix = 8;
    }
    else
    {
        i = 0;
        radix = 10;
    }

    for(; *(s+i)!='\0'; i++)
    {
        chr = *(s+i);
        switch(radix)
        {
            case 16:
                if(chr>='0' && chr<='9')
                    num = num*radix + (chr - '0');
                else if(chr>='a' && chr<='f')
                    num = num*radix + (chr - 'a' + 10);
                else if(chr>='A' && chr<='F')
                    num = num*radix + (chr - 'A' + 10);
                else
                    printf("illegal hex number...\n");
                break;
            case 10:
            case 8:
                if(chr>='0' && chr<('0'+radix))
                    num = num*radix + (chr - '0');
                else
                    printf("illegal number...\n");
                break;
            default:
                break;
        }
    }

    return num;
}

/*VPWM:0xbfec0020 0x309817b0 single signal period/2 */
/*VPWM:0xbfec0020 0x70a817b0 double signal period/2*/
/*VPWM:0xbfec0020 0x72980ff0 16Khz*/

int v_play(int argc, void *argv[])
{
    if(argc != 3)
    {
        printf("\n usage: v <addr> <num>\n\r");
        return 1;
    }
    //unsigned int addr, count;
    //addr = str2num(argv[1]);
    //count = str2num(argv[2]);

    //dma_reset();
    printf("VPWM dma ad test started \n");
    //dma_start(addr, count, 0x1);
    printf("VPWM dma ad test end\n");
    return 0;
}
// touch show
int touch_reg_show()
{
    printf("\nTsensor:\n");
    printf("TS_Ctrl    = 0x%8x\n", TS_CTRL);
    printf("TS_OscTh   = 0x%8x\n", TS_OSCTH);
    printf("TS_PollTim = 0x%8x\n", TS_POLLTIM);
    printf("       ChnAttr       CntRes\n");
    int i;
    for (i=0; i<12; i++)
        printf("[%2d] = 0x%8x    0x%8x\n", i, TS_CHNATTR(i), TS_CNTRES(i));
    return 0;
}
int touch_value_show()
{
    printf("\n         00   01   02   03   04   05   06   07   08   09   10   11\n");
    int i;
    printf("basval");
    for (i=0; i<12 ; i++)
        printf(" %4d", TOUCH_GetBaseVal(i));
    printf("\ncntval");
    for (i=0; i<12; i++)
        printf(" %4d", TOUCH_GetCountValue(i));
    return 0;
}

int touch_test(int argc, void *argv[])
{
    if (!strcmp("init",argv[1]))
    {
        if (argc != 3)
        {
            printf("\n touch init <down_th>.");
            return 1;
        }
        printf("\n Touch Init");
        INT8U down_th = str2num(argv[2]);
        TOUCH_Init(down_th);
    }
    else if (!strcmp("scan",argv[1]) && argc==2)
    {
        printf("\n Touch Scan");
        TOUCH_EnableSingleScan();
        // Printf_CountVal();
    }
    else if (!strcmp("down",argv[1]))
    {
        printf("\n Touch DownTh");
        INT8U down_th = str2num(argv[2]);
        INT8U channel;
        if(argc == 3)        //All Channel
        {
            for(channel=0; channel<CHANNEL_NUM; channel++)
            {
                TOUCH_SetDownTh(channel, down_th);
            }
        }
        else if (argc == 4)  //Single Channel
        {
            channel = str2num(argv[3]);
            {
                TOUCH_SetDownTh(channel, down_th);
            }
        }
        else
        {
            printf("\n touch down <down_th> <channel>");
            return 1;
        }
    }
    else if (!strcmp("poll",argv[1]) && argc==2)
    {
        printf("\n Touch Poll.");
        TOUCH_EnablePollScan();
    }
    else if (!strcmp("show",argv[1]) && argc==2)
    {
        touch_reg_show();
        touch_value_show();
    }
    else
    {
        printf("\n Input Error.");
        printf("\nUsage: touch init <down_th> \
		\n       touch down <down_th> <channel> \
	        \n       touch scan \
		\n       touch poll \
		\n       touch show");
        return 1;
    }
    return 0;
}

int hpet(int argc, void *argv[])
{
    printf("\r\n hpet timer start....\n");

    uint32_t periodic,cmp,stp;
    if(argc < 4)
    {
        TIM_Cmd(0);
        TIM_ITConfig(0);
        printf("\r\n Usage: hpet <periodic> <start_ms> <sec>");
        return 1;
    }

    periodic = str2num(argv[1]);
    cmp = str2num(argv[2]);
    stp = str2num(argv[3]);

    TIM_InitTypeDef TIM_InitStruct;

    TIM_StructInit(&TIM_InitStruct);

    TIM_InitStruct.TIME_PERIODIC = periodic << 2;
    TIM_InitStruct.TIME_CMP = cmp * 7999;
    TIM_InitStruct.TIME_STP = stp * 8000000;

    TIM_Init(&TIM_InitStruct);

    return 0;

}

int latimer(int argc, void *argv[])
{
    uint32_t sec;
    printf("\r\n core timer start....argc:%d\n",argc);

    if(argc < 2)
    {
        Set_Timer_stop();
        printf("\r\n Usage: latimer <sec>");
        return 1;
    }

    sec = str2num(argv[1]);
    //Set_Timer_Init(sec*8000000);
    Set_Timer_Init(sec*10667000);

    return 0;
}

int delay(int argc, void *argv[])
{

    uint32_t flags,val;

    if(argc < 3)
    {
        printf("\r\n Usage: delay <mode:1:us,2:ms,3:s> <val>");
        return 1;
    }

    flags = str2num(argv[1]);
    val = str2num(argv[2]);

    if (flags == 1)
    {
        delay_us(val);
        printf("\r\n%dus",val);
    }
    else if (flags == 2)
    {

        delay_ms(val);
        printf("\r\n%dms",val);
    }
    else if (flags == 3)
    {

        delay_s(val);
        printf("\r\n%ds",val);
    }
    else
    {
        printf("\r\n Usage: delay <mode:1:us,2:ms,3:s> <val>");
    }
    return 0;
}

int wakeup(int argc, void *argv[])
{
    uint32_t sec;
    printf("\r\nTimer Wake Up start....argc:%d\n",argc);
    DisableInt();
    if(argc < 2)
    {
        Wake_Set(0);
        printf("\r\n Usage: wakeup <sec>");
        return 1;
    }

    sec = str2num(argv[1]);
    Wake_Set(sec);
    cpu_sleep();

    return 0;
}

uint32_t a_to_n(uint8_t chr)  //ascii to number
{
    if (chr < 0x30 || chr > 0x39)
    {
        printf("\r\nwrong range of number!\r\n");
        return -1;
    }
    return (chr & 0x0f);
}


/***rtc test***/
int date (int argc, void* argv[])
{
    printf("\r\n");
    char *param_str [4] = {"-h", "-s", "-i", "-f"};
    if (!strcmp(argv[1], param_str[0]))
    {
        printf("date to display time. \r\n");
        printf("date -s yy-mm-dd hh:mm:ss to set time.\r\n");
        printf("date -i mm-dd hh:mm:ss to set int.\r\n");
        printf("date -f Hz :input freqency in integer to set prescale.\r\n");
        return 0;
    }
    if (!strcmp(argv[1], param_str[1]))  //set time
    {
        if (argc != 4)
        {
            printf("check parameter\r\n");
            return 1;
        }
        //uint32_t rtc0, rtc1;
        uint32_t yy, mm, dd, h, m, s;
        uint32_t invalid_param;
        uint8_t *rtc1_str, *rtc0_str;
        tsRtcTime rtc_param;
        rtc1_str = (uint8_t*) argv[2];
        rtc0_str = (uint8_t*) argv[3];
        yy = 10*a_to_n(*(rtc1_str+0))+a_to_n(*(rtc1_str+1));
        mm = 10*a_to_n(*(rtc1_str+3))+a_to_n(*(rtc1_str+4));
        dd = 10*a_to_n(*(rtc1_str+6))+a_to_n(*(rtc1_str+7));
        h  = 10*a_to_n(*(rtc0_str+0))+a_to_n(*(rtc0_str+1));
        m  = 10*a_to_n(*(rtc0_str+3))+a_to_n(*(rtc0_str+4));
        s  = 10*a_to_n(*(rtc0_str+6))+a_to_n(*(rtc0_str+7));
        invalid_param = mm > 12 || mm == 0 || dd > 31 ||
                        dd == 0 || ((mm == 2) && (dd > 29)) ||
                        h  > 24 || m > 60  || s  > 60 ;
        if (invalid_param)
        {
            printf("invalid param");
            return 1;
        }

        rtc_param.sec = s;
        rtc_param.min = m;
        rtc_param.hour = h;
        rtc_param.day = dd;
        rtc_param.mon = mm;
        rtc_param.year = yy;

        RtcUpdateData(0, &rtc_param);

    }
    else if (!strcmp(argv[1], param_str[2]))  //set int
    {
        if (argc != 4)
        {
            printf("check parameter\r\n");
            return 1;
        }

#if 0
        uint32_t mm, dd, h, m, s;
        uint32_t invalid_param;
        uint8_t *cfg_str0,*cfg_str1;
        tsRtcTime rtc_param;
        cfg_str0 = (uint8_t*) argv[2];
        cfg_str1 = (uint8_t*) argv[3];
        mm = 10*a_to_n(*(cfg_str0+0))+a_to_n(*(cfg_str0+1));
        dd = 10*a_to_n(*(cfg_str0+3))+a_to_n(*(cfg_str0+4));
        h  = 10*a_to_n(*(cfg_str1+0))+a_to_n(*(cfg_str1+1));
        m  = 10*a_to_n(*(cfg_str1+3))+a_to_n(*(cfg_str1+4));
        s  = 10*a_to_n(*(cfg_str1+6))+a_to_n(*(cfg_str1+7));
        invalid_param = mm > 12 || mm == 0 || dd > 31 ||
                        dd == 0 || ((mm == 2) && (dd > 29)) ||
                        h  > 24 || m > 60  || s  > 60 ;
        if (invalid_param)
        {
            printf("invalid param");
            return 1;
        }

        rtc_param.sec = s;
        rtc_param.min = m;
        rtc_param.hour = h;
        rtc_param.day = dd;
        rtc_param.mon = mm;
        rtc_alarm_set(&rtc_param);
#endif
        return 0;
    }
    else if (!strcmp(argv[1], param_str[3]))  //set prescale
    {
#if 0
        uint32_t freq_in = str2num(argv[2]);
        if (freq_in <30000 || freq_in > 50000)
        {
            printf("\r\nshould be close to 32768 Hz\r\n");
            return 1;
        }
        uint32_t freq, freq_int, freq_frac;
        freq_int=freq_in >> 4;
        freq_frac = freq_in%16;
        //RTC_FREQ = freq;
 #endif
        return 0;
    }
    else // or display time
    {
        if (argc != 1)
            printf("date -h for usage\r\n");
        uint32_t yy, mm, dd, h, m, s;
        tsRtcTime rtc_param;

        RtcUpdateData(1,&rtc_param);

        yy = rtc_param.year;
        mm = rtc_param.mon;
        dd = rtc_param.day;
        h  = rtc_param.hour;
        m  = rtc_param.min;
        s  = rtc_param.sec;
        printf("20%02d-%02d-%02d ", yy, mm, dd);
        printf("%02d:%02d:%02d", h, m, s);
        return 0;
    }
    return 0;
}

int watch_dog(int argc, void *argv[])
{
    uint32_t sec;
    printf("\r\nwdg test start....argc:%d\n",argc);

    if(argc < 2)
    {
        WDG_DogFeed();
        printf("\r\n Usage: wdg <sec>");
        return 1;
    }

    sec = str2num(argv[1]);
    WDG_SetWatchDog(sec);

    return 0;
}

int i2cp (int argc, void *argv[])
{
    I2C_InitTypeDef I2C_InitStruct;
    I2C_StructInit(&I2C_InitStruct);
    I2C_Init(I2C,&I2C_InitStruct);
    return 0;
}

int i2cw (int argc, void *argv[])
{
    if (argc !=4)
    {
        printf("\n usage: i2cw <chipaddr> <regaddr> <wdata>\n");
        return 1;
    }
    printf ("\r\n");

    unsigned char chipaddr, regaddr, wdata;
    chipaddr = str2num(argv[1]);
    regaddr  = str2num(argv[2]);
    wdata    = str2num(argv[3]);
    unsigned short addr = ((chipaddr & 0xff) <<7) | (regaddr & 0xff);
    printf("wdata=0x%x",wdata);
    printf("addr=0x%x",addr);
    CAT24_Write(addr,wdata);
    return 0;
}

int i2cr (int argc, void *argv[])
{
    if (argc !=3)
    {
        printf("\n usage: i2cr<chipaddr><regaddr>\n");
        return 1;
    }

    unsigned int chipaddr, regaddr;
    chipaddr = str2num(argv[1]);
    regaddr  = str2num(argv[2]);
    uint8_t read_data;
    unsigned short addr = ((chipaddr & 0xff) <<7) | (regaddr & 0xff);
    read_data = CAT24_Read(addr);
    printf("\r\n rdata 0x%02x\r\n", read_data);
    return 0;
}

int adc(int argc, void* argv[])
{
    uint32_t sel   = str2num(argv[1]);
    uint32_t div   = str2num(argv[2]);
    uint32_t wrong = ( sel>7 || div>2 || (argc<3 || argc>4) );
    volatile uint32_t times = str2num(argv[3]);
    //uint32_t r;
    if (wrong)
    {
        printf("\r\nADC <sel> <div> <times>");
        printf("\r\nsel: (0)ADC_I0, (1)ADC_I1, (2)VRCAP, (3)1.0V, (4)VROUT, (5)1.0V(vr1), (6)vref, (7)dacamc");
        printf("\r\ndiv: 0: d2, 1: d4");
        return 1;
    }
    PMU_CMDSTS  =   0x8000;   // IT en
    while(times)
    {
        PMU_AdcCtrl =   (div << 4) + sel; // ADC config
        PMU_AdcCtrl |=  0x100;     // run
        uint32_t res = PMU_AdcDat;
        printf("\r\ncalculate%d", (res*3350)>>12);  // VDD: 3350 mV
        times--;
    }
    PMU_CMDSTS  &=   (~0x8000);   // IT dis
    return 0;
}

int batdet (int argc, void* argv[])
{
    if (argc != 2)
    {
        printf("\n usage: batdet <n> 0 ADC_I0, 1 GPIO0, 2 GPIO1\n");
        return 1;
    }
    uint32_t bat_det_sel = str2num(argv[1]);
    if (bat_det_sel ==0)
    {
        PMU_CHIPCTRL |= 0x20000;
    }
    if (bat_det_sel < 3)
    {
        switch (bat_det_sel)
        {
            case 0:
                PMU_CHIPCTRL    &=  0xFCFFFFFF;
                break;
            case 1:
                PMU_CHIPCTRL    &=  0xFCFFFFFF;
                PMU_CHIPCTRL    |=  (2 << 24);
                break;
            case 2:
                PMU_CHIPCTRL    &=  0xFCFFFFFF;
                PMU_CHIPCTRL    |=  (3 << 24);
                break;
        }
    }
    else
    {
        printf("\r\nwrong parameter");
        return 1;
    }
    PMU_CMDSTS  =   0x800;   // IT config
    return 0;
}

#define ERASE_CMD  0xa0000000
#define PAGE_CMD   0xe0000000
#define CLRPL_CMD  0x40000000
#define UPKEY_CMD  0xf0000000
#define UPBND_CMD  0x90000000
#define VERIF_CMD  0x10000000
#define TESTK_CMD  0x20000000
#define INTCLR_CMD 0x30000000
#define SLEEP_CMD  0xc0000000
int copy()
{
    FLASH_CAH_REG = 0xffffffff;
    FLASH_CAL_REG = 0xffffffff;
    int i, j;
    FLASH_PET_REG = 0x10;
    for (i=0xbf000000; i<0xbf020000; i=i+0x80)
    {
        FLASH_CMD_REG = ERASE_CMD|(i & 0x3ffff);
        WDG_DogFeed();
        printf(".");
        FLASH_CMD_REG = CLRPL_CMD;
        for (j=i; j<i+0x80; j=j+4)
        {
            *(volatile unsigned int*)(j) = *(volatile unsigned int*)(j-0x1000000);
            if (j == 0xbf000000)
            {
                //    printf("0xbf000000: %x\n",*(volatile unsigned int*)(j));
            }
        }
        FLASH_CMD_REG = PAGE_CMD|(i & 0x3ffff);
    }

    for (i=0xbf000000; i<0xbf020000; i=i+0x80)
    {
        for (j=i; j<i+0x80; j=j+4)
        {
            if (*(volatile unsigned int*)(j) != *(volatile unsigned int*)(j-0x1000000))
            {
                printf("copy error\n");
            }
        }
    }

    return 0;
}


int uart_init(int argc, void *argv[])
{
    if (argc != 2)
    {
        printf("\n usage: uinit <1/2> \n");
        return 1;
    }

    uint32_t num = str2num(argv[1]);
    if(num == 1)
    {
        Uart_init(UART1);
    }
    else if(num == 2)
    {
        Uart_init(UART2);
    }
    else
    {
        printf("\n usage: uinit <1/2> \n");
        return 1;
    }

    return 0;
}

#if 0
int uart_tx(int argc, void *argv[])
{
    if (argc != 2)
    {
        printf("\n usage: uart_tx <1/2> \n");
        return 1;
    }
    uint32_t num = str2num(argv[1]);

    if(num == 1)
    {
        uart1_printf("\r\nThis uart 1 tx test");
    }
    else if(num == 2)
    {
        uart2_printf("\r\nThis uart 2 tx test");
    }
    else
    {
        printf("\n usage: utx <1/2> \n");
        return 1;
    }
    return 0;
}


int uart_rx(int argc, void *argv[])
{
    if (argc != 2)
    {
        printf("\n usage: uart_rx <1/2> \n");
        return 1;
    }
    uint32_t num = str2num(argv[1]);

    if(num == 1)
    {
        while(1)
        {
            char c1;
            c1 = uart1_getchar();
            if (c1 == 0x3)
            {
                printf("\r\nThis uart 1 rx test exit");
                break;
            }
        }

    }
    else if(num == 2)
    {
        while(1)
        {
            char c2;
            c2 = uart2_getchar();
            if (c2 == 0x3)
            {
                printf("\r\nThis uart 2 rx test exit");
                break;
            }
        }

    }
    else
    {
        printf("\n usage: urx <1/2> \n");
        return 1;
    }
    return 0;
}
#endif
void sleep(uint8_t sflags)
{

    while(!(PMU_CMDSTS & 0x1));/*wail till can sleep*/

    PMU_CMDW = 0x1ff0000;/*clear int*/

    /*sflags=1 wakeup immediately,others do not wakeup*/
    if(sflags)
    {
        Wake_Set(5);/*5s*/
    }
    else
    {
#ifdef ATE_RUN
        PMU_GPIOA_O = 0xffffffff;
        PMU_GPIOB_OE = 0xfc;
        PMU_GPIOB_O = PMU_GPIOB_O | 0xffffff03;

#else
        PMU_GPIOA_OE = 0x0;
        PMU_GPIOB_OE = 0x0;
        PMU_GPIOA_OE = 0xffffffff;
        PMU_GPIOB_OE = 0xffffffff;
#endif

    }

    cpu_sleep();
}

int spi_init(int argc, void* argv[])
{
    uint16_t flash_id;
    SPI->SPCR    = 0x52; // enable mstr
    SPI->PARAM   = 0x20; // ~memory_en

    flash_id = Spiflash_Rdid();
    printf("\r\nid=0x%x\r\n",flash_id);
    // SPI->PARAM   = 0x23; // ~memory_en
    return 0;
}

int spi_write(int argc, void* argv[])
{
    if (argc != 2)
    {
        printf("\n usage: swrite <addr> \n");
        return 1;
    }
    uint32_t addr = str2num(argv[1]);
    uint32_t i,k,j=0,count;
    uint8_t *data = NULL;

    printf("\n write start\n");
    count = 512;
    for(k=0; k<count; k++)
    {
        j=k*0x100;
        for(i=0; i<0x100; i++)
        {
            data[i] = *(volatile uint8_t*)(0xbf000000+j+i);
        }
        printf(". ");
        Spiflash_Page_Program(addr+j,data,0x100);
    }
    printf("\n");
    return 0;

}
int spi_read(int argc, void* argv[])
{
    if (argc != 2)
    {
        printf("\n usage: sread <addr> \n");
        return 1;
    }
    uint32_t addr = str2num(argv[1]);
    uint32_t i,j=0,k,count;
    uint8_t* data = NULL;
    printf("\n read start\n");
    count = 512;
    for(k=0; k<count; k++)
    {
        j=k*0x100;
        Spiflash_Read_Bytes(addr+j, data, 0x100);
        for(i=0; i<0x100; i++)
        {
            if(data[i] != *(volatile uint8_t*)(0xbf000000+j+i))
            {
                printf("\ncheck error rdata=%x iaddr=%x\n",data[i],0xbf000000+j+i);
                break;
            }
        }
        printf(". ");
    }

    printf("\n");
    return 0;
}


int spi_erase(int argc, void* argv[])
{
    if (argc != 2)
    {
        printf("\n usage: serase <addr> \n");
        return 1;
    }
    uint32_t addr = str2num(argv[1]);
    printf("\n erase start\n");
    Spiflash_Block64k_Erase(addr);
    Spiflash_Block64k_Erase(addr+0x10000);

    return 0;
}


int spi_flash(int argc, void* argv[])
{
    uint8_t *data = NULL;
    uint16_t flash_id;
    uint32_t addr = 0x0;
    uint32_t i,k,j=0,count;

    SPI->SPCR    = 0x52; // enable mstr
    SPI->PARAM   = 0x20; // ~memory_en
    /*Get flash id*/
    flash_id = Spiflash_Rdid();
    printf("\r\nid=0x%x\r\n",flash_id);
    /*Erase chip*/
    Spiflash_Block64k_Erase(addr);
    Spiflash_Block64k_Erase(addr+0x10000);
    /*Program */
    printf("program start\n");
    count = 512;
    for(k=0; k<count; k++)
    {
        j=k*0x100;
        for(i=0; i<0x100; i++)
        {
            data[i] = *(volatile uint8_t*)(0xbf000000+j+i);
        }
        Spiflash_Page_Program(addr+j,data,0x100);
        printf(". ");
    }
    printf("\r\nprogram done\n");
    printf("\r\ncheckout start\n");
    count = 512;
    for(k=0; k<count; k++)
    {
        j=k*0x100;
        Spiflash_Read_Bytes(addr+j, data, 0x100);
        for(i=0; i<0x100; i++)
        {
            if(data[i] != *(volatile uint8_t*)(0xbf000000+j+i))
            {
                printf("\ncheck error rdata=%x iaddr=%x\n",data[i],0xbf000000+j+i);
                break;
            }
        }
        printf(". ");
    }
    printf("\r\ncheck done\n");

    return 0;
}


int gpio_int_test(void)
{
    printf("\r\ngpio_int_test\n");

    AFIO_RemapConfig(AFIOA, GPIO_Pin_1, GPIO_FUNC_GPIO);
    AFIO_RemapConfig(AFIOC, GPIO_Pin_3, GPIO_FUNC_GPIO);
    EXTI_InitTypeDef exti_gpio;
    EXTI_StructInit(&exti_gpio);

    exti_gpio.EXTI_GPIO = IRQ_GPIO_A1|IRQ_GPIO_C3;/*GPIOA1,GPIOC3*/
    exti_gpio.EXTI_Mode = EXTI_Mode_Edge;
    exti_gpio.EXTI_Trigger = EXTI_Trigger_Rising_High;
    exti_gpio.EXTI_GpioCmd = ENABLE;

    EXTI_Init(EXTI, &exti_gpio);
    return 0;
}

int gpio_int (int argc, void *argv[])
{
    gpio_int_test();
    return 0;
}

int tmp(int argc, void *argv[])
{
    uint32_t tmp = str2num(argv[1]);
    m4_flash_4k(tmp);


    return 0;
}

int m4_flash(int argc, void *argv[])
{
    uint32_t tmp = str2num(argv[1]);
    FLASH_PET_REG = 0x10;
    FLASH_CMD_REG = ERASE_CMD | (tmp & 0x3ff80);
    FLASH_CMD_REG = CLRPL_CMD;
    *(volatile unsigned int*)(tmp) = 0x5aa555aa;
    FLASH_CMD_REG = PAGE_CMD | (tmp & 0x3ff80);

    return 0;
}
int copy_iram(int argc, void *argv[])
{
    unsigned int j;
    printf("\r\ncopyiram start\r\n");
    for (j = 0x0; j < 0x2000; j=j + 4)
    {
        *(volatile unsigned int*)(0x80000000+j) = 0x0;
    }

    for (j = 0x0; j < 0x2000; j=j + 4)
    {
        *(volatile unsigned int*)(0x80000000+j) = *(volatile unsigned int*)(0xbe010000 + j);
    }
    for (j = 0x0; j < 0x2000; j=j + 4)
    {
        if(*(volatile unsigned int*)(0x80000000+j) != *(volatile unsigned int*)(0xbe010000 + j))
        {
            printf("copy error %x,%x,%x\n",j,*(volatile unsigned int*)(0x80000000+j), *(volatile unsigned int*)(0xbe010000 + j));
        }
    }
    asm volatile (\
                  "li.w $r12, 0x80000000;\n" \
                  "jirl $r0, $r12 , 0x0;\n"\
                 );

    return 0;
}

