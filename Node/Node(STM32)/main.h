#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_i2c.h"              // Device:StdPeriph Drivers:I2C
#include "stm32f10x_tim.h"              // Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Device:StdPeriph Drivers:USART



#include "delay.h"
#include "gpio.h"
#include "lcd16x2.h"
// #include "Soft_I2c.h"
#include "Hard_I2C.h"
#include "BMP180.h"
#include "sht3x.h"
#include "usart.h"


#ifdef __cplusplus
}
#endif

#endif

