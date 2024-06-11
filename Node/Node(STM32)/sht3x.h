#ifndef _SHT3x_H_
#define _SHT3x_H_

#include "main.h"

// #define SOFTWARE_I2C

#ifdef SOFTWARE_I2C

void SHT3x_Reset(void);
void SHT3x_Read_T_H(unsigned int *temperature, unsigned int *humidity);

#else

void SHT3x_Reset(I2C_TypeDef* I2Cx);
void SHT3x_Read_T_H(I2C_TypeDef* I2Cx, unsigned int *temperature, unsigned int *humidity);

#endif 

#endif
