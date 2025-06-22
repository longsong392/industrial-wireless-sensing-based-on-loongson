#ifndef _LS1X_PMU_H_
#define _LS1X_PMU_H_

#include "ls1x.h"

#define PMU_TURBO_DISABLE				       	(uint32_t)0x00000000
#define PMU_TURBO_ENABLE				       	(uint32_t)0x08000000
#define PMU_TURBO_MASK					        (uint32_t)0x08000000

#define PMU_RSTSRC_EXT					     	(uint32_t)0x00000000
#define PMU_RSTSRC_WDG					     	(uint32_t)0x04000000
#define PMU_RSTSRC_WDG1					    	(uint32_t)0x08000000
#define PMU_RSTSRC_WAKE				       		(uint32_t)0x0C000000
#define PMU_RSTSRC_MASK				       		(uint32_t)0x0C000000



typedef enum
{
    PMU_RSTSRC_ext = 0x0,
    PMU_RSTSRC_wdg,
    PMU_RSTSRC_wake
} PMU_RstSrc;

PMU_RstSrc PMU_GetRstRrc(void);
FlagStatus  PMU_GetBootSpiStatus(void);

#endif /* INCLUDE_POWER_H_ */



