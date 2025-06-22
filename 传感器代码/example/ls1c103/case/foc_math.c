#include "foc.h"
#include "foc_math.h"

/* Private macro -------------------------------------------------------------*/
// sin(0-90度)分为256份，最大值为32766/32768
#define SIN_COS_TABLE {\
    0x0000,0x00C9,0x0192,0x025B,0x0324,0x03ED,0x04B6,0x057F,\
    0x0648,0x0711,0x07D9,0x08A2,0x096A,0x0A33,0x0AFB,0x0BC4,\
    0x0C8C,0x0D54,0x0E1C,0x0EE3,0x0FAB,0x1072,0x113A,0x1201,\
    0x12C8,0x138F,0x1455,0x151C,0x15E2,0x16A8,0x176E,0x1833,\
    0x18F9,0x19BE,0x1A82,0x1B47,0x1C0B,0x1CCF,0x1D93,0x1E57,\
    0x1F1A,0x1FDD,0x209F,0x2161,0x2223,0x22E5,0x23A6,0x2467,\
    0x2528,0x25E8,0x26A8,0x2767,0x2826,0x28E5,0x29A3,0x2A61,\
    0x2B1F,0x2BDC,0x2C99,0x2D55,0x2E11,0x2ECC,0x2F87,0x3041,\
    0x30FB,0x31B5,0x326E,0x3326,0x33DF,0x3496,0x354D,0x3604,\
    0x36BA,0x376F,0x3824,0x38D9,0x398C,0x3A40,0x3AF2,0x3BA5,\
    0x3C56,0x3D07,0x3DB8,0x3E68,0x3F17,0x3FC5,0x4073,0x4121,\
    0x41CE,0x427A,0x4325,0x43D0,0x447A,0x4524,0x45CD,0x4675,\
    0x471C,0x47C3,0x4869,0x490F,0x49B4,0x4A58,0x4AFB,0x4B9D,\
    0x4C3F,0x4CE0,0x4D81,0x4E20,0x4EBF,0x4F5D,0x4FFB,0x5097,\
    0x5133,0x51CE,0x5268,0x5302,0x539B,0x5432,0x54C9,0x5560,\
    0x55F5,0x568A,0x571D,0x57B0,0x5842,0x58D3,0x5964,0x59F3,\
    0x5A82,0x5B0F,0x5B9C,0x5C28,0x5CB3,0x5D3E,0x5DC7,0x5E4F,\
    0x5ED7,0x5F5D,0x5FE3,0x6068,0x60EB,0x616E,0x61F0,0x6271,\
    0x62F1,0x6370,0x63EE,0x646C,0x64E8,0x6563,0x65DD,0x6656,\
    0x66CF,0x6746,0x67BC,0x6832,0x68A6,0x6919,0x698B,0x69FD,\
    0x6A6D,0x6ADC,0x6B4A,0x6BB7,0x6C23,0x6C8E,0x6CF8,0x6D61,\
    0x6DC9,0x6E30,0x6E96,0x6EFB,0x6F5E,0x6FC1,0x7022,0x7083,\
    0x70E2,0x7140,0x719D,0x71F9,0x7254,0x72AE,0x7307,0x735E,\
    0x73B5,0x740A,0x745F,0x74B2,0x7504,0x7555,0x75A5,0x75F3,\
    0x7641,0x768D,0x76D8,0x7722,0x776B,0x77B3,0x77FA,0x783F,\
    0x7884,0x78C7,0x7909,0x794A,0x7989,0x79C8,0x7A05,0x7A41,\
    0x7A7C,0x7AB6,0x7AEE,0x7B26,0x7B5C,0x7B91,0x7BC5,0x7BF8,\
    0x7C29,0x7C59,0x7C88,0x7CB6,0x7CE3,0x7D0E,0x7D39,0x7D62,\
    0x7D89,0x7DB0,0x7DD5,0x7DFA,0x7E1D,0x7E3E,0x7E5F,0x7E7E,\
    0x7E9C,0x7EB9,0x7ED5,0x7EEF,0x7F09,0x7F21,0x7F37,0x7F4D,\
    0x7F61,0x7F74,0x7F86,0x7F97,0x7FA6,0x7FB4,0x7FC1,0x7FCD,\
    0x7FD8,0x7FE1,0x7FE9,0x7FF0,0x7FF5,0x7FF9,0x7FFD,0x7FFE}  //最大值为32766

#define SIN_MASK        0x0300u  // 十进制768, 数字后面有u声明这是一个无符号数
#define U0_90           0x0200u
#define U90_180         0x0300u
#define U180_270        0x0000u
#define U270_360        0x0100u
#define divSQRT_3 (int32_t)0x49E6    /* 1/sqrt(3) in q1.15 format=0.5773315*/
	
const int16_t h_sin_cos_table[256] = SIN_COS_TABLE;	

