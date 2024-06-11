#include "Hard_I2C.h"

GPIO_InitTypeDef GPIO_I2C_InitStruct;

// Ham private
void I2C1_InitPin(I2C_PinsPack_t pinspack);
void I2C2_InitPin(I2C_PinsPack_t pinspack);
//

void I2c_Init(I2C_TypeDef* I2Cx, I2C_PinsPack_t pinspack, uint32_t clockSpeed){

	I2C_InitTypeDef I2C_InitStruct;
	// Cap xung cho Alternative Function
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// Set default I2C GPIO settings
	GPIO_I2C_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_I2C_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	
	
	if(I2Cx == I2C1){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		// Enable pin
		I2C1_InitPin(pinspack);
		
		// Set value
		I2C_InitStruct.I2C_ClockSpeed = clockSpeed;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_OwnAddress1 = 0x00; // own address (dia chi cua stm32), not relevant in master mode
		I2C_InitStruct.I2C_Ack = I2C_Ack_Disable; // disable acknowledge when reading
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
		
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	}
	else if(I2Cx == I2C2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		
		// Enable pin
		I2C2_InitPin(pinspack);
		
		// Set value
		I2C_InitStruct.I2C_ClockSpeed = clockSpeed;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_OwnAddress1 = 0x00; // own address (dia chi cua stm32), not relevant in master mode
		I2C_InitStruct.I2C_Ack = I2C_Ack_Disable; // disable acknowledge when reading
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
		
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	}
	
	I2C_Cmd(I2Cx, DISABLE);
	I2C_Init(I2Cx, &I2C_InitStruct);
	I2C_Cmd(I2Cx, ENABLE);
	
}
void I2c_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack){
	
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	uint16_t I2C_Timeout = 2000;
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) && I2C_Timeout) {
		if (--I2C_Timeout == 0x00) {
			break;
		}
	}

	if (ack) {
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}
	
	I2C_Send7bitAddress(I2Cx, address, direction);

	if (direction == I2C_Direction_Transmitter) {
		I2C_Timeout = 2000;
		while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) && I2C_Timeout) {
			if (--I2C_Timeout == 0x00) {
				break;
			}
		}
	} else if (direction == I2C_Direction_Receiver) {
		I2C_Timeout = 2000;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && I2C_Timeout) {
			if (--I2C_Timeout == 0x00) {
				break;
			}
		}
	}
	I2Cx->SR2;
}

void I2c_Stop(I2C_TypeDef* I2Cx){
	uint16_t I2C_Timeout = 2000; // thoi gian cho
	// I2C_FLAG_TXE: Data register empty flag (Transmitter)
	// I2C_FLAG_BTF: Byte transfer finished flag
	// Cho truyen xong du lieu
	while (((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF))) && I2C_Timeout) {
		if (--I2C_Timeout == 0x00) {
			break;
		}
	}
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2c_WriteData(I2C_TypeDef* I2Cx, uint8_t data){
	uint16_t I2C_Timeout = 2000; // thoi gian cho
	// I2C_FLAG_TXE: Data register empty flag (Transmitter)
	// Cho truyen xong du lieu
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) && I2C_Timeout) {
		I2C_Timeout--;
	}
	I2C_SendData(I2Cx, data);
}

uint8_t I2c_ReadAck(I2C_TypeDef* I2Cx){
	uint8_t data;
	I2C_AcknowledgeConfig(I2Cx, ENABLE); // ENABLE I2C_InitStruct.I2C_Ack
	
	uint16_t I2C_Timeout = 2000; // thoi gian cho
	// Cho nhan du lieu
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && I2C_Timeout) {
		I2C_Timeout--;
	}

	data = I2C_ReceiveData(I2Cx);
	return data;
}

uint8_t I2c_ReadNack(I2C_TypeDef* I2Cx){
	uint8_t data;
	
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	
	uint16_t I2C_Timeout = 2000; // thoi gian cho
	// Cho nhan du lieu
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && I2C_Timeout) {
		I2C_Timeout--;
	}
	data = I2C_ReceiveData(I2Cx);
	return data;
}


uint8_t I2c_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
	uint8_t received_data;
	I2c_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2c_WriteData(I2Cx, reg);
	I2c_Stop(I2Cx);
	I2c_Start(I2Cx, address, I2C_Direction_Receiver, 0);
	received_data = I2c_ReadNack(I2Cx);
	I2c_Stop(I2Cx);
	return received_data;
}

u8 I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2c_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2c_WriteData(I2Cx, reg);
	I2c_Stop(I2Cx);
	I2c_Start(I2Cx, address, I2C_Direction_Receiver, 0);
	for (i = 0; i < count; i++) {
		if (i == (count - 1)) {
			//Last byte
			data[i] = I2c_ReadNack(I2Cx);
		} else {
			data[i] = I2c_ReadNack(I2Cx);
		}
	}
	I2c_Stop(I2Cx);
	return 0;
}

void I2c_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {
	I2c_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2c_WriteData(I2Cx, reg);
	I2c_WriteData(I2Cx, data);
	I2c_Stop(I2Cx);
}

u8 I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	I2c_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	I2c_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++) {
		I2c_WriteData(I2Cx, data[i]);
	}
	I2c_Stop(I2Cx);
	return 0;
}


void I2C1_InitPin(I2C_PinsPack_t pinspack){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	if(pinspack == Pin_PB6PB7){
		GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_I2C_InitStruct);
	}
	else if(pinspack == Pin_PB8PB9){
		GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_Init(GPIOB, &GPIO_I2C_InitStruct);
		GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
		
	}
}

void I2C2_InitPin(I2C_PinsPack_t pinspack){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	if(pinspack == Pin_PB10PB11){
		GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_Init(GPIOB, &GPIO_I2C_InitStruct);
	}
}
