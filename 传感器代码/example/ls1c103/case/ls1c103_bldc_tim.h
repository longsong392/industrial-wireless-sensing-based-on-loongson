#ifndef __LS1C103_BLDC_TIM_H_
#define __LS1C103_BLDC_TIM_H_

#include "test.h"

#include "ls1c103_map.h"

/***************************PWM CHANNEL*******************************/

#define MOTOR_TIM                 TIM1

#define PWM_PERIOD_COUNT        (2400)
#define PWM_MAX_PERIOD_COUNT    (PWM_PERIOD_COUNT - 100)
#define PWM_PRESCALER_COUNT     (3)

#define TIM_COM_TS_ITRx         TIM_TS_ITR3    // (TIM4->TRGO->TIM1->ITR3)

/**************************HALL CHANNEL****************************/

#define HALL_TIM                            TIM2	
#define HALL_PERIOD_COUNT                   (0xFFFF)
#define HALL_PRESCALER_COUNT                (110)     

/***************************PID CHANNEL*******************************/
#define BASIC_TIM           		  TIM6

#define BASIC_PERIOD_MS    (4)

#define BASIC_PERIOD_COUNT    (BASIC_PERIOD_MS*100 - 1)

#define BASIC_PRESCALER_COUNT   (71)

typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

void Motor_Init(void);
void Hall_Init(void);
void BasicTim_Init(void);

void set_pwm_pulse(uint32_t pulse);
void set_direction(motor_dir_t dir);
void stop_pwm_output(void);

void hall_enable(void);
void hall_disable(void);

void HALL_TriggerCallback(void);
void HALL_PeriodElapsedCallback(void);

#endif
