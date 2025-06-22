#ifndef _LS1C103_TIMER_H_
#define _LS1C103_TIMER_H_
#include"Config.h"
#include"test.h"
typedef struct
{
    INT32U begin;
    INT32U end;
} TIMER_COUNT;

typedef struct		//����ʱ�ӽṹ������
{
    INT32U TimeStart;//ʱ����ʼʱ��
    INT32U TimeInter;//���ʱ��

} tsTimeType;


INT32U		get_count(void);
void 		start_count(TIMER_COUNT *timer_count);
INT32U 		stop_count(TIMER_COUNT *timer_count);
void 		delay_cycle(INT32U num);
void 		delay_us(INT32U x);		    //us��λ,��׼��ʱ
void 		delay_ms(INT32U x);		    //ms��λ,��׼��ʱ
void 		delay_s(INT32U x);  	    //s ��λ,��׼��ʱ

void SetTime(tsTimeType *TimeType, INT32U TimeInter); //�����ӿ�?
INT8U  CompareTime(tsTimeType *TimeType);//�ȽϺ������з���ֵ��ֻҪ����ṹ��ָ�뼴��
#endif
