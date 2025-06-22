#ifndef _LS1C101_TOUCH_H_
#define _LS1C101_TOUCH_H_
//#include "ls1c101_exit.h"
#include "test.h"
/**
  * @brief  Touch_CountBaseInit Structure definition
  */
typedef struct
{
    INT8U TOUCH_up_th;	    	/*!< Specifies the Touch Control Count.*/


    INT8U TOUCH_bas_th;	    	/*!< Specifies the Touch Control Count.*/


    INT8U TOUCH_cnt_neg;		/*!< Specifies the Touch Control Count.
										This parameter can be: 0 or 1 */

    INT8U TOUCH_cnt_prd;		/*!< Specifies the Touch Control Count.
										This parameter can be a number between 0x0 and 0xF */

    INT8U TOUCH_rsel;			/*!< Specifies the Touch Control Count.
										This parameter can be a number between 0x0 and 0xD */

} Touch_CountBaseInitTypeDef;

/**
  * @brief  Touch_PerformInit Structure definition
  */
typedef struct
{
    INT8U TOUCH_dbc_prd;		/*!< Specifies the Touch Control Threshold.
										This parameter can be a number between 0x0 and 0xFF */

    INT8U TOUCH_act_stb;		/*!< Specifies the Touch Control of filter jitter time.
										This parameter can be a number between 0x0 and 0xFF */

    INT8U TOUCH_stb_prd;		/*!< Specifies the Touch Control of standby mode cycle.
										This parameter can be a number between 0x0 and 0xFF */

    INT8U TOUCH_act_prd;		/*!< Specifies the Touch Control of activation mode duration.
										This parameter can be a number between 0x0 and 0x3 */

    INT8U TOUCH_act_num;		/*!< Specifies the Touch Control of the activation Mode cycle.
										This parameter can be a number between 0x0 and 0xF */

} Touch_PerformInitTypeDef;

/**
  * @brief  Touch_ControlConfig Structure definition
  */
typedef struct
{
    INT16U TOUCH_chn_en;		/*!< Specifies the Touch Control of channel switch.
										This parameter can be a number between 0x0 and 0xFFFF */

    INT8U  TOUCH_dbc_en;		/*!< Specifies the Touch Control of filter jitter enable.
										This parameter can be a number between 0x0 and 0xFF */

    INT8U  TOUCH_dbc_num;		/*!< Specifies the Touch Control of filter jitter number.
										This parameter can be a number between 0x0 and 0xFF */

    INT8U  TOUCH_eos_ov;		/*!< Specifies the Touch Control of key_interrupt mode.
										This parameter can be: 0x1 or 0x2 or 0x4 or 0x8 */

    INT8U  TOUCH_flt_level;		/*!< Specifies the Touch Control of key_interrupt mode.
										This parameter can be: 0x1 or 0x2 or 0x4 or 0x8 */

    INT8U  TOUCH_int_en;		/*!< Specifies the Touch Control of key_interrupt mode.
										This parameter can be: 0x1 or 0x2 or 0x4 or 0x8 */

    INT8U  TOUCH_poll_en;		/*!< Specifies the Touch Control of scan-round enable.
										This parameter can be: 0 or 1  */

    INT8U  TOUCH_scan_en;		/*!< Specifies the Touch Control of scan-single enable.
										This parameter can be: 0 or 1  */

} Touch_ControlConfigTypeDef;

void TOUCH_EnablePollScan(void);
void TOUCH_DisablePollScan(void);
void TOUCH_EnableSingleScan(void);
void Touch_CountBaseInit(Touch_CountBaseInitTypeDef* Touch_CountBaseInitStruct);
void Touch_PerformInit(Touch_PerformInitTypeDef* Touch_PerformInitStruct);
void Touch_ControlConfig(Touch_ControlConfigTypeDef* Touch_ControlConfigStruct);
void TOUCH_Cmd(FunctionalState NewState);
void TOUCH_SetDownTh(INT8U channel, INT8U down_th);
void TOUCH_SetBaseVal(INT8U channel, INT16U base_val);
void TOUCH_SetChnAttr(INT8U channel, INT8U down_th);
void TOUCH_SetAllChnAttr2(INT8U down_th);
void TOUCH_SetAllChnAttr(INT8U down_th);
void TOUCH_Init(INT8U down_th);
void Printf_KeyChannel(INT16U KeyChannel);
void Printf_KeyType(INT8U KeyType);
void Printf_KeyVal(void);
// void Printf_CountVal(void);

INT16U TOUCH_GetCountValue(INT16U KeyChannel);
INT16U TOUCH_GetBaseVal(INT16U KeyChannel);

INT32U Touch_Self_Check(void) ;



#define CHANNEL_NUM         12
#define BIT(x)              (0x1 << x)
#define KEY_CHANNEL11       BIT(11)
#define KEY_CHANNEL10       BIT(10)
#define KEY_CHANNEL9        BIT(9)
#define KEY_CHANNEL8        BIT(8)
#define KEY_CHANNEL7        BIT(7)
#define KEY_CHANNEL6        BIT(6)
#define KEY_CHANNEL5        BIT(5)
#define KEY_CHANNEL4        BIT(4)
#define KEY_CHANNEL3        BIT(3)
#define KEY_CHANNEL2        BIT(2)
#define KEY_CHANNEL1        BIT(1)
#define KEY_CHANNEL0        BIT(0)

#endif

