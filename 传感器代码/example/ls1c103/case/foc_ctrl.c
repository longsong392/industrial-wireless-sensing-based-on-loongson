#include "Config.h"
#include "ls1x.h"
#include "foc.h"
#include "foc_ctrl.h"
#include "math.h"
#include "test.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"


/**
  * @brief  电机初始化
  * @param  无
  * @retval 无
  */
void foc_bldcm_init(void)
{
  FocMotor_Init();   // 电机控制定时器，引脚初始化
  FocEncoder_Init();    // 初始化
}

/**
  * @brief  使能电机
  * @param  无
  * @retval 无
  */
void foc_set_bldcm_enable(void)
{
  BASIC_TIM->DIER |= (TIM_IT_Update);
  BASIC_TIM->CR1  |=(TIM_CR1_CEN);
  MOTOR_TIM->DIER |= TIM_IT_Update;
  MOTOR_TIM->BDTR |= TIM_BDTR_MOE;
  MOTOR_TIM->CR1  |= TIM_CR1_CEN;
  ENCODER_TIM->CR1 |= (TIM_CR1_CEN);
  //BLDCM_ENABLE_SD();  //TODO
  mc_flag.ctrl_loop_mode = MC_ELEC_CALIB_MODE;
  printf("motor enable\n\r");
}

/**
  * @brief  禁用电机
  * @param  无
  * @retval 无
  */
void foc_set_bldcm_disable(void)
{  
  /* 停止 PWM 输出 */
  foc_stop_pwm_output();
  
  /* 关闭 MOS 管 */
  //BLDCM_DISABLE_SD();  //TODO
  myprintf("motor disable\n\r");
}

/**
  * @brief 此函数每10ms执行一次
  * @note  当电机处于电角度校准模式，进行电角度校准。
  */
static void mc_10ms_loop(void)
{
  static unsigned short cnt = 0;
  static short vq = 0;
  qd_t volt_set;
  if (mc_flag.ctrl_loop_mode == MC_ELEC_CALIB_MODE) {
    
    volt_set.q = vq;
    volt_set.d = 0;
    foc_v_qd_set(volt_set);
    cnt += 1;
    if (cnt < 60)
      vq += 100;
    
    if (cnt > 100) {
      vq = 3000;
      cnt = 0;	
      
      volt_set.q = vq;
      volt_set.d = 0;
      foc_v_qd_set(volt_set);
      mc_angle_init();
      mc_flag.ctrl_loop_mode = MC_TEST_MODE;
      //mc_flag.ctrl_loop_mode = MC_OPEN_MODE;
      
      myprintf("ELEC_CALIB Complete!\n\r");
    }
  } 
}

void mc_ctrl(void)
{
  static int i = 0;
  
  i++;
  
  // 1ms
  //mc_1ms_loop();
  
  // 10ms
   if (i % 10 == 0) {
    mc_10ms_loop();
   }
}
