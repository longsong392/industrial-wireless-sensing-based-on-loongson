#include "ls1x_common.h"
#include "ls1x.h"

INT8U IsGlobalIntOpen(void)
{
    unsigned int val = 0;
    asm volatile("csrrd  %0, 0x0":"=r" (val));

    return val & 0x4;
}

void DisableInt(void)
{
    asm volatile(\
                "li.w $r12, 0x4;\n"\
                "csrxchg $r0, $r12, 0x0;\n"\
                :::"$r12"
                );
}

void EnableInt(void)
{
    asm volatile(\
                "li.w $r12, 0x4;\n"\
                "csrxchg $r12, $r12, 0x0;\n"\
                :::"$r12"
                );
}

void Set_soft_int(void)
{
    asm volatile(\
                "li.w $r12, 0x1;\n"\
                "csrxchg $r12, $r12, 0x4;\n"\
                :::"$r12"
                );
}

void Set_soft_stop(void)
{
    asm volatile(\
                "li.w $r12, 0x1;\n"\
                "csrxchg $r0, $r12, 0x4;\n"\
                :::"$r12"
                );
}

void Set_Timer_stop(void)
{
    asm volatile (\
                  "csrwr $r0, 0x41;\n" \
                 );
}

void Set_Timer_clear(void)
{
    asm volatile (\
                  "li.w $r12, 0x1;\n" \
                  "csrwr $r12, 0x44;\n" \
                  :::"$r12"
                 );

}

void Set_Timer_Init(INT32U timer)
{
    asm volatile (\
                  "csrrd    $r12, 0x41;"\
                  "ori      $r12, %0, 0x3;"\
                  "csrwr    $r12, 0x41;"\
                  :
                  :"r"(timer)
                  :"$r12"
                 );
}

void Wake_Set(INT32U sec)
{
    if(sec != 0)
    {
        INT32U time = sec * 256;
        INT32U compare = 0;
        compare = (PMU_COUNT & 0xfffff) + time;
        PMU_COMPARE  = compare;
        PMU_CMDSTS |= (1 << 7) | (1 << 8);
    }
    else
    {
        PMU_CMDSTS &=  ~((1 << 7) | (1 << 8));
    }
}
void Wake_256sec_Set(INT32U p256sec)
{
    if(p256sec != 0)
    {
        INT32U time = p256sec ;
        INT32U compare = 0;
        compare = (PMU_COUNT & 0xfffff) + time;
        PMU_COMPARE  = compare;
        PMU_CMDSTS |= (1 << 7) | (1 << 8);
    }
    else
    {
        PMU_CMDSTS &=  ~((1 << 7) | (1 << 8));
    }
}
