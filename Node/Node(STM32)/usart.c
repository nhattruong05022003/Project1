#include "usart.h"

void USART1_Init(void){
//	/*
//	Khoi kich hoat xung HSI de USART hoat dong
//	*/
//	/*enable HSI source clock*/
//	RCC_HSICmd(ENABLE);	
//	/* Wait till PLL is ready */
//	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
//	/* Select HSI as system clock source */
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
//	/* Wait till HSI is used as system clock source */
//	while(RCC_GetSYSCLKSource() != 0x00) {}    
	
	USART_InitTypeDef USART_InitStruct;
	
	// Enable xung clock AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	
	// Thiet lap toc do baud
	USART_InitStruct.USART_BaudRate = 19200;
	// Cau hinh che do dieu khien luu luong (khong dieu khien bang hardware, dieu khien bang software)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	/*
	The USART1_RTS is an output pin. 
	It is set/high when the USART1 is ready to receive data. 
	The USART1_CTS is an input pin. 
	The other end sets it high when it is ready to receive data, and low when it is not.

	Let's say that you send and receive one character at a time, 
	and use two GPIO pins for USART1_RTS and USART1_CTS instead of the hardware support.

	Normally, you keep USART1_RTS high. 
	When receiving data, if you are running out of receive buffer, you set USART1_RTS low. 
	When you make more room in the receive buffer, you set USART1_RTS high. 
	(If you have a buffering scheme that cannot run out of receive buffer, you can tie the RTS line high.)

	Before sending each character, you check if USART1_CTS is high. 
	If it is low, you don't send the data, but wait for USART1_CTS to become high before you do.
	*/
	
	// Che do truyen nhan (chon ca truyen va nhan)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// Cau hinh su dung che do Parity (kiem tra loi)
	USART_InitStruct.USART_Parity = USART_Parity_No;
	
	/*
	When parity is enabled, the computed parity is inserted
  at the MSB position of the transmitted data (9th bit when
  the word length is set to 9 data bits; 8th bit when the
  word length is set to 8 data bits).
	*/
	
	// Cau hinh so bit STOP 
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	
	/*
	Co 4 loai:
	USART_STOPBITS_0_5 : USART frame with 0.5 stop bit
	USART_StopBits_1 : USART frame with 1 stop bit
	USART_StopBits_1_5 : USART frame with 1.5 stop bit
	USART_StopBits_2 : USART frame with 2 stop bit
	*/
	
	// Cau hinh so bit nhan trong 1 khung truyen
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
	// Enable RXD interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct1;
		// Enable xung clock AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// Enable xung clock GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Chan Tx dinh nghia dang push pull alternative function
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // Chan Tx vua vao vua ra kieu day keo
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Chan Rx dinh nghia input floating
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Chan Rx dinh nghia ngo vao tha noi
	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_10;

	GPIO_Init(GPIOA, &GPIO_InitStruct1);
	
	// Thiet lap ngat USART1
	NVIC_InitTypeDef NVIC_InitStruct;
	

	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

void USART1_PutChar(char data){
	// Cho thanh ghi truyen du lieu rong moi truyen tiep 
	// USART_IT_TXE : Transmit Data Register empty interrupt
	// USART_FLAG_TC:   Transmission Complete flag
	USART1->DR = 0x00;
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART1_PutString(char* s){
	uint16_t i = 0;
	while(s[i] != 0){
		USART1_PutChar(s[i]);
		i++;
	}
}

uint8_t USART1_GetChar(void){
	// Cho cho den khi nhan du lieu xong
	// USART_FLAG_RXNE: Receive data register not empty flag
	while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
	
	return USART_ReceiveData(USART1);
}

void USART2_Init(void){
	/*
	Khoi kich hoat xung HSI de USART hoat dong
	*/
	/*enable HSI source clock*/
	RCC_HSICmd(ENABLE);	
	/* Wait till PLL is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
	/* Select HSI as system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	/* Wait till HSI is used as system clock source */
	while(RCC_GetSYSCLKSource() != 0x00) {}    
	
	USART_InitTypeDef USART_InitStruct;
	
	// Enable xung clock AFIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	
	// Thiet lap toc do baud
	USART_InitStruct.USART_BaudRate = 9600;
	// Cau hinh che do dieu khien luu luong (khong dieu khien bang hardware, dieu khien bang software)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	/*
	The USART1_RTS is an output pin. 
	It is set/high when the USART1 is ready to receive data. 
	The USART1_CTS is an input pin. 
	The other end sets it high when it is ready to receive data, and low when it is not.

	Let's say that you send and receive one character at a time, 
	and use two GPIO pins for USART1_RTS and USART1_CTS instead of the hardware support.

	Normally, you keep USART1_RTS high. 
	When receiving data, if you are running out of receive buffer, you set USART1_RTS low. 
	When you make more room in the receive buffer, you set USART1_RTS high. 
	(If you have a buffering scheme that cannot run out of receive buffer, you can tie the RTS line high.)

	Before sending each character, you check if USART1_CTS is high. 
	If it is low, you don't send the data, but wait for USART1_CTS to become high before you do.
	*/
	
	// Che do truyen nhan (chon ca truyen va nhan)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// Cau hinh su dung che do Parity (kiem tra loi)
	USART_InitStruct.USART_Parity = USART_Parity_No;
	
	/*
	When parity is enabled, the computed parity is inserted
  at the MSB position of the transmitted data (9th bit when
  the word length is set to 9 data bits; 8th bit when the
  word length is set to 8 data bits).
	*/
	
	// Cau hinh so bit STOP 
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	
	/*
	Co 4 loai:
	USART_STOPBITS_0_5 : USART frame with 0.5 stop bit
	USART_StopBits_1 : USART frame with 1 stop bit
	USART_StopBits_1_5 : USART frame with 1.5 stop bit
	USART_StopBits_2 : USART frame with 2 stop bit
	*/
	
	// Cau hinh so bit nhan trong 1 khung truyen
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
	
	// Enable RXD interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct1;
		// Enable xung clock AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// Enable xung clock GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Chan Tx dinh nghia dang push pull alternative function
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // Chan Tx vua vao vua ra kieu day keo
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Chan Rx dinh nghia input floating
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Chan Rx dinh nghia ngo vao tha noi
	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_3;

	GPIO_Init(GPIOA, &GPIO_InitStruct1);
	
	// Thiet lap ngat USART2
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

void USART2_PutChar(char data){
	// Cho thanh ghi truyen du lieu rong moi truyen tiep 
	//USART_IT_TXE : Transmit Data Register empty interrupt
	// USART_FLAG_TC:   Transmission Complete flag
	USART2->DR = 0x00;
	USART_SendData(USART2, data);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void USART2_PutString(char* s){
	uint16_t i = 0;
	while(s[i] != 0){
		USART2_PutChar(s[i]);
		i++;
	}
}

uint8_t USART2_GetChar(void){
	// Cho cho den khi nhan du lieu xong
	// USART_FLAG_RXNE: Receive data register not empty flag
	while(!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	
	return USART_ReceiveData(USART2);
}

//void Clock_Config(void)
//{
//    /* RCC system reset */
//    RCC_DeInit();
//    /* HCLK = SYSCLK */
//    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
//    /* PCLK2 = HCLK */
//    RCC_PCLK2Config(RCC_HCLK_Div1);
//    /* PCLK1 = HCLK/2 */
//    RCC_PCLK1Config(RCC_HCLK_Div1);
//    /*enable HSI source clock*/
//    RCC_HSICmd(ENABLE);	
//    /* Wait till PLL is ready */
//    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
//    /* Select HSI as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
//    /* Wait till HSI is used as system clock source */
//    while(RCC_GetSYSCLKSource() != 0x00) {}    
//}