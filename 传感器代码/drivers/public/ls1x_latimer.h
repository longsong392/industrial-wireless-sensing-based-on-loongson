#ifndef _LS1X_LATIMER_H_
#define _LS1X_LATIMER_H_

#include "test.h"
#include "ls1x.h"
typedef struct
{
    uint32_t	begin;
    uint32_t	end;
} TIMER_COUNT;

typedef struct
{
    uint32_t	TimeStart;
    uint32_t	TimeInter;

} tsTimeType;

uint8_t  CompareTime(tsTimeType *TimeType);

uint32_t get_count(void);
uint32_t stop_count(TIMER_COUNT *timer_count);

void start_count(TIMER_COUNT *timer_count);
void delay_cycle(uint32_t num);
void delay_us(uint32_t x);
void delay_ms(uint32_t x);
void delay_s(uint32_t x);
void open_count();
void close_count();
void SetTime(tsTimeType *TimeType, uint32_t TimeInter);

#endif

