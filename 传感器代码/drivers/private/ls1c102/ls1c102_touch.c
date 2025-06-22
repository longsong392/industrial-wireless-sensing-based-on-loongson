//#include"Main.h"
#include "ls1c102_touch.h"
#include "Config.h"
#include "ls1x.h"
void Touch_CountBaseInit(Touch_CountBaseInitTypeDef* Touch_CountBaseInitStruct)
{

    TS_OSCTH = ((Touch_CountBaseInitStruct->TOUCH_up_th&0xff)<<24)|\
               ((Touch_CountBaseInitStruct->TOUCH_bas_th&0xff)<<16)|\
               ((Touch_CountBaseInitStruct->TOUCH_cnt_neg&0x01)<<15)|\
               ((Touch_CountBaseInitStruct->TOUCH_cnt_prd&0x1f)<<8) |\
               ((Touch_CountBaseInitStruct->TOUCH_rsel&0x0f)<<0);
}
void Touch_PerformInit(Touch_PerformInitTypeDef* Touch_PerformInitStruct)
{

    TS_POLLTIM   = ((Touch_PerformInitStruct->TOUCH_dbc_prd&0xff)<<24)|\
                   ((Touch_PerformInitStruct->TOUCH_act_stb&0x3)<<22)|\
                   ((Touch_PerformInitStruct->TOUCH_stb_prd&0x3f)<<16)|\
                   ((Touch_PerformInitStruct->TOUCH_act_prd&0x0f)<<8)|\
                   ((Touch_PerformInitStruct->TOUCH_act_num&0xff)<<0);
}
void Touch_ControlConfig(Touch_ControlConfigTypeDef* Touch_ControlConfigStruct)
{
    TS_CTRL = ((Touch_ControlConfigStruct->TOUCH_chn_en & 0xffff)<<16)|\
              ((Touch_ControlConfigStruct->TOUCH_dbc_en & 0x1)<<15)|\
              ((Touch_ControlConfigStruct->TOUCH_dbc_num & 0x7)<<12)|\
              ((Touch_ControlConfigStruct->TOUCH_eos_ov & 0x1)<<11)|\
              ((Touch_ControlConfigStruct->TOUCH_flt_level & 0x03)<<8)|\
              ((Touch_ControlConfigStruct->TOUCH_int_en & 0x0f)<<4)|\
              ((Touch_ControlConfigStruct->TOUCH_poll_en & 0x1)<<1)|\
              ((Touch_ControlConfigStruct->TOUCH_scan_en & 0x1)<<0);
}

void TOUCH_Cmd(FunctionalState NewState)
{

    if (NewState != DISABLE)
    {
        PMU_CMDSTS   |= (1<<9);                       //触摸按键中断使能
    }
    else
    {
        PMU_CMDSTS &= ~(((INT32U)0x01) << 9);
    }
}

INT16U TOUCH_GetBaseVal(INT16U KeyChannel)
{
    return (TS_CHNATTR(KeyChannel) & 0xFFF);
}

INT16U TOUCH_GetCountValue(INT16U KeyChannel)
{
    return (TS_CNTRES(KeyChannel) & 0xFFF);
}

void TOUCH_EnablePollScan(void)
{
    TS_CTRL |= 0x2;
}

void TOUCH_disablePollScan(void)
{
    TS_CTRL &= ~(0x2);
}

void TOUCH_EnableSingleScan(void)
{
    TS_CTRL |= 0x1;
}

INT32U TOUCH_GetDownTh(INT8U channel)
{
    return  ( (TS_CHNATTR(channel) >>24)& 0xff) ;
}

void TOUCH_SetDownTh(INT8U channel, INT8U down_th)
{
    TS_CHNATTR(channel) = ( (TS_CHNATTR(channel) & 0xfff) | (down_th << 24) );
}

void TOUCH_SetBaseVal(INT8U channel, INT16U base_val)
{
    TS_CHNATTR(channel) = ( (TS_CHNATTR(channel) & 0xff000000) | (base_val & 0xfff) );
}

void TOUCH_SetChnAttr(INT8U channel, INT8U down_th)
{
    TS_CHNATTR(channel) = ( down_th << 24) | (TS_CNTRES(channel) & 0xfff);
}

void TOUCH_SetAllChnAttr(INT8U down_th)
{
    INT16U KeyChannel;
    for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
    {
        TOUCH_SetChnAttr(KeyChannel, down_th);
    }
}

void TOUCH_SetAllChnAttr2(INT8U down_th)
{
    INT16U KeyChannel;
    for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
    {
        TS_CHNATTR(KeyChannel) = ( down_th << 24) | (TS_CHNATTR(KeyChannel) & 0xfff);
    }
}

