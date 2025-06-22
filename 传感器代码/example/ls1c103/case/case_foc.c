#include "Config.h"
#include "ls1x.h"
#include "test.h"
#include "ls1x_gpio.h"
#include "ls1x_uart.h"
#include "ls1c103_map.h"
#include "ls1c103_tim.h"
#include "ls1x_latimer.h"
#include "foc_tim.h"
#include "foc_ctrl.h"
#include "foc.h"
static int err;

extern unsigned int str2num(unsigned char *);
static motor_dir_t direction;     // dir
static uint16_t    dutyfactor;             // speed

extern void irq_motor_init(int index);
static int tim_foc_test(void)
{

  volatile int ChannelQ = 4000;
  int i = 0 , sum;
  printf("\n\rSimple FOC TEST\n\r");
	 
  irq_motor_init(2);
  foc_bldcm_init();
  FocBasicTim_Init();
  
  qd_t my_qd_set={0};

  my_qd_set.q = ChannelQ;
  foc_v_qd_set(my_qd_set);
  /* 使能电机 */
  foc_set_bldcm_enable();
  return 0;
}

int tim_foc(int argc, void *argv[])
{   
    tim_foc_test();
    return 0;
}


#include <time.h>
typedef clock_t MTCORE_TICKS;
INT32U start_timer = 0;
INT32U end_timer = 0;

#define MTCPU_RAM_FREQ       (32767000)
#define MTAPB_DIV_S	         (MTCPU_RAM_FREQ / 3)
#define MTEE_TICKS_PER_SEC   (MTAPB_DIV_S)
INT32U mttime_in_secs(MTCORE_TICKS ticks)
{
    INT32U retval = ((INT32U)ticks) / (INT32U)MTEE_TICKS_PER_SEC;
    return retval;
}

extern INT32U g_foc_counter;
INT32U g_flags  = 0;
int foc_speed_test(int argc, void *argv[])
{   
    if (argc != 2)
    {
        printf("\n usage: speed test <0:end 1:begin 2:clr>\n");
        return 1;
    }
    MTCORE_TICKS   total_time;
    uint32_t n1 = 0,n2 = 0;
    uint32_t  code_in = str2num(argv[1]);

    if(code_in == 1)
    {
	open_count();
        asm volatile("rdtimel.w	%0, $r0":"=r" (start_timer));
        g_flags = 1;
    }
    else if(code_in == 0)
    {
        asm volatile("rdtimel.w	%0, $r0":"=r" (end_timer));
	close_count();
        g_flags = 0;
        total_time = end_timer - start_timer;
        n1 = g_foc_counter / mttime_in_secs(total_time);
        n2 = g_foc_counter / (mttime_in_secs(total_time) * 60);
        printf("start= 0x%x, %d\r\n", start_timer, start_timer);
        printf("stop= 0x%x, %d\r\n", end_timer, end_timer);     
        printf("total= 0x%x, %d\r\n", total_time, total_time);     
        printf("counter = %d\r\n", g_foc_counter);     
        printf("sec = %d\r\n", total_time/10667000);     
       // printf("timer sec = %d\r\n", mttime_in_secs(total_time));     
        printf("n1 = %d/sec\r\n", n1);     
       // printf("n2 = %d/min\r\n", n2);     
       // printf("Test done \r\n");
    }
    else{
	g_flags = 2;    
    }
    return 0;
}

int foc_speed_set(int argc, void *argv[])
{   
    if (argc != 2)
    {
        printf("\n usage: speed set <par:100-32767>\n");
        return 1;
    }
    uint32_t  speed = str2num(argv[1]);
    if(speed > 32767){
       speed = 32767;
    }
    else if(speed < 100){
       speed = 100;
    }

    qd_t my_qd_set1={0};

    my_qd_set1.q = speed;
    foc_v_qd_set(my_qd_set1);
    return 0;
}





