#include "Soft_I2c.h"
int i = 0;
void I2c_Init(void){
	// Enable xung clock AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// Enable xung clock GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// Khai bao port
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2C_SCL = 1;
	I2C_SDA = 1;
}

void I2C_Start(void){
	SDA_OUT();
	I2C_SCL = 1;
	I2C_SDA = 1;
	delay_ms(1);
	I2C_SDA = 0;
	delay_ms(1);
	I2C_SCL = 0;
}

void I2C_Stop(void){
	SDA_OUT();
	I2C_SCL = 0;
	I2C_SDA = 0;
	delay_ms(1);
	I2C_SCL = 1;
	delay_ms(1);
	I2C_SDA = 1;
}

/*
	Tham so txd : du lieu can truyen
*/
void I2C_Send_Byte(u8 txd){ // truyen tu bit cao den bit thap
	SDA_OUT();
	int i;
	I2C_SCL = 0;
	for(i = 0; i < 8; i++){
		I2C_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_ms(1);
		I2C_SCL = 1;
		delay_ms(1);
		I2C_SCL = 0;
	}
}

void I2C_Ack(void){
	SDA_OUT();
	I2C_SDA = 0;
	delay_ms(1);
	I2C_SCL = 1;
	delay_ms(1);
	I2C_SCL = 0;
}

void I2C_NAck(void){
	SDA_OUT();
	I2C_SDA = 1;
	delay_ms(1);
	I2C_SCL = 1;
	delay_ms(1);
	I2C_SCL = 0;
}

/*
	Tham so ack : tin hieu ack tu master gui den slave (0: tiep tuc truyen, 1: dung truyen)
*/
u8 I2C_Read_Byte(unsigned char ack){
	SDA_OUT();
	I2C_SDA = 1; // san sang nhan du lieu
	SDA_IN();
	int i;
	u8 dat = 0;
	I2C_SCL = 0;
	for(i = 0; i < 8; i++){
		READ_SDA = 1; // cho SDA = 1 de san sang nhan du lieu
		delay_ms(1);
		I2C_SCL = 1;
		delay_ms(1);
		dat <<= 1;
		dat |= READ_SDA; // muon doc du lieu tu SDA
		I2C_SCL = 0;
	}
	// SDA = 0 -> ACK
	// SDA = 1 -> NACK
	if(ack == 0){
		I2C_Ack();
	}
	else{
		I2C_NAck();
	}
	return dat;
}

u8 I2C_Wait_Ack(void){
	SDA_IN();
	u8 result = 0;
	u8 time = 0;
	I2C_SDA = 1;
	delay_ms(1);
	I2C_SCL = 1;
	delay_ms(1);
	while(READ_SDA){ // muon doc du lieu tu SDA
		time++;
		if(time > 250){
			result = 1;
			break;
		}
	}
	I2C_SCL = 0;
	return result;
}
