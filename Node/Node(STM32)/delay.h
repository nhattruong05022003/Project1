#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
#include "stdio.h" 
//////////////////////////////////////////////////////////////////////////////////	 

//SysTick STM32F10x
//delay_us,delay_ms

// #define DELAY_INTERRUPT

#ifdef DELAY_INTERRUPT
u32 millis(void);
u32 micros(void);

void delay_init(void);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay( uint32_t ms );
void delayMicroseconds(uint32_t us);

#else

void delay_init(void);
void DelayMillis(void);
void delay_ms(unsigned long t);

#endif

#endif





























