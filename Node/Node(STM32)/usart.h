#ifndef _USART_H_
#define _USART_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

// Nho enable ngat : USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
// Khoi tao san port ko can GPIO_Configuration
void USART1_Init(void);
void USART1_PutChar(char data);
void USART1_PutString(char* s);
uint8_t USART1_GetChar(void);

void USART2_Init(void);
void USART2_PutChar(char data);
void USART2_PutString(char* s);
uint8_t USART2_GetChar(void);

// Khoi tao ham nay dau tien de giao tiep USART
// void Clock_Config(void);

#ifdef __cplusplus
}
#endif

#endif 