#include "ls1x.h"
#include "ls1x_rtc.h"

void Rtc_Read(tsRtcTime *Rtime)
{
    INT32U Rtc_time1 = 0;
    INT32U Rtc_time2 = 0;

    Rtc_time1 = RTC_RTC1;
    Rtc_time2 = RTC_RTC0;

    Rtime->year = (Rtc_time1 & 0xFE00) >> 9;
    Rtime->mon =  (Rtc_time1 & 0x1E0) >> 5;
    Rtime->day =  (Rtc_time1 & 0x1F);

    Rtime->hour = (Rtc_time2 & 0x1F0000) >> 16;
    Rtime->min =  (Rtc_time2 & 0xFC00) >> 10;
    Rtime->sec =  (Rtc_time2 & 0x3F0) >> 4;

}

void Rtc_Write(tsRtcTime *Wtime)
{
    if(((Wtime->mon) > 12) || ((Wtime->mon) <= 0))
        Wtime->mon = 1;
    if(((Wtime->day) > 31) || ((Wtime->day) <= 0))
        Wtime->day = 1;

    RTC_FREQ = (0x800 << 16) ;                                          // 32768Hz -> 16Hz
    RTC_RTC0 = (((Wtime->hour) % 24) << 16) | (((Wtime->min) % 60) << 10) | (((Wtime->sec) % 60) << 4) | (((Wtime->week) % 100) << 0); // ???ио?????????????
    RTC_RTC1 = ((Wtime->year) << 9) | (((Wtime->mon) % 13) << 5) | (((Wtime->day) % 32) << 0); 				 // ???ио????????????
    RTC_CFG = 0x80000000;
}

void RtcUpdateData(INT8U direction, tsRtcTime *time)
{
    // INT32U ret;
    if(direction)	              //??
    {
        Rtc_Read(time);
    }
    else			              //з╒
    {
        Rtc_Write(time);
    }

}

