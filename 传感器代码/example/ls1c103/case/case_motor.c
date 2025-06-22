//#include "ls1c103.h"
#include "ls1x.h"
#include "ls1x_gpio.h"
#include "Config.h"
#include "test.h"
#include "ls1c103_adc.h"
#include "ls1x_gpio.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"
#include "ls1c103_bldc_tim.h"
#include "ls1c103_bldc_ctrl.h"
static int err;

#define KEY0 (0x00)
#define KEY1 (0x01)

extern unsigned int str2num(unsigned char *);
static void GPIO_Configuration(void)
{
  AFIO_RemapConfig(AFIOA, GPIO_Pin_12 | GPIO_Pin_13, GPIO_FUNC_GPIO);
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

static uint8_t scan_key(uint32_t key){
  uint16_t keyuse;
  if(key == KEY1){
    keyuse = GPIO_Pin_13;
  }
  else{
    keyuse = GPIO_Pin_12;
  }
  uint8_t sflag = GPIO_ReadInputDataBit(GPIOA, keyuse);
  if(sflag==0){
	while(sflag==0){
	       sflag = GPIO_ReadInputDataBit(GPIOA, keyuse);
	}
	return 1;
  }
  else{
	return 0;
  }
 
}



extern void irq_motor_init(int index);
extern void irq_motor_default(void);

int tim_motor_test(void)
{
  GPIO_Configuration();

  volatile uint32_t ChannelPulse = PWM_MAX_PERIOD_COUNT/8;
  int i = 0, sum;
  uint32_t myaddr = (uint32_t)(&MOTOR_TIM->CR1);


  printf("BLDC MOTOR TEST\n\r");
  printf("PWM addr define test:%x\n\r",myaddr);
  myaddr = (uint32_t)(&HALL_TIM->CR1);
  printf("Hall addr define test:%x\n\r",myaddr);
  bldcm_init();
	 
  irq_motor_default();
  irq_motor_init(1);
  printf("PWM Initial End!\n\r");

  while(1){
    //char c; 
   // c = x_getchar();
   // if (c == 0x3)    break;
    if(scan_key(KEY0)){ 
        ChannelPulse = PWM_MAX_PERIOD_COUNT/8;
        printf("START\n\r");
        set_bldcm_speed(ChannelPulse);
        set_bldcm_enable();
        break;
    }   
  }
  return 0;
}

int tim_motor(int argc, void *argv[])
{   
    printf("lbx----->>>\n\r");
    tim_motor_test();
    return 0;
}

int speed_set(int argc, void *argv[])
{   
    if (argc != 3)
    {
        printf("\n usage: speed set <dir:0/1> <speed:100-1800>\n");
        return 1;
    }

    uint32_t  dir_in = str2num(argv[1]);
    motor_dir_t dir;
    uint32_t  speed = str2num(argv[2]);
    if(speed > (3*PWM_MAX_PERIOD_COUNT)/4){
       speed = (3*PWM_MAX_PERIOD_COUNT)/4;
    }
    else if(speed < 100){
       speed = 100;
    }

    if(dir_in == 0){
      dir = MOTOR_FWD;
    }
    else{
      dir = MOTOR_REV;
    }


    set_bldcm_speed(speed);
    set_bldcm_direction(dir);

    return 0;
}

int speed_keyset(int argc, void *argv[])
{   
    if (argc != 2)
    {
        printf("\n usage: speed set <dir:0/1>\n");
        return 1;
    }

    uint32_t  dir_in = str2num(argv[1]);

    motor_dir_t dir;

    uint32_t ChannelPulse = PWM_MAX_PERIOD_COUNT/8;
    set_bldcm_speed(ChannelPulse);

    if(dir_in == 0){
      dir = MOTOR_FWD;
    }
    else{
      dir = MOTOR_REV;
    }

    set_bldcm_direction(dir);

    int i = 0;

    while(1){
      if(scan_key(KEY0)){ 
	        ChannelPulse +=  PWM_MAX_PERIOD_COUNT/20;
	        if(ChannelPulse > (3*PWM_MAX_PERIOD_COUNT)/4){
	          ChannelPulse = (3*PWM_MAX_PERIOD_COUNT)/4;
	        }    
	        set_bldcm_speed(ChannelPulse);
	        printf("acc\n\r");
	    }
	    else if (scan_key(KEY1)){ 
	        ChannelPulse -=  PWM_MAX_PERIOD_COUNT/20;
	        if(ChannelPulse < 100){
	          ChannelPulse = 100;
	        }
	        set_bldcm_speed(ChannelPulse);
	        printf("deacc\n\r");
	    }

      char c; 
      c = getchar();
      if (c == 0x3)    break;
      i++;
      if(i==100000){
	    i = 0;
	    printf(".");
	  }
  }


  return 0;
}   

int tim_set(int argc, void *argv[])
{   
    if (argc != 4)
    {
        printf("\n usage: tim set <0:atim/1:gtim> <psc> <arr>\n");
        return 1;
    }

    uint32_t  tim_choice = str2num(argv[1]);
    uint32_t  psc_in = str2num(argv[2]);
    uint32_t  arr_in = str2num(argv[2]);
    uint32_t  data;


    switch(tim_choice){
      case 1:  MOTOR_TIM->PSC = psc_in;  MOTOR_TIM->ARR = arr_in; break; 
      case 2:  HALL_TIM->PSC = psc_in;  HALL_TIM->ARR = arr_in; break; 
    }
    return 0;
}