/**
  * @brief 反park变换:将同步旋转坐标系下的vqd，变换为静止坐标系的v_alpha,v_beta
  * @param volt_input: vqd
  * @param theta: 电角度值
  */
alphabeta_t mc_rev_park(qd_t volt_input, int16_t theta)
{
	int32_t q_v_alpha_tmp1, q_v_alpha_tmp2, q_v_beta_tmp1, q_v_beta_tmp2;
	trig_components local_vector_components;
	alphabeta_t volt_output;
	
	local_vector_components = mc_trig_functions(theta);

	q_v_alpha_tmp1 = volt_input.q * ( int32_t )local_vector_components.h_cos;
	q_v_alpha_tmp2 = volt_input.d * ( int32_t )local_vector_components.h_sin;
	
	volt_output.alpha = ( int16_t )( ( ( q_v_alpha_tmp1 ) + ( q_v_alpha_tmp2 ) ) / 32768 );


	q_v_beta_tmp1 = volt_input.q * ( int32_t )local_vector_components.h_sin;
	q_v_beta_tmp2 = volt_input.d * ( int32_t )local_vector_components.h_cos;
	
	volt_output.beta = ( int16_t )( ( q_v_beta_tmp2 - q_v_beta_tmp1 ) / 32768 );

	
	return (volt_output);
}

/**
  * @brief 根据输入的电角度，计算三角正弦和余弦
  * @param h_angle: 电角度值
  */
trig_components mc_trig_functions( int16_t h_angle )
{
  int32_t shindex;
  uint16_t uhindex;

  trig_components local_components;

  /* 10 bit index computation  */
  shindex = ( ( int32_t )32768 + ( int32_t )h_angle );
  uhindex = ( uint16_t )shindex;
  uhindex /= ( uint16_t )64; //在正弦表中把90度分成了256份


  switch ( ( uint16_t )( uhindex ) & SIN_MASK )
  {
    case U0_90:
      local_components.h_sin = h_sin_cos_table[( uint8_t )( uhindex )];
      local_components.h_cos = h_sin_cos_table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )];
      break;

    case U90_180:
      local_components.h_sin = h_sin_cos_table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )];
      local_components.h_cos = -h_sin_cos_table[( uint8_t )( uhindex )];
      break;

    case U180_270:
      local_components.h_sin = -h_sin_cos_table[( uint8_t )( uhindex )];
      local_components.h_cos = -h_sin_cos_table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )];
      break;

    case U270_360:
      local_components.h_sin =  -h_sin_cos_table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )];
      local_components.h_cos =  h_sin_cos_table[( uint8_t )( uhindex )];
      break;
    default:
      break;
  }
  return ( local_components );
}


/* MMI Table Motor 1 MAX_MODULATION_92_PER_CENT */
#define START_INDEX     54
#define MAX_MODULE      30145   // root(Vd^2+Vq^2) <= MAX_MODULE = 32767*92%
#define MMITABLE {\
32424,32091,31929,31611,31302,31002,30855,30568,30289,30017,\
29884,29622,29368,29243,28998,28759,28526,28412,28187,27968,\
27753,27648,27441,27238,27040,26942,26750,26563,26470,26288,\
26110,25935,25849,25679,25513,25350,25269,25111,24955,24803,\
24727,24579,24433,24361,24219,24079,23942,23874,23740,23609,\
23479,23415,23289,23165,23042,22982,22863,22745,22629,22572,\
22459,22347,22292,22183,22075,21970,21917,21813,21711,21610,\
21561,21462,21365,21268\
}

uint16_t circle_limit_table[74] = MMITABLE;

/*
	Check whether Vqd.q^2 + Vqd.d^2 <= 32767^2
 and if not it applies a limitation keeping constant ratio
 Vqd.q / Vqd.d.
*/
qd_t circle_limitation(qd_t v_qd)
{
	uint16_t table_element;
	uint32_t uw_temp;
	int32_t sw_temp;

	qd_t local_vqd = v_qd;
	sw_temp = (int32_t)(v_qd.q) * (v_qd.q) +
			  (int32_t)(v_qd.d) * (v_qd.d);	
	
	uw_temp = (uint32_t)sw_temp;
	
	/* uw_temp 最小值 0, 最大值 2*32767*32767 */
	if (uw_temp > (uint32_t)(MAX_MODULE) * MAX_MODULE) {
		uw_temp /= (uint32_t)(16777216); // 16777216 = 2 * 32768 * 256

		// 计算前，uw_temp 最小值 p_handle->start_index, 最大值 127 
		// 计算后，uw_temp 最小值0， 最大值127 - p_handle->start_index
		uw_temp -= START_INDEX;

		table_element = circle_limit_table[(uint8_t)uw_temp];

		sw_temp = v_qd.q * (int32_t)table_element;
		local_vqd.q = (int16_t)(sw_temp / 32768);

		sw_temp = v_qd.d * (int32_t)(table_element);
		local_vqd.d = (int16_t)(sw_temp / 32768);
	}

	return (local_vqd);
}




