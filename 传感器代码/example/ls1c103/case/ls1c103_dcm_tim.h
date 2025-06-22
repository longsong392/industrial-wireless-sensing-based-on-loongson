#ifndef __LS1C103_DCM_TIM_H
#define	__LS1C103_DCM_TIM_H

#define KEY0 (0x00)
#define KEY1 (0x01)
#define DCM_TIM                TIM1
#define DCM_TIM2               TIM2
#define ENCODER_TIM            TIM2
#define DCM_PERIOD_COUNT        (1000)
#define DCM_PRESCALER_COUNT     (11)

extern volatile int Encoder_Overflow_Count;

/**************************ENCODER CHANNEL****************************/
/* 定时器选择 */
#define ENCODER_TIM                         TIM2
/* 定时器溢出值 */		
#define ENCODER_TIM_PERIOD                  (ENCODER_RESOLUTION * REDUCTION_RATIO)
/* 定时器预分频值 */
#define ENCODER_TIM_PRESCALER               0


#define ENCODER_RESOLUTION                  (16 * 4)

#define REDUCTION_RATIO                        30

typedef enum
{
  DCM_MOTOR_FWD = 0,
  DCM_MOTOR_REV,
}dcm_motor_dir_t;
typedef enum
{
  DCM_NONE = 0,
  DCM_FOUR,
  DCM_ENC,
}dcm_mode_t;

dcm_mode_t dcm_mode;

#endif 
