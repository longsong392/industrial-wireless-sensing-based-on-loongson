/*
 * BMP280.c
 *
 * created: 2024/5/23
 *  author: 
 */
#include "BMP280.h"
#include "ls1x_string.h"


int32_t bmp280RawPressure = 0;
int32_t bmp280RawTemperature = 0;

#define BMP280_PRESSURE_OSR			(BMP280_OVERSAMP_8X)
#define BMP280_TEMPERATURE_OSR		(BMP280_OVERSAMP_16X)
#define BMP280_MODE					(BMP280_PRESSURE_OSR << 2 | BMP280_TEMPERATURE_OSR << 5 | BMP280_NORMAL_MODE)

bmp280Calib  bmp280Cal;

uint8_t BMP280_Init(void)
{
    uint8_t bmp280_id;
    uint8_t tmp[10];

    Sensors_I2C_ReadRegister(BMP280_SLAVE_ADDRESS, BMP280_CHIPID_REG, 1, &bmp280_id);

    /* 读取校准数据 */
    Sensors_I2C_ReadRegister(BMP280_SLAVE_ADDRESS, BMP280_DIG_T1_LSB_REG,24,(INT8U *)&bmp280Cal);

    tmp[0] = BMP280_MODE;
    Sensors_I2C_WriteRegister(BMP280_SLAVE_ADDRESS, BMP280_CTRLMEAS_REG, 1, tmp);

    tmp[0] = (5<<2);
    Sensors_I2C_WriteRegister(BMP280_SLAVE_ADDRESS, BMP280_CONFIG_REG, 1, tmp);		/*配置IIR滤波*/

    return bmp280_id;
}

 void BMP280GetPressure(void)
{
    uint8_t data[BMP280_DATA_FRAME_SIZE];

    // read data from sensor
    Sensors_I2C_ReadRegister(BMP280_SLAVE_ADDRESS, BMP280_PRESSURE_MSB_REG, BMP280_DATA_FRAME_SIZE, (uint8_t *)&data);
    bmp280RawPressure = (int32_t)((((uint32_t)(data[0])) << 12) | (((uint32_t)(data[1])) << 4) | ((uint32_t)data[2] >> 4));
    bmp280RawTemperature = (int32_t)((((uint32_t)(data[3])) << 12) | (((uint32_t)(data[4])) << 4) | ((uint32_t)data[5] >> 4));
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC
// t_fine carries fine temperature as global value
 int32_t BMP280CompensateT(int32_t adcT)
{
    int32_t var1, var2, T;

    var1 = ((((adcT >> 3) - ((int32_t)bmp280Cal.dig_T1 << 1))) * ((int32_t)bmp280Cal.dig_T2)) >> 11;
    var2  = (((((adcT >> 4) - ((int32_t)bmp280Cal.dig_T1)) * ((adcT >> 4) - ((int32_t)bmp280Cal.dig_T1))) >> 12) * ((int32_t)bmp280Cal.dig_T3)) >> 14;
    bmp280Cal.t_fine = var1 + var2;

    T = (bmp280Cal.t_fine * 5 + 128) >> 8;

    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
 uint32_t BMP280CompensateP(int32_t adcP)
{
    INT64U var1, var2, p;
    var1 = ((INT64U)bmp280Cal.t_fine) - 128000;
    var2 = var1 * var1 * (INT64U)bmp280Cal.dig_P6;
    var2 = var2 + ((var1*(INT64U)bmp280Cal.dig_P5) << 17);
    var2 = var2 + (((INT64U)bmp280Cal.dig_P4) << 35);
    var1 = ((var1 * var1 * (INT64U)bmp280Cal.dig_P3) >> 8) + ((var1 * (INT64U)bmp280Cal.dig_P2) << 12);
    var1 = (((((INT64U)1) << 47) + var1)) * ((INT64U)bmp280Cal.dig_P1) >> 33;
    if (var1 == 0)
        return 0;
    p = 1048576 - adcP;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((INT64U)bmp280Cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((INT64U)bmp280Cal.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((INT64U)bmp280Cal.dig_P7) << 4);
    return (INT32U)p;
}

/**
 * Converts pressure to altitude above sea level (ASL) in meters
 */

#define CONST_PF 0.1902630958	//(1/5.25588f) Pressure factor
#define FIX_TEMP 25				// Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
								// TLDR: Adjusting for temp changes does more harm than good.



void BMP280GetData(void)
{
     INT64U t;
     INT64U p;

	BMP280GetPressure();
	t = BMP280CompensateT(bmp280RawTemperature);
	p = BMP280CompensateP(bmp280RawPressure);
	printf("t: %d\r\n",t);
	printf("p: %d\r\n",p);
	//temperature = (float)t;/*单位度*/

}


