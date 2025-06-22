#ifndef __MOTOR_TIM_H_
#define __MOTOR_TIM_H_

#include "test.h"
#include "ls1c103_map.h"
#include "foc_ctrl.h"
#include "foc.h"




/***************************PWM CHANNEL*******************************/

#define MOTOR_TIM                 TIM1
/* 累计 2*TIM_Period个后产生一个更新或者中断,当定时器从0计数到PWM_PERIOD到0，为一个定时周期 */
#define PWM_PERIOD_COUNT        PWM_PERIOD_CYCLES
#define PWM_MAX_PERIOD_COUNT    (PWM_PERIOD_COUNT - 10)
#define PWM_PRESCALER_COUNT     (1)

#define TIM_COM_TS_ITRx         TIM_TS_ITR3    // 内部触发配置(TIM4->TRGO->TIM1->ITR3)

/**************************ENCODER CHANNEL****************************/
/* 定时器选择 */
#define ENCODER_TIM                         TIM2
/* 定时器溢出值 */		
#define ENCODER_TIM_PERIOD                  ENCODER_TOTAL_RESOLUTION
/* 定时器预分频值 */
#define ENCODER_TIM_PRESCALER               0
/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION                  1000
/* 经过倍频之后的总分辨率 */
#define ENCODER_TOTAL_RESOLUTION           (ENCODER_RESOLUTION * 4)  /* 4倍频后的总分辨率 */
#define POLE_PAIR_NUM (4) //极对数

extern volatile int Encoder_Overflow_Count;

   

/***************************PID CHANNEL*******************************/
#define BASIC_TIM           		  TIM6
//PID计算周期:1ms计算一次--频率1KHz
#define BASIC_PERIOD_MS    (1)

#define BASIC_PERIOD_COUNT    (BASIC_PERIOD_MS*1000 - 1)

#define BASIC_PRESCALER_COUNT   (30)


void FocMotor_Init(void);
void FocEncoder_Init(void);
void FocBasicTim_Init(void);

void foc_stop_pwm_output(void);

void mc_angle_init(void);
int Get_Electrical_Angle(void);

#endif
