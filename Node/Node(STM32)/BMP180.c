#include "BMP180.h"

BMP180_EEPROM bmp180_eeprom;

const uint8_t BMP180_EEPROM_ADDR_MSB[11] = { 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe };
const uint8_t BMP180_EEPROM_ADDR_LSB[11] = { 0xab, 0xad, 0xaf, 0xb1, 0xb3, 0xb5, 0xb7, 0xb9, 0xbb, 0xbd, 0xbf };

#ifndef SOFT_I2C

// Read calibration data from EEPROM of the BMP180
void BMP180_UpdateCalibrationData(void) 
{
	//I2C_ReadMulti(I2Cx, BMP180_I2C_ADDR << 1 | 1, 0xAA, data,16);
  bmp180_eeprom.BMP180_AC1 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[0]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[0])  ;
	bmp180_eeprom.BMP180_AC2 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[1]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[1]) ;
	bmp180_eeprom.BMP180_AC3 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[2]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[2]) ;
	bmp180_eeprom.BMP180_AC4 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[3]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[3]) ;
	bmp180_eeprom.BMP180_AC5 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[4]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[4]) ;
	bmp180_eeprom.BMP180_AC6 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[5]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[5]) ;
	bmp180_eeprom.BMP180_B1 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[6]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[6]) ;
	bmp180_eeprom.BMP180_B2 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[7]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[7]) ;
	bmp180_eeprom.BMP180_MB = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[8]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[8]) ;
	bmp180_eeprom.BMP180_MC = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[9]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[9]) ;
	bmp180_eeprom.BMP180_MD = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[10]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[10]);
}



// Read uncompensated temperature value
int32_t BMP180_GetRawTemperature(void)
{
	uint8_t data[3];
	int32_t ut;
	int32_t x1;
	int32_t x2;
	int32_t b5;
	// Read uncompensated temperature value
	I2c_Write(bmp180_i2c, BMP180_I2C_ADDR << 1, 0xf4, 0x2e);
	delay_ms(5);
	//I2C_ReadMulti(I2Cx, BMP180_I2C_ADDR << 1 | 1, 0xf6, data, 2);
	
	ut = (BMP180_ReadReg(BMP180_MSB_REG) << 8) | BMP180_ReadReg(BMP180_LSB_REG);
	x1 = (ut - bmp180_eeprom.BMP180_AC6) * bmp180_eeprom.BMP180_AC5 / (1 << 15);
	x2 = (bmp180_eeprom.BMP180_MC * (1 << 11)) / (x1 + bmp180_eeprom.BMP180_MD);
	b5 = x1 + x2;
	return (b5 + 8) / (1 << 4);
}

double BMP180_GetTemperature(void) {
	int32_t temp = BMP180_GetRawTemperature();
	return temp / 10.0;
}


