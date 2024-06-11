#include "sht3x.h"

#ifdef SOFTWARE_I2C

void SHT3x_Reset(void){
	I2C_Start();
	I2C_Send_Byte(0x44 << 1);
	I2C_Wait_Ack();
	I2C_Send_Byte(0x30);
	I2C_Wait_Ack();
	I2C_Send_Byte(0xA2);
	I2C_Wait_Ack();
	I2C_Stop();
	delay_ms(10);
}

void SHT3x_Read_T_H(unsigned int *temperature, unsigned int *humidity){
	I2C_Start();
	I2C_Send_Byte(0x44 << 1);
	I2C_Wait_Ack();
	I2C_Send_Byte(0x24);
	I2C_Wait_Ack();
	I2C_Send_Byte(0x00);
	I2C_Wait_Ack();
	I2C_Stop();
	delay_ms(20);
	I2C_Start();
	I2C_Send_Byte((0x44 << 1) | 1);
	I2C_Wait_Ack();
	*temperature = I2C_Read_Byte(0);
	*temperature <<= 8;
	*temperature |= I2C_Read_Byte(0);
	I2C_Read_Byte(0);
	*humidity = I2C_Read_Byte(0);
	*humidity <<= 8;
	*humidity |= I2C_Read_Byte(0);
	I2C_Read_Byte(1);
	I2C_Stop();
}

#else

void SHT3x_Reset(I2C_TypeDef* I2Cx){
	I2c_Start(I2Cx, 0x44 << 1,  I2C_Direction_Transmitter, 0);
	I2c_WriteData(I2Cx, 0x30);
	I2c_WriteData(I2Cx, 0xA2);
	I2c_Stop(I2Cx);
	delay_ms(10);
}

void SHT3x_Read_T_H(I2C_TypeDef* I2Cx, unsigned int *temperature, unsigned int *humidity){
	I2c_Start(I2Cx, 0x44 << 1,  I2C_Direction_Transmitter, 0);
	I2c_WriteData(I2Cx, 0x24);
	I2c_WriteData(I2Cx, 0x00);
	I2c_Stop(I2Cx);
	delay_ms(20);
	I2c_Start(I2Cx, (0x44 << 1) | 1,  I2C_Direction_Receiver, 0);
	*temperature = I2c_ReadAck(I2Cx);
	*temperature <<= 8;
	*temperature |= I2c_ReadAck(I2Cx);
	I2c_ReadAck(I2Cx);
	*humidity = I2c_ReadAck(I2Cx);
	*humidity <<= 8;
	*humidity |= I2c_ReadAck(I2Cx);
	I2c_ReadNack(I2Cx);
	I2c_Stop(I2Cx);
}

#endif
