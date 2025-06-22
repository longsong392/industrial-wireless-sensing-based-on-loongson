#ifndef __FOC_H_
#define __FOC_H_

/* 电机控旋转实现结构体 */

#define SPEED_FILTER_NUM      30    // 速度滤波次数
typedef struct
{
  int timeout;            // 定时器更新计数
  float speed;                // 电机速度 rps（转/秒）
  int enable_flag;        // 电机使能标志
  int speed_group[SPEED_FILTER_NUM];
}motor_rotate_t;



/**
  * @brief Two components q, d type definition
  */
typedef struct
{
  int q;
  int d;
} qd_t;
/**
  * @brief Two components a,b type definition
  */
typedef struct
{
  int a;
  int b;
} ab_t;
/**
  * @brief Two components alpha, beta type definition
  */
typedef struct
{
  int alpha;
  int beta;
} alphabeta_t;

typedef enum {
    MC_NO_CTRL_MODE        = 0x00,
    MC_OPEN_MODE	   = 0x01,
    MC_ELEC_CALIB_MODE     = 0x02,
    MC_TEST_MODE           = 0x03,
    MC_SPD_CLOSE_LOOP_MODE = 0x04,
    MC_POS_CLOSE_LOOP_MODE = 0x05,
} mc_ctrl_loop_mode_e;

typedef struct {
    mc_ctrl_loop_mode_e ctrl_loop_mode;
} mc_flag_t;

extern mc_flag_t mc_flag;


#define UINT15_MAX 32768
#define UINT32_MAX 0xFFFFFFFF

#define ANGLE_0             ((int)(0))
#define ANGLE_90            ((int)(65536 * 90 / 360))
#define ANGLE_180           ((int)(65536 * 180 / 360))


#define PWM_FREQ	(22000) 	// 5 k

#define PWM_PERIOD_CYCLES (uint32_t)(11*(unsigned long long)1000000u/((uint32_t)(PWM_FREQ)))
									  
#define SQRT3FACTOR (uint32_t) 0xDDB4 /* = (16384 * 1.732051 * 2)*/  // 根号3的q15格式表示？

#define hT_Sqrt3 ((PWM_PERIOD_CYCLES*SQRT3FACTOR)/16384u)

void svpwm_calc(alphabeta_t v_alpha_beta);
void foc_v_qd_set(qd_t set);
void foc_update(void);





#endif
