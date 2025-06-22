#ifndef __MOTOR_CONTROL_H
#define	__MOTOR_CONTROL_H

#include "foc_tim.h"
#include "ls1x.h"
#include "ls1c103_map.h"
#include "test.h"
#include "ls1x_gpio.h"

/* 电机 SD or EN 使能脚 */   
#define BLDCM_ENABLE_SD()      GPIO_SetBits(GPIOA, GPIO_Pin_13)    
#define BLDCM_DISABLE_SD()     GPIO_ResetBits(GPIOA, GPIO_Pin_13)  

typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

void foc_bldcm_init(void);
void foc_set_bldcm_enable(void);
void foc_set_bldcm_disable(void);

void mc_ctrl(void);

#endif

