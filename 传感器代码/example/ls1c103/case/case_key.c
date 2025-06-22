#include "ls1x.h"
#include "ls1x_gpio.h"
#include "test.h"
#include "Config.h"
#include "ls1x_gpio.h"
#include "ls1c103_dma.h"
#include "ls1c103_map.h"
#include "ls1x_exti.h"


static int err;
#define KEY0 (0x00)
#define KEY1 (0x01)

extern INT8U x_getchar(void);
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
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
  //printf("key:%d\n\r",key0);
  if(sflag==0){
	while(sflag==0){
	       // printf("lkey:%d\n\r",key0);
	       sflag = GPIO_ReadInputDataBit(GPIOA, keyuse);
	}
	return 1;
  }
  else{
	return 0;
  }
 
}



static int key_test(void)
{

  GPIO_Configuration();

  printf("\n\r");

  while(1){
    char c; 
    c = x_getchar();
    if (c == 0x3)    break;

    if(scan_key(KEY0)){
      printf("KEY0\n\r");
    }
    else if(scan_key(KEY1)){
      printf("KEY1\n\r");
    }
  }

  return 0;
}

static int key_int_test(void)
{
  GPIO_Configuration();
  EXTI_InitTypeDef exti_gpio;
  EXTI_StructInit(&exti_gpio);
  
  exti_gpio.EXTI_GPIO = IRQ_GPIO_A12|IRQ_GPIO_A13;/*GPIOA12,GPIOA13*/
  exti_gpio.EXTI_Mode = EXTI_Mode_Edge;
  exti_gpio.EXTI_Trigger = EXTI_Trigger_Rising_High;
  exti_gpio.EXTI_GpioCmd = ENABLE;
  
  EXTI_Init(EXTI, &exti_gpio);
}

int key (int argc, void *argv[])
{   
    key_test();
    return 0;
}

int key_int (int argc, void *argv[])
{   
    key_int_test();
    return 0;
}