int32_t BMP180_GetPressure(void) // don vi la Pa
{
	uint8_t data[3];
	int32_t up, x1, x2, b5, b6, x3, b3, b4, b7, p, ut;
		// Read uncompensated pressure value
	I2c_Write(bmp180_i2c, BMP180_I2C_ADDR << 1, 0xf4, 0x2e);
	delay_ms(5);
//	I2C_ReadMulti(I2Cx, BMP180_I2C_ADDR << 1 | 1, 0xf6, data, 2);
//	ut = (data[0] << 8) | data[1];
	ut = (BMP180_ReadReg(BMP180_MSB_REG) << 8) | BMP180_ReadReg(BMP180_LSB_REG);
	
	I2c_Write(bmp180_i2c, BMP180_I2C_ADDR << 1, 0xf4, 0x34);
	delay_ms(5);
//	I2C_ReadMulti(bmp180_i2c, BMP180_I2C_ADDR << 1 | 1, 0xf6, data, 3);
	data[0] = BMP180_ReadReg(0xf6);
	data[1] = BMP180_ReadReg(0xf7);
	data[2] = BMP180_ReadReg(0xf8);
	 up = ((data[0] << 16) | (data[1] << 8) | data[2]) >> (8);
	 x1 = ((ut - bmp180_eeprom.BMP180_AC6) * bmp180_eeprom.BMP180_AC5) >> 15;
	 x2 = (bmp180_eeprom.BMP180_MC << 11) / (x1 + bmp180_eeprom.BMP180_MD);
	 b5 = x1 + x2;
	
	 b6 = b5 - 4000;
	x1 = (bmp180_eeprom.BMP180_B2 * ((b6 * b6) >> 12)) >> 11;
	x2 = (bmp180_eeprom.BMP180_AC2 * b6) >> 11;
	 x3 = x1 + x2;
	 b3 = (((bmp180_eeprom.BMP180_AC1 * 4 + x3)) + 2) / 4;
	x1 = (bmp180_eeprom.BMP180_AC3 * b6) >> 13;
	x2 = (bmp180_eeprom.BMP180_B1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) / 4;
	b4 = (bmp180_eeprom.BMP180_AC4 * (uint32_t) (x3 + 32768)) >> 15;
	b7 = (up - b3) * (50000);
	if (b7 < 0x80000000)
		p = (b7 / b4 * 2) ;
	else
		p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p = p + ((x1 + x2 + 3791) >> 4);
	return p;
}

uint8_t BMP180_ReadReg(uint8_t reg)
{
	return I2c_Read(bmp180_i2c, BMP180_I2C_ADDR << 1 | 1, reg);
}


#else


// Read calibration data from EEPROM of the BMP180
void BMP180_UpdateCalibrationData(void) 
{
	//I2C_ReadMulti(I2Cx, BMP180_I2C_ADDR << 1 | 1, 0xAA, data,16);
  bmp180_eeprom.BMP180_AC1 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[0]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[0])  ;
	bmp180_eeprom.BMP180_AC2 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[1]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[1]) ;
	bmp180_eeprom.BMP180_AC3 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[2]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[2]) ;
	bmp180_eeprom.BMP180_AC4 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[3]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[3]) ;
	bmp180_eeprom.BMP180_AC5 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[4]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[4]) ;
	bmp180_eeprom.BMP180_AC6 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[5]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[5]) ;
	bmp180_eeprom.BMP180_B1 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[6]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[6]) ;
	bmp180_eeprom.BMP180_B2 = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[7]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[7]) ;
	bmp180_eeprom.BMP180_MB = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[8]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[8]) ;
	bmp180_eeprom.BMP180_MC = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[9]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[9]) ;
	bmp180_eeprom.BMP180_MD = BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[10]) << 8 | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[10]);
}



// Read uncompensated temperature value
int32_t BMP180_GetRawTemperature(void)
{
	uint8_t data[3];
	int32_t ut;
	int32_t x1;
	int32_t x2;
	int32_t b5;
	// Read uncompensated temperature value
	// I2c_Write(bmp180_i2c, BMP180_I2C_ADDR << 1, 0xf4, 0x2e);
	I2C_Start();
	I2C_Send_Byte(BMP180_I2C_ADDR << 1);
	I2C_Wait_Ack();
	I2C_Send_Byte(0xf4);
	I2C_Wait_Ack();
	I2C_Send_Byte(0x2e);
	I2C_Wait_Ack();	
	I2C_Stop();
	
	delay_ms(5);
	//I2C_ReadMulti(I2Cx, BMP180_I2C_ADDR << 1 | 1, 0xf6, data, 2);
	
	ut = (BMP180_ReadReg(BMP180_MSB_REG) << 8) | BMP180_ReadReg(BMP180_LSB_REG);
	x1 = (ut - bmp180_eeprom.BMP180_AC6) * bmp180_eeprom.BMP180_AC5 / (1 << 15);
	x2 = (bmp180_eeprom.BMP180_MC * (1 << 11)) / (x1 + bmp180_eeprom.BMP180_MD);
	b5 = x1 + x2;
	return (b5 + 8) / (1 << 4);
}