void TOUCH_Init(INT8U down_th)
{
    Touch_CountBaseInitTypeDef Touch_CountBaseInitStruct;
    Touch_PerformInitTypeDef   Touch_PerformInitStruct;
    Touch_ControlConfigTypeDef Touch_ControlConfigStruct;

    Touch_CountBaseInitStruct.TOUCH_up_th   = 0x14;
    Touch_CountBaseInitStruct.TOUCH_bas_th  = 0x10;
    Touch_CountBaseInitStruct.TOUCH_cnt_neg = 1;
    Touch_CountBaseInitStruct.TOUCH_cnt_prd = 0x1;
    Touch_CountBaseInitStruct.TOUCH_rsel    = 0x1;
    Touch_CountBaseInit(&Touch_CountBaseInitStruct);

    Touch_PerformInitStruct.TOUCH_dbc_prd = 0x3;
    Touch_PerformInitStruct.TOUCH_act_stb = 0x0;
    Touch_PerformInitStruct.TOUCH_stb_prd = 0x8;
    Touch_PerformInitStruct.TOUCH_act_prd = 0x8;
    Touch_PerformInitStruct.TOUCH_act_num = 0xff;
    Touch_PerformInit(&Touch_PerformInitStruct);

    Touch_ControlConfigStruct.TOUCH_chn_en    = 0xfff;
    Touch_ControlConfigStruct.TOUCH_dbc_en    = 0x1;
    Touch_ControlConfigStruct.TOUCH_dbc_num   = 0x3;
    Touch_ControlConfigStruct.TOUCH_eos_ov    = 0x0;
    Touch_ControlConfigStruct.TOUCH_flt_level = 0x3;
    Touch_ControlConfigStruct.TOUCH_int_en    = 0xf;
    Touch_ControlConfigStruct.TOUCH_poll_en   = 0x0;
    Touch_ControlConfigStruct.TOUCH_scan_en   = 0x0;
    Touch_ControlConfig(&Touch_ControlConfigStruct);

    TOUCH_EnableSingleScan();
    while(TS_CTRL & 0x1);  //wait scan end
    TOUCH_SetAllChnAttr(down_th);

    TOUCH_Cmd(ENABLE);
}

void Printf_KeyChannel(INT16U KeyChannel)
{
    INT32U i;
    for(i=0; i<CHANNEL_NUM; i++)
    {
        if( KeyChannel & BIT(i))
            printf("\t%d", i);
    }
    printf("\n");
}

void  Printf_KeyType(INT8U KeyType)
{
    if(KeyType & BIT(0))
        printf("\r\nDown");
    if(KeyType & BIT(1))
        printf("\r\nUP");
    if(KeyType & BIT(2))
        printf("\r\nMuliDown");
    if(KeyType & BIT(3))
        printf("\r\nCount OV");
}

void Printf_KeyVal(void)
{
    INT32S KeyChannel,  SubVal[CHANNEL_NUM], BaseVal[CHANNEL_NUM], CountVal[CHANNEL_NUM];
    for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
    {
        CountVal[KeyChannel] = TOUCH_GetCountValue(KeyChannel);
        BaseVal[KeyChannel] = TOUCH_GetBaseVal(KeyChannel);

        if(BaseVal[KeyChannel])
        {
            SubVal[KeyChannel] = BaseVal[KeyChannel] - CountVal[KeyChannel];
            // Ratio[KeyChannel] = 1000*SubVal[KeyChannel]/BaseVal[KeyChannel];
        }
        else
        {
            SubVal[KeyChannel] = 0;
            //  Ratio[KeyChannel] = 0;
        }
    }

    printf("\r\n\t00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t10\t11");
    printf("\r\ncnt");
    for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
    {
        printf("\t%4d", CountVal[KeyChannel]);
    }
    printf("\r\nBaseVal");
    for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
    {
        printf("\t%4d", BaseVal[KeyChannel]);
    }
    printf("\r\nSubVal");
    for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
    {
        if (SubVal[KeyChannel]<0)
            printf("\t%2d", SubVal[KeyChannel]);
        else
            printf("\t%3d", SubVal[KeyChannel]);
    }
    printf("\n");
}

// void Printf_CountVal(void)
// {
//     INT32S KeyChannel, CountVal[CHANNEL_NUM];
//     for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
//     {
//         CountVal[KeyChannel] = TOUCH_GetCountValue(KeyChannel);
//     }

// //    printf("\r\n\t00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t10\t11");
// //    printf("\nCntVal");
//     printf("\n");
//     for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
//     {
//         printf("\t%4d", CountVal[KeyChannel]);
//     }
// //    printf(" ---sta: %x",TS_STAT& 0x7);
// //    printf("\n");
// }

// void Printf_KeyDownth(void)
// {
//     INT32S KeyChannel, keydown[CHANNEL_NUM];
//     for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
//     {
//     	keydown[KeyChannel] = TOUCH_GetDownTh(KeyChannel);
//     }

// //    printf("\r\n\t00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t10\t11");
//     printf("\nKeyDown");
//     for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
//     {
//         printf("\t%d", keydown[KeyChannel]);
//     }
// }

// void TOUCH_get_basov()
// {
//     INT32S KeyChannel, CountVal[CHANNEL_NUM];
//     int k=0;
//     for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
//     {
//     	if((TS_CNTRES(KeyChannel)>>30) & 0x1)
//     	{
//     		//基线溢出，更新基准
//     	    printf("#basov.",KeyChannel);
//     	    k=1;
//     	}
//     }
//     if(k){
// 		printf("\r\nBaseVal");
// 		for(KeyChannel=0; KeyChannel<CHANNEL_NUM; KeyChannel++)
// 		{
// 		  printf("\t%4d", TOUCH_GetBaseVal(KeyChannel));
// 		}
//     }
// }

