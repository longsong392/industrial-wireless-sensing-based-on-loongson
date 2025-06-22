#include "Config.h"
#include "test.h"
#include "ls1x_common.h"
#include "ls1c103_tim.h"
#include "ls1c103_map.h"
#include "ls1c103_adc.h"
#include "ls1c103_bldc_tim.h"
#include "ls1c103_bldc_ctrl.h"
#include "ls1c103_dcm_tim.h"
#include "ls1x_exti.h"
#include "ls1x_wdg.h"
#include "foc.h"

/* Exported ------------------------------------------------------------------*/
/******************************************************************************/
extern void mc_ctrl(void);
extern motor_rotate_t motor_drive;    // 定义电机驱动管理结构体


/*                         User Function                                      */
/******************************************************************************/

/*------------------------ ext function --------------------------------------*/
#define	BEBUG_IRQ()  printf("\r\n <  line: %d func:%s   >\r\n",__LINE__,__FUNCTION__)
void (* const exti_irq_handle[32])(void);

void exti_gpioa0_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A0);
}
void exti_gpioa1_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A1);
}
void exti_gpioa2_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A2);
}
void exti_gpioa3_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A3);
}
void exti_gpioa4_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A4);
}
void exti_gpioa5_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A5);
}
void exti_gpioa6_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A6);
}
void exti_gpioa7_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A7);
}
void exti_gpioa8_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A8);
}
void exti_gpioa9_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A9);
}
void exti_gpioa10_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A10);
}
void exti_gpioa11_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A11);
}
void exti_gpioa12_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A12);
}
void exti_gpioa13_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A13 );
}
void exti_gpioa14_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A14);
}
void exti_gpioa15_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_A15 );
}

void exti_gpiob0_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B0);
}
void exti_gpiob1_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B1);
}
void exti_gpiob2_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B2);
}
void exti_gpiob3_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B3);
}
void exti_gpiob4_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B4);
}
void exti_gpiob5_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B5);
}
void exti_gpiob6_irq_handler(void)
{
	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B6);
}
void exti_gpiob7_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B7);
}
void exti_gpiob8_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B8);
}
void exti_gpiob9_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B9);
}
void exti_gpiob10_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B10);
}
void exti_gpiob11_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B11);
}
void exti_gpiob12_irq_handler(void)
{

	BEBUG_IRQ();
	EXTI_ClearITPendingBit(EXTI, IRQ_GPIO_B12);
}

void (* const Ext_IrqHandle[29])(void) = {
   exti_gpioa0_irq_handler,
   exti_gpioa1_irq_handler,
   exti_gpioa2_irq_handler,
   exti_gpioa3_irq_handler,
   exti_gpioa4_irq_handler,
   exti_gpioa5_irq_handler,
   exti_gpioa6_irq_handler,
   exti_gpioa7_irq_handler,
   exti_gpioa8_irq_handler,
   exti_gpioa9_irq_handler,
   exti_gpioa10_irq_handler,
   exti_gpioa11_irq_handler,
   exti_gpioa12_irq_handler,
   exti_gpioa13_irq_handler,
   exti_gpioa14_irq_handler,
   exti_gpioa15_irq_handler,
   exti_gpiob0_irq_handler,
   exti_gpiob1_irq_handler,
   exti_gpiob2_irq_handler,
   exti_gpiob3_irq_handler,
   exti_gpiob4_irq_handler,
   exti_gpiob5_irq_handler,
   exti_gpiob6_irq_handler,
   exti_gpiob7_irq_handler,
   exti_gpiob8_irq_handler,
   exti_gpiob9_irq_handler,
   exti_gpiob10_irq_handler,
   exti_gpiob11_irq_handler,
   exti_gpiob12_irq_handler,
};


/*------------------------ pmu function --------------------------------------*/


/*------------------------ intc function -------------------------------------*/


/*------------------------ pmu function --------------------------------------*/


/*------------------------ gtim function -------------------------------------*/
void bldc_handler(int vector,void *param){
  //  printf("%d, %s\n",vector,param);
      if ((HALL_TIM->SR &(TIM_IT_Trigger)) == (TIM_IT_Trigger)!= RESET)
      {
         HALL_TIM->SR &= ~(TIM_IT_Trigger);
        if ((((HALL_TIM->DIER & TIM_IT_Trigger) == (TIM_IT_Trigger)) ? SET : RESET)  != RESET)
        {
           HALL_TriggerCallback();
        }      
      }
      else if ((HALL_TIM->SR &(TIM_FLAG_Update)) == (TIM_FLAG_Update)!= RESET)
      {
         HALL_TIM->SR &= ~(TIM_IT_Update);
        if ((((HALL_TIM->DIER & TIM_IT_Update) == (TIM_IT_Update)) ? SET : RESET)  != RESET)
        {
          HALL_PeriodElapsedCallback();
        }     
      }
}

void dcm_handler(int vector, void *param){
 //   printf("%d, %s\n",vector,param);
 if ((ENCODER_TIM->SR &(TIM_FLAG_Update)) == (TIM_FLAG_Update)!= RESET)
  {
     ENCODER_TIM->SR &= ~(TIM_IT_Update);
    if ((((ENCODER_TIM->DIER & TIM_IT_Update) == (TIM_IT_Update)) ? SET : RESET)  != RESET)
    {
	  if((ENCODER_TIM->CR1 &(TIM_CR1_DIR)) == (TIM_CR1_DIR)){
	    /* 下溢 */
	    Encoder_Overflow_Count--;
	  }
	  else{
	    /* 上溢 */
	    Encoder_Overflow_Count++;
	  }
    }
  
  }
}

