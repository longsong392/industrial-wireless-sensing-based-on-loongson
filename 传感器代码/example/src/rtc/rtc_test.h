/*
 * rtc_test.h
 *
 * created: 2024/1/23
 *  author: 
 */

#ifndef _RTC_TEST_H
#define _RTC_TEST_H
//void rtc_set(void);
uint32_t a_to_n(uint8_t chr);
int date (int argc, void* argv[]);
void rtc_set_time(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);
#endif // _RTC_TEST_H

