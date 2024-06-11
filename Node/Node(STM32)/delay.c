#include "delay.h"


#ifdef DELAY_INTERRUPT 

volatile uint32_t counter_ms = 0;
volatile uint32_t counter_us = 0;

__IO uint32_t TimingMillis;
static void __empty(void) {
	// Empty
}
void yield(void) __attribute__ ((weak, alias("__empty")));

void SysTick_Handler(void) {
	//neu init ngat 1ms su dung 2 ham millis() v?micros()
  counter_ms++;//1ms
	TimingMillis++;
	
	//neu init ngat 1us
	//counter_us++; counter_ms %= counter_us;//1us
}

u32 millis(void) {
  return counter_ms;
}

u32 micros(void)//thong qua ms
{
	// Neu init ngat 1 ms
	u32 Micros = millis()*1000 + (SystemCoreClock/1000-SysTick->VAL)/72; // chay dc 72 tick dong nghia 1 us
	return Micros;
	
	// Neu init ngat 1 us
	// return counter_us;
}

void delay_init(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Config(SystemCoreClock / 1000);	// ngat 1ms (72 000)  SystemCoreClock = 72MHz
	//SysTick_Config (SystemCoreClock / 1000000); //1us per interrupt (72)
	// Dem tu 72 000 - 1 -> 0
}
void delay_ms (u32 nms)
{
	delay( nms );
	counter_ms = 0;
	counter_us = 0;
}


void delay_us(u32 nus)
{
	uint32_t end = micros() + nus;
  while (micros() < end);//-615
	counter_ms = 0;
	counter_us = 0;
}
//////////////////////////////////////////////////


void delay( uint32_t ms )
{
	uint32_t end = millis() + ms;
	while (millis() < end)
		yield();
	counter_ms = 0;
	counter_us = 0;
}			 


void delayMicroseconds(uint32_t us)
{
	uint32_t end = micros() + us;
  while (micros() < end);
	counter_ms = 0;
	counter_us = 0;
	
}

#else

void delay_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
}

void DelayMillis(void)
{
	SysTick->LOAD = 0x11940;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
}

void delay_ms(unsigned long t)
{
	for(;t>0;t--)
		{
			DelayMillis();
		}
}

#endif