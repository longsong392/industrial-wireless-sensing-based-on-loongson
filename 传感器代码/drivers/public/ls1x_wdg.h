#ifndef _LS1X_WDG_H_
#define _LS1X_WDG_H_

#include "ls1x.h"

void WDG_SetWatchDog(uint32_t time);
void WDG_DogFeed(void);
void WdgInit(void);

#endif
