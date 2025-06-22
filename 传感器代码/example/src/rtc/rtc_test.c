/*
 * rtc_test.c
 *
 * created: 2024/1/23
 *  author: 
 */
#include "ls1x.h"
#include "ls1x_rtc.h"

tsRtcTime rtc_param;
void rtc_set_time(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    rtc_param.sec = second;
    rtc_param.min = minute;
    rtc_param.hour = hour;
    rtc_param.day = day;
    rtc_param.mon = month;
    rtc_param.year = year;
    RtcUpdateData(0, &rtc_param);
}




