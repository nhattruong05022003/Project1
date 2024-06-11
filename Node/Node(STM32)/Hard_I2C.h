#ifndef _HARD_I2C_
#define _HARD_I2C_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

// 

// Speed mode
#define I2C_CLOCK_STANDARD 100000
#define I2C_CLOCK_FAST_MODE 400000
#define I2C_CLOCK_FAST_MODE_PLUS 1000000
#define I2C_CLOCK_HIGH_SPEED 3400000

typedef enum{ // SCL:SDA
	Pin_PB6PB7, // I2C1
	Pin_PB8PB9, // I2C1
	
	Pin_PB10PB11 // I2C2
} I2C_PinsPack_t;

// Init thi ko can GPIO Configuaration
void I2c_Init(I2C_TypeDef* I2Cx, I2C_PinsPack_t pinspack, uint32_t clockSpeed); // default cua clockSpeed = 100kHz
void I2c_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack); // address : dia chi device (gui 8 bit)
void I2c_Stop(I2C_TypeDef* I2Cx);
void I2c_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
// Doc du lieu va gui ACK
uint8_t I2c_ReadAck(I2C_TypeDef* I2Cx);
// // Doc du lieu va gui NACK
uint8_t I2c_ReadNack(I2C_TypeDef* I2Cx);

uint8_t I2c_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);
void I2c_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);
u8 I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);
u8 I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);

#ifdef __cplusplus
}
#endif

#endif