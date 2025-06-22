#ifndef _LS1X_COMMON_H_
#define _LS1X_COMMON_H_
#include "test.h"

INT8U IsGlobalIntOpen(void);

void DisableInt(void);
void EnableInt(void);

void DisableExtInt(void);
void EnableExtInt(void);

void Set_soft_int(void);
void Set_soft_stop(void);

void Set_Timer_stop(void);
void Set_Timer_clear(void);
void Set_Timer_Init(INT32U timer);

void Wake_Set(INT32U sec);

#endif
