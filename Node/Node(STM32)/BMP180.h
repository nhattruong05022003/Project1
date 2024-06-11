#ifndef _BMP180_
#define _BMP180_

#include "main.h"

// #define SOFT_I2C

#define BMP180_I2C_ADDR		0x77
#define BMP180_MSB_REG					0xf6
#define BMP180_LSB_REG					0xf7
#define BMP180_XLSB_REG					0xf8

typedef struct BMP180_EEPROM {
	short BMP180_AC1;
	short BMP180_AC2;
	short BMP180_AC3;
	unsigned short BMP180_AC4;
	unsigned short BMP180_AC5;
	unsigned short BMP180_AC6;
	short BMP180_B1;
	short BMP180_B2;
	short BMP180_MB;
	short BMP180_MC;
	short BMP180_MD;
} BMP180_EEPROM;


#ifndef SOFT_I2C

#define bmp180_i2c I2C1
	
#endif

uint8_t BMP180_ReadReg(uint8_t reg);
void BMP180_UpdateCalibrationData(void);

int32_t BMP180_GetRawTemperature(void);
double BMP180_GetTemperature(void);
int32_t BMP180_GetPressure(void);



#endif