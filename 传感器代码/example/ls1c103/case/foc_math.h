#ifndef __FOC_MATH_H_
#define __FOC_MATH_H_

#include "stdint.h"
#include "foc.h"

/** @addtogroup MC_Math
  * @{
  */
#define SQRT_2  1.4142
#define SQRT_3  1.732

typedef struct 
{
	int16_t h_cos;
	int16_t h_sin;
} trig_components;


alphabeta_t mc_rev_park(qd_t volt_input, int16_t theta);
trig_components mc_trig_functions(int16_t h_angle);
qd_t circle_limitation(qd_t v_qd);





#endif
