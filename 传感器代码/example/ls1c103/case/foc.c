#include "Config.h"
#include "foc.h"
#include "foc_math.h"
#include "foc_tim.h"

#define SECTOR_1  0u
#define SECTOR_2  1u
#define SECTOR_3  2u
#define SECTOR_4  3u
#define SECTOR_5  4u
#define SECTOR_6  5u

/**
  * @brief 根据v_alpha_beta计算svpwm
  */
void svpwm_calc(alphabeta_t v_alpha_beta)
{
  int32_t w_x, w_y, w_z, w_u_alpha, w_u_beta, w_time_ph_a, w_time_ph_b, w_time_ph_c;
  
  uint8_t sector;
  
  w_u_alpha = v_alpha_beta.alpha * (int32_t)hT_Sqrt3;
  w_u_beta = -(v_alpha_beta.beta * ( int32_t )(PWM_PERIOD_CYCLES) ) * 2;
  
  w_x = w_u_beta;
  w_y = ( w_u_beta + w_u_alpha ) / 2;
  w_z = ( w_u_beta - w_u_alpha ) / 2;
  
  /* Sector calculation from w_x, w_y, w_z */
  if (w_y < 0) {
    if (w_z < 0) {
	sector = SECTOR_2;
	w_time_ph_a = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( w_y - w_z ) / ( int32_t )262144 );
	w_time_ph_b = w_time_ph_a + w_z / 131072;
	w_time_ph_c = w_time_ph_a - w_y / 131072;
	//pSetADCSamplingPoint = pHandle->pFctSetADCSampPointSect2;
    } else if (w_x <= 0) { /* w_z >= 0 */
      sector = SECTOR_4;
      w_time_ph_a = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( w_x - w_z ) / ( int32_t )262144 );
      w_time_ph_b = w_time_ph_a + w_z / 131072;
      w_time_ph_c = w_time_ph_b - w_x / 131072;
      //pSetADCSamplingPoint = pHandle->pFctSetADCSampPointSect4;
    } else { // w_x > 0
      sector = SECTOR_3;
      w_time_ph_a = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( w_y - w_x ) / ( int32_t )262144 );
      w_time_ph_c = w_time_ph_a - w_y / 131072;
      w_time_ph_b = w_time_ph_c + w_x / 131072;
      //pSetADCSamplingPoint = pHandle->pFctSetADCSampPointSect3;
    }
  } else { // w_y > 0
    if ( w_z >= 0 )
      {
      sector = SECTOR_5;
      w_time_ph_a =  ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( w_y - w_z ) / ( int32_t )262144 );
      w_time_ph_b = w_time_ph_a + w_z / 131072;
      w_time_ph_c = w_time_ph_a - w_y / 131072;
      //pSetADCSamplingPoint = pHandle->pFctSetADCSampPointSect5;
      }
    else /* w_z < 0 */
      if ( w_x <= 0 )
	{
	  sector = SECTOR_6;
	  w_time_ph_a = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( w_y - w_x ) / ( int32_t )262144 );
	  w_time_ph_c = w_time_ph_a - w_y / 131072;
	  w_time_ph_b = w_time_ph_c + w_x / 131072;
	  //pSetADCSamplingPoint = pHandle->pFctSetADCSampPointSect6;
	}
      else /* w_x > 0 */
	{
	  sector = SECTOR_1;
	  w_time_ph_a = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( w_x - w_z ) / ( int32_t )262144 );
	  w_time_ph_b = w_time_ph_a + w_z / 131072;
	  w_time_ph_c = w_time_ph_b - w_x / 131072;
	  //pSetADCSamplingPoint = pHandle->pFctSetADCSampPointSect1;
	}
  }
  
  MOTOR_TIM->CCR1 = ( uint16_t )w_time_ph_a;
  MOTOR_TIM->CCR2 = ( uint16_t )w_time_ph_b;
  MOTOR_TIM->CCR3 = ( uint16_t )w_time_ph_c;
  //myprintf("%d %d %d\n\r",MOTOR_TIM->CCR1,MOTOR_TIM->CCR2,MOTOR_TIM->CCR3);
}

qd_t v_qd_set;

void foc_v_qd_set(qd_t set)
{
    v_qd_set.q = set.q;
    v_qd_set.d = set.d;
}

static int16_t encoder_elec_test = ANGLE_0;

mc_flag_t mc_flag = {
.ctrl_loop_mode = MC_OPEN_MODE,
};


/**
 * @brief 此函数每次定时器1中断执行一次，主要根据v_qd_set计算v_alpha_beta
  */
void foc_update(void)
{
  qd_t v_qd;
  alphabeta_t v_alpha_beta;

  int16_t Eangle;
  // 读取电角度
  Eangle = Get_Electrical_Angle();
  if (mc_flag.ctrl_loop_mode == MC_OPEN_MODE) {
    /*
        当电机处于开环模式，直接设置vq=3000，vd=0。让电机缓慢转动，
        此模式可用于测量电机本体或者驱动芯片是否正常
    */
    encoder_elec_test += 300;
    qd_t volt;
    volt.q = 7000;
    volt.d = 0;
    
    v_qd = circle_limitation(volt);   // 空间电压矢量限幅，防止超过六边形内接圆
    v_alpha_beta = mc_rev_park(v_qd, encoder_elec_test);
    //myprintf("o:%d\n\r",encoder_elec_test);
  } else if (mc_flag.ctrl_loop_mode == MC_ELEC_CALIB_MODE) { // 电角度校准
    /*
        当电机处于电角度校准模式，将电角度固定为-90度。电角度校准详情请参考文档
    */
    
    v_qd = circle_limitation(v_qd_set);
    v_alpha_beta = mc_rev_park(v_qd, -ANGLE_90);
    
  } else {
    // 当电机处于速度闭环模式，位置闭环模式，或者测试模式进入此分支
    
    if (mc_flag.ctrl_loop_mode == MC_TEST_MODE) { 
      /*
	当电机处于测试模式，由CMDLINE设置目标速度
      */            
    }
    
    // 对设置的v_qd进行限幅，得到可正常输出的v_qd    
    v_qd = circle_limitation(v_qd_set);

    // 反park变换，得到v_alpha_beta
    v_alpha_beta = mc_rev_park(v_qd, Eangle);
  }
  
  // 根据v_alpha_beta输出svpwm波，驱动电机运动
  svpwm_calc(v_alpha_beta);
  
}



