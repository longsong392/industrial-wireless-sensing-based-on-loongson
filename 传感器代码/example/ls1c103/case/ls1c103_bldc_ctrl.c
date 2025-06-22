#include "ls1c103_bldc_ctrl.h"
#include "math.h"
#include "stdlib.h"
#include "Config.h"

//bldc motor status
static bldcm_status_t bldcm_status;

void bldcm_init(void)
{
  Motor_Init();   
  Hall_Init();   
}


void set_bldcm_speed(uint16_t v)
{
  bldcm_status.dutyfactor = v;
  
  set_pwm_pulse(v);    
}

void set_bldcm_direction(motor_dir_t dir)
{
  bldcm_status.direction = dir;
  set_direction(dir);
}

motor_dir_t get_bldcm_direction(void)
{
  return bldcm_status.direction;
}


void set_bldcm_enable(void)
{
  MOTOR_TIM->BDTR|=(TIM_BDTR_MOE);
  MOTOR_TIM->CR1|=TIM_CR1_CEN;
  BLDCM_ENABLE_EN(); 
  hall_enable();
  printf("motor enable\n\r");
}

void set_bldcm_disable(void)
{
  hall_disable();
  
  stop_pwm_output();
  
  BLDCM_DISABLE_EN(); 
  printf("motor disable\n\r");
}
