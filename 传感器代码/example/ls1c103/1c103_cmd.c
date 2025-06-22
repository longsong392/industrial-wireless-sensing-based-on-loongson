//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1c103_map.h"
#include "ls1x_rtc.h"
#include "ls1x_latimer.h"
#include "ls1x_common.h"
#include "ls1x_string.h"
#include "ls1x_spi.h"
#include "ls1x_wdg.h"
#include "ls1x_gpio.h"
#include "Config.h"
INT32U wdt_caller;
extern uint32_t SystemFreq;
unsigned int str2num(unsigned char *);
extern void cpu_sleep();
extern int spi_cfg(void);
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
    INT32U addr;
    int i, num;
    addr = str2num(argv[1]);
    if(argc == 2) num = 1;
    else num = str2num(argv[2]);
    
    for(i=0; i<num; i++)
    {
        if((i%8) == 0)
        printf("\n 0x%08x:\t", addr+i);
        printf(" %02x ", *(volatile INT8U*)(addr+i));
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
        switch(radix){
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

int watch_dog(int argc, void *argv[])
{
    INT32U sec;
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

int delay(int argc, void *argv[])
{

    INT32U flags,val;

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
    else if (flags == 2) {

 		delay_ms(val);
        printf("\r\n%dms",val);
    }else if (flags == 3){

 		delay_s(val);  	    
        printf("\r\n%ds",val);
    }else {
         printf("\r\n Usage: delay <mode:1:us,2:ms,3:s> <val>");
    }
    return 0;
}

INT32U a_to_n(INT8U chr)  //ascii to number
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
        INT32U rtc0, rtc1;    
        INT32U yy, mm, dd, h, m, s;
        INT32U invalid_param;
        INT8U *rtc1_str, *rtc0_str;
        tsRtcTime rtc_param;
        rtc1_str = (INT8U*) argv[2]; 
        rtc0_str = (INT8U*) argv[3]; 
        yy = 10*a_to_n(*(rtc1_str+0))+a_to_n(*(rtc1_str+1));
        mm = 10*a_to_n(*(rtc1_str+3))+a_to_n(*(rtc1_str+4));
        dd = 10*a_to_n(*(rtc1_str+6))+a_to_n(*(rtc1_str+7));
        h  = 10*a_to_n(*(rtc0_str+0))+a_to_n(*(rtc0_str+1));
        m  = 10*a_to_n(*(rtc0_str+3))+a_to_n(*(rtc0_str+4));
        s  = 10*a_to_n(*(rtc0_str+6))+a_to_n(*(rtc0_str+7));
        invalid_param = mm > 12 || mm == 0 || dd > 31 || 
                      dd == 0 ||(mm == 2) && (dd > 29) ||
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
        INT32U mm, dd, h, m, s;
        INT32U invalid_param;
        INT8U *cfg_str0,*cfg_str1;
        tsRtcTime rtc_param;
        cfg_str0 = (INT8U*) argv[2]; 
        cfg_str1 = (INT8U*) argv[3]; 
        mm = 10*a_to_n(*(cfg_str0+0))+a_to_n(*(cfg_str0+1));
        dd = 10*a_to_n(*(cfg_str0+3))+a_to_n(*(cfg_str0+4));
        h  = 10*a_to_n(*(cfg_str1+0))+a_to_n(*(cfg_str1+1));
        m  = 10*a_to_n(*(cfg_str1+3))+a_to_n(*(cfg_str1+4));
        s  = 10*a_to_n(*(cfg_str1+6))+a_to_n(*(cfg_str1+7));
        invalid_param = mm > 12 || mm == 0 || dd > 31 || 
                      dd == 0 ||(mm == 2) && (dd > 29) ||
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

 //       bsp_rtc_alarm_set(&rtc_param);
        return 0;
    }
    else if (!strcmp(argv[1], param_str[3]))  //set prescale
    {
        INT32U freq_in = str2num(argv[2]);
        if (freq_in <30000 | freq_in > 50000)
        {
            printf("\r\nshould be close to 32768 Hz\r\n");
            return 1;
        }
        INT32U freq, freq_int, freq_frac;        
        freq_int=freq_in >> 4;
        freq_frac = freq_in%16; 
        //RTC_FREQ = freq; 
        return 0;
    }
    else // or display time
    {
        if (argc != 1)
            printf("date -h for usage\r\n");
        INT32U yy, mm, dd, h, m, s;
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

int copy_to_flash(unsigned int src, unsigned int offset, unsigned int size)
{
    unsigned int j;
    unsigned int start = offset; 
    unsigned int end = offset + size; 
    FLASH_CAH_REG = 0xffffffff;
    FLASH_CAL_REG = 0xffffffff;
    FLASH_PET_REG = 0x18;
    printf("\r\nprogram start\r\n");
    for (offset=start;offset<end;offset=offset + 0x80)
    {
        FLASH_CMD_REG = ERASE_CMD|offset;
	WDG_DogFeed();
        printf(".");
        FLASH_CMD_REG = CLRPL_CMD;
        for (j=offset;j<offset+0x80;j=j+4)
        {
            *(volatile unsigned int*)(FLASH_MEM_BASE + j) = *(volatile unsigned int*)(src + j);
        }
        FLASH_CMD_REG = PAGE_CMD|offset;
    }
    printf("\r\nprogram done\r\n");
    printf("\r\ncheck start\r\n");
    for (offset=start;offset<end;offset=offset+0x80)
    {
        for (j=offset;j<offset+0x80;j=j+4)
        {
            if (*(volatile unsigned int*)(FLASH_MEM_BASE + j) != *(volatile unsigned int*)(src + j))
            {
                printf("copy error %x\n",j);
            }
       }
    }
    printf("\r\ncheck done\r\n");

    return 0;
}

int copy(void)
{
    copy_to_flash(0x20000000, 0x0, 0x10000);
   
    return 0;
}

int wakeup(int argc, void *argv[])
{
    INT32U sec;
    printf("\r\nTimer Wake Up start....argc:%d\r\n",argc);
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

int coremark_test(int argc, void *argv[])
{
    if (argc != 2)
    {
        printf("\n usage: coremark <n> \n"); 
        return 1;
    }
    INT32U loop = str2num(argv[1]);    
	//core_main(0,0,0x66,loop,7,1,2000);
	printf("\ncoremark test pass!\n");
	return 0;
}


int spi_flash(int argc, void *argv[])
{
    uint8_t *data;
	data = (uint8_t *)0x10000700;
    uint16_t flash_id;
    uint32_t addr = 0x0;
    uint32_t i,k,j=0,count;
    
    spi_cfg();
//    SPI->SPCR    = 0x52; // enable mstr
//    SPI->PARAM   = 0x20; // ~memory_en
    /*Get flash id*/
    flash_id = Spiflash_Rdid();
    printf("\r\nid=0x%x\r\n",flash_id);
    /*Erase chip*/
    Spiflash_Block64k_Erase(addr);
    /*Program */
    printf("program start\n");
    count = 256;
    for(k=0;k<count;k++)
    {
        j=k*0x100;
        for(i=0;i<0x100;i++)
        {
            data[i] = *(volatile uint8_t*)(0x18000000+j+i);
        }
        Spiflash_Page_Program(addr+j,data,0x100);
        printf(". ");
     }
     printf("\r\nprogram done\n");
     printf("\r\ncheckout start\n"); 
     count = 256;
     for(k=0;k<count;k++)
     {
        j=k*0x100;
        Spiflash_Read_Bytes(addr+j, data, 0x100);
        for(i=0;i<0x100;i++)
        {
            if(data[i] != *(volatile uint8_t*)(0x18000000+j+i))
            {
                printf("\ncheck error rdata=%x iaddr=%x\n",data[i],0x18000000+j+i);
                break;
            }
        }
        printf(". ");
    }
    printf("\r\ncheck done\n");
    
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

int tmp(int argc, void *argv[])
{
    uint32_t tmp =0;


    return 0;
}


int copy_iram(void)
{   
    unsigned int j;
    printf("\r\ncopyiram start\r\n");
    for (j = 0x0;j < 0x2000; j=j + 4)
    {
        *(volatile unsigned int*)(0x10000000+j) = 0x0;
    }

    for (j = 0x0;j < 0x2000; j=j + 4)
    {
        *(volatile unsigned int*)(0x10000000+j) = *(volatile unsigned int*)(0x20010000 + j);
    }
    for (j = 0x0;j < 0x2000; j=j + 4)
    {
        if(*(volatile unsigned int*)(0x10000000+j) != *(volatile unsigned int*)(0x20010000 + j))
        {
            printf("copy error %x,%x,%x\n",j,*(volatile unsigned int*)(0x10000000+j), *(volatile unsigned int*)(0x20010000 + j));
        }
    }
    asm volatile (\
                  "li.w $r12, 0x10000000;\n" \
                  "jirl $r0, $r12 , 0x0;\n"\
                  );

    return 0;
}

