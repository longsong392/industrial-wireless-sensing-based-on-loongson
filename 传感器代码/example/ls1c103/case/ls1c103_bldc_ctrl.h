#ifndef __LS1C103_BLDC_CTRL_H__
#define	__LS1C103_BLDC_CTRL_H__

#include "ls1x_gpio.h"
#include "ls1c103_map.h"
#include "ls1c103_bldc_tim.h"
//#include "ls1c103.h"
#include "ls1x.h"
#include "test.h"


/* Motor Driver Enable/Disable */ 
#define BLDCM_ENABLE_EN()      GPIO_SetBits(GPIOA, GPIO_Pin_10)    
#define BLDCM_DISABLE_EN()     GPIO_ResetBits(GPIOA, GPIO_Pin_10)  



typedef struct
{
  motor_dir_t direction;    
  uint32_t dutyfactor;      
  uint8_t is_enable;        
}bldcm_status_t;

void bldcm_init(void);
void set_bldcm_speed(uint16_t v);
void set_bldcm_direction(motor_dir_t dir);
motor_dir_t get_bldcm_direction(void);
void set_bldcm_enable(void);
void set_bldcm_disable(void);

#endif 
