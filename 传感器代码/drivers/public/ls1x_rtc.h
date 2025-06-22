#ifndef _LS1X_RTC_H_
#define _LS1X_RTC_H_
#include "test.h"
typedef struct
{
    INT8U sec;
    INT8U min;
    INT8U hour;
    INT8U day;
    INT8U mon;
    INT8U year;
    INT8U week;
} tsRtcTime;

#define RTC_READ                        1				//读取时间
#define RTC_WRITE                       0				//写时间寄存器

void Rtc_Write(tsRtcTime *Wtime);
void Rtc_Read(tsRtcTime *Rtime);
void RtcUpdateData(INT8U direction, tsRtcTime *time);

#endif

