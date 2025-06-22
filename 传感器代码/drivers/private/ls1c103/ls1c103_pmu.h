#ifndef __LS1C103_PMU_H__
#define __LS1C103_PMU_H__
#include "test.h"
//#include "ls1c103_type.h"

uint32_t pmu_get_time();
void pmu_wake_timer_start(uint32_t delta);
void pmu_wake_timer_stop();
void pmu_watchdog_feed();
void pmu_watchdog_set(uint32_t delay);
uint32_t wait_int();
uint32_t wait_cpu_int();
void pmu_rst_cmd(uint32_t PMU_RstArr, uint32_t NewState);
void pmu_clk_cmd(uint32_t PMU_ClkArr, uint32_t NewState);

//void pmu_gpio_bit_write(int idx, uint32_t flag);
//uint32_t pmu_gpio_bit_read(int idx);

//void adc_start(uint32_t chn);
//void adc_read();

#define RCC_FLASH              ((uint32_t)0x00000001)
#define RCC_SPI                ((uint32_t)0x00000002)
#define RCC_DMA                ((uint32_t)0x00000004)
#define RCC_UART0              ((uint32_t)0x00000008)
#define RCC_UART1              ((uint32_t)0x00000010)
#define RCC_I2C                ((uint32_t)0x00000020)
#define RCC_BTIM               ((uint32_t)0x00000040)
#define RCC_GTIM               ((uint32_t)0x00000080)
#define RCC_ATIM               ((uint32_t)0x00000100)
#define RCC_CRC                ((uint32_t)0x00000200)
#define RCC_ADC                ((uint32_t)0x00000400)

#define RCC_AFIO               ((uint32_t)0x00010000)
#define RCC_GPIOA              ((uint32_t)0x00020000)
#define RCC_GPIOB              ((uint32_t)0x00040000)


#define PMU_COUNT_ONE_SECOND 0x100

#endif