INT32U g_foc_counter = 0;
extern INT32U g_flags;
void foc_handler(int vector, void *param){
 if ((ENCODER_TIM->SR &(TIM_FLAG_Update)) == (TIM_FLAG_Update)!= RESET)
  {
     ENCODER_TIM->SR &= ~(TIM_IT_Update);
    if ((((ENCODER_TIM->DIER & TIM_IT_Update) == (TIM_IT_Update)) ? SET : RESET)  != RESET)
    {
          if(g_flags == 1)
          {
            g_foc_counter++;
          }
          else if(g_flags == 2)
          {
            g_foc_counter=0;
          }
    }
  
  }

}

typedef void (*isr_handler_t)(int vector, void *param);

struct irq_desc
{
    isr_handler_t handler;
    void *param;
};

struct irq_desc irq_handler_table[3];

void irq_default_handler(int vector, void *param)
{
        printf("Error !!! BLDC STOP\r\n");
        HALL_PeriodElapsedCallback();
}

isr_handler_t motor_interrupt_register(int vector,isr_handler_t handler, void *param)
{
    isr_handler_t old_handler = NULL;
    
    if(vector >=0 && vector < 3)
    {
        old_handler = irq_handler_table[vector].handler;
        irq_handler_table[vector].handler = handler;
        irq_handler_table[vector].param = param;
    }
    printf("%s index %d regsiter successful\n",param,vector);
    return old_handler;
}

static int g_index = 0;

extern void *memset(void *s, int c, int count);

void irq_motor_default(void)
{
    int idx;
    memset(irq_handler_table, 0x00, sizeof(irq_handler_table));
    for(idx=0; idx<3; idx++)
    {
        irq_handler_table[idx].handler = irq_default_handler;
    }

}

void irq_motor_init(int index)
{
    if(index == 1)
    {
        motor_interrupt_register(index,bldc_handler, "bldc_irq");
    }
    else if(index == 0)
    {
        motor_interrupt_register(index,dcm_handler, "dcm_irq");
    }
    else if(index == 2)
    {
        motor_interrupt_register(index,foc_handler, "foc_irq");
    }
    else if(index == 3)
    {
       printf("do nothing\r\n");
    }
    g_index = index;
}

void TIMER_HANDLER(void)
{
    Set_Timer_clear();
    printf("Core Timer clear interrupt..\n");
    Set_Timer_stop();
}

/*------------------------ atim function -------------------------------------*/



/*                         System Interrupt Handler                           */
/******************************************************************************/
/*------------------------ ext handler ---------------------------------------*/
void ext_handler(void)
{
	INT32U regsrc;// = EXTI->EXINT_SRC;
	INT32U regen;// = EXTI->EXINT_SRC;
	INT32U irq_no;
	BEBUG_IRQ();
	regsrc = EXTI->EXINT_SRC;
	regen = EXTI->EXINT_EN;
	regsrc = (regsrc & regen);
	asm("nop");

	for(irq_no = 0; irq_no < 32; irq_no++)
        {
		if((regsrc>>irq_no) & 0x1)
		{
		   Ext_IrqHandle[irq_no]();
		}
        }
}

/*------------------------ pmu handler ---------------------------------------*/
void pmu_handler()
{
    printf("\r\nThis is pmu_handler");
    if(PMU_CMDSTS & (1<<16))
    {
         printf("\n\rTIMER_WAKE_INT\n\r"); 
         PMU_CMDW = (1 << 16); // 清除中断标志
         //INT32U SleepEn = PMU_CMDSTS & 0xffffffff;
	 WDG_DogFeed();
    }
}

/*------------------------ intc handler --------------------------------------*/
void intc_handler(){
    //printf("\r\nThis is intc_handler!");
    if(INT_OUT & 0x1)
    {
      //  printf("\n\rBTIM_INT!"); 
        if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET){
            TIM_ClearFlag(TIM6, TIM_IT_Update);
            mc_ctrl();
        }
    }
}

/*------------------------ adc handler ---------------------------------------*/
void adc_handler(){
  uint16_t tmpreg[3];
  static int i = 0;
  if(ADC_GetITStatus(ADC1, ADC_IT_JEOC) == SET){
      ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);

  #if 0 
      if(i<1000){
    	i++;
      }
      else{
    	tmpreg[0] = ((ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1)*5000)>>12);
    	tmpreg[1] = ((ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2)*5000)>>12);
    	tmpreg[2] = ((ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_3)*5000)>>12);
    	//printf("U:%d,V:%d,W:%d\n\r",tmpreg[0],tmpreg[1],tmpreg[2]);
    	//printf("%d\n\r",tmpreg[0]);
    	i=0;
      }
  #else
    	printf("this is adc int\n\r");
  #endif
  }
}

/*------------------------ gtim handler --------------------------------------*/
void gtim_handler(void)
{   
    void *param;
    isr_handler_t irq_func = NULL;
    //printf("g_index:%d\r\n",g_index);
    irq_func = irq_handler_table[g_index].handler;
    param = irq_handler_table[g_index].param;
    irq_func(g_index,param);
}

/*------------------------ atim handler --------------------------------------*/
void atim_handler(){
  //if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET){
  //  TIM_ClearFlag(TIM1, TIM_IT_Update);
  //  printf("atim up int\n");
  //}
  if ((MOTOR_TIM->SR &(TIM_IT_Update)) == (TIM_IT_Update)!= RESET)
    {
      if ((((MOTOR_TIM->DIER & TIM_IT_Update) == (TIM_IT_Update)) ? SET : RESET)  != RESET)
	{
	  MOTOR_TIM->SR &= ~(TIM_IT_Update);
	  foc_update();
	}
    }
}