double BMP180_GetTemperature(void) {
	int32_t temp = BMP180_GetRawTemperature();
	return temp / 10.0;
}


int32_t BMP180_GetPressure(void) // don vi la Pa
{
	uint8_t data[3];
	int32_t up, x1, x2, b5, b6, x3, b3, b4, b7, p, ut;
		// Read uncompensated pressure value
	// I2c_Write(bmp180_i2c, BMP180_I2C_ADDR << 1, 0xf4, 0x2e);
	I2C_Start();
	I2C_Send_Byte(BMP180_I2C_ADDR << 1);
	I2C_Wait_Ack();
	I2C_Send_Byte(0xf4);
	I2C_Wait_Ack();
	I2C_Send_Byte(0x2e);
	I2C_Wait_Ack();	
	I2C_Stop();
	delay_ms(5);
//	I2C_ReadMulti(I2Cx, BMP180_I2C_ADDR << 1 | 1, 0xf6, data, 2);
//	ut = (data[0] << 8) | data[1];
	ut = (BMP180_ReadReg(BMP180_MSB_REG) << 8) | BMP180_ReadReg(BMP180_LSB_REG);
	
	// I2c_Write(bmp180_i2c, BMP180_I2C_ADDR << 1, 0xf4, 0x34);
	I2C_Start();
	I2C_Send_Byte(BMP180_I2C_ADDR << 1);
	I2C_Wait_Ack();
	I2C_Send_Byte(0xf4);
	I2C_Wait_Ack();
	I2C_Send_Byte(0x34);
	I2C_Wait_Ack();	
	I2C_Stop();
	
	delay_ms(5);
//	I2C_ReadMulti(bmp180_i2c, BMP180_I2C_ADDR << 1 | 1, 0xf6, data, 3);
	data[0] = BMP180_ReadReg(0xf6);
	data[1] = BMP180_ReadReg(0xf7);
	data[2] = BMP180_ReadReg(0xf8);
	 up = ((data[0] << 16) | (data[1] << 8) | data[2]) >> (8);
	 x1 = ((ut - bmp180_eeprom.BMP180_AC6) * bmp180_eeprom.BMP180_AC5) >> 15;
	 x2 = (bmp180_eeprom.BMP180_MC << 11) / (x1 + bmp180_eeprom.BMP180_MD);
	 b5 = x1 + x2;
	
	 b6 = b5 - 4000;
	x1 = (bmp180_eeprom.BMP180_B2 * ((b6 * b6) >> 12)) >> 11;
	x2 = (bmp180_eeprom.BMP180_AC2 * b6) >> 11;
	 x3 = x1 + x2;
	 b3 = (((bmp180_eeprom.BMP180_AC1 * 4 + x3)) + 2) / 4;
	x1 = (bmp180_eeprom.BMP180_AC3 * b6) >> 13;
	x2 = (bmp180_eeprom.BMP180_B1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) / 4;
	b4 = (bmp180_eeprom.BMP180_AC4 * (uint32_t) (x3 + 32768)) >> 15;
	b7 = (up - b3) * (50000);
	if (b7 < 0x80000000)
		p = (b7 / b4 * 2) ;
	else
		p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p = p + ((x1 + x2 + 3791) >> 4);
	return p;
}

uint8_t BMP180_ReadReg(uint8_t reg)
{
	uint8_t dat;
	I2C_Start();
	I2C_Send_Byte(BMP180_I2C_ADDR << 1);
	I2C_Wait_Ack();
	I2C_Send_Byte(reg);
	I2C_Wait_Ack();
	I2C_Stop();
	I2C_Start();
	I2C_Send_Byte(BMP180_I2C_ADDR << 1 | 1);
	I2C_Wait_Ack();
	dat = I2C_Read_Byte(1);
	I2C_Stop();
	return dat;
}

#endif
