#include "main.h"

// Bien xu li chuoi
static char c = 0;
static uint8_t start = 0;
static uint8_t available = 0;
static uint8_t i = 0;
static uint8_t count = 0;
static char msg[16];

// Bien xu li lenh
static uint16_t timeT = 500;
static uint16_t timeP = 500;
static uint16_t timeH = 500;

static uint16_t storeT = 500;
static uint16_t storeP = 500;
static uint16_t storeH = 500;

// Bien thoi gian lay mau thu
 uint16_t tT = 0;
 uint16_t tP = 0;
 uint16_t tH = 0;

// Bien tat bat sensor
static uint8_t sensor1 = 1;
static uint8_t sensor2 = 1;
static uint8_t written1 = 0;
static uint8_t written2 = 0;

// Bien chua gia tri
static unsigned int temperature = 0;
static unsigned int humidity = 0;
static double temp, humi;
static int32_t pressure;

void USART1_IRQHandler(void);
void handleInput(void);

int main(void){

	
	GPIO_Configuration();
	delay_init();
	delay_ms(1);
	LCD16X2_Init();
	delay_ms(1);
	I2c_Init(I2C1, Pin_PB6PB7, I2C_CLOCK_STANDARD);
	delay_ms(1);
	USART1_Init();
	// I2c_Init(I2C2, Pin_PB10PB11, I2C_CLOCK_STANDARD);
	// I2c_Init();
	
	
	pinMode(PB0, OUTPUT);
	pinMode(PB1, OUTPUT);
	delay_ms(1);
	digitalWrite(PB0, LOW);
	digitalWrite(PB1, LOW);
	delay_ms(1);
	LCD16X2_Clear();
	delay_ms(1);
	BMP180_UpdateCalibrationData();
	delay_ms(10);
	while(1){
		if(available){
			handleInput();
			available = 0;
		}
		if(sensor1){
			if(tT == 0){
				SHT3x_Read_T_H(I2C1, &temperature, &humidity);
				temp = -45 + 175 * temperature / 65535;
				LCD16X2_Gotoxy(0,0);
				LCD16X2_PutChar('T');
				LCD16X2_PutChar(':');
				LCD16X2_PutChar((int32_t)temp / 100 + 0x30);
				LCD16X2_PutChar((int32_t)temp / 10 % 10 + 0x30);
				LCD16X2_PutChar((int32_t)temp % 10 + 0x30);
				LCD16X2_PutChar(0xDF);
				LCD16X2_PutChar('C');
				
				USART1_PutString("T: ");
				USART1_PutChar((int32_t)temp / 100 + 0x30);
				USART1_PutChar((int32_t)temp / 10 % 10 + 0x30);
				USART1_PutChar((int32_t)temp % 10 + 0x30);
				USART1_PutChar('\n');
			}
			
			if(tH == 0){
				SHT3x_Read_T_H(I2C1, &temperature, &humidity);
				humi = 100 * humidity / 65535;
				LCD16X2_Gotoxy(9,0);
				LCD16X2_PutChar('H');
				LCD16X2_PutChar(':');
				LCD16X2_PutChar((int32_t)humi / 100 + 0x30);
				LCD16X2_PutChar((int32_t)humi / 10 % 10 + 0x30);
				LCD16X2_PutChar((int32_t)humi % 10 + 0x30);
				LCD16X2_PutChar('%');
				
				USART1_PutString("H: ");
				USART1_PutChar((int32_t)humi / 100 + 0x30);
				USART1_PutChar((int32_t)humi / 10 % 10 + 0x30);
				USART1_PutChar((int32_t)humi % 10 + 0x30);
				USART1_PutChar('\n');
			}
			tT ++;
			tH ++;
		}
		else{
			if(written1 == 0){
				LCD16X2_Gotoxy(0,0);
				LCD16X2_PutChar('T');
				LCD16X2_PutChar(':');
				LCD16X2_PutChar('O');
				LCD16X2_PutChar('F');
				LCD16X2_PutChar('F');
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar(' ');
				
				LCD16X2_Gotoxy(9,0);
				LCD16X2_PutChar('H');
				LCD16X2_PutChar(':');
				LCD16X2_PutChar('O');
				LCD16X2_PutChar('F');
				LCD16X2_PutChar('F');
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar(' ');
				tT = 0;
				tH = 0;
				written1 = 1;
			}
		}
		
		if(sensor2){
			if(tP == 0){
				pressure = BMP180_GetPressure();
				LCD16X2_Gotoxy(1,1);
				LCD16X2_PutString("Press:");
				LCD16X2_PutChar(pressure / 100000 + 0x30);
				LCD16X2_PutChar(pressure / 10000 % 10+ 0x30);
				LCD16X2_PutChar(pressure / 1000 % 10 + 0x30);
				LCD16X2_PutChar(pressure / 100 % 10 + 0x30);
				LCD16X2_PutChar('.');
				LCD16X2_PutChar(pressure / 10 % 10 + 0x30);
				LCD16X2_PutChar(pressure % 10 + 0x30);
				LCD16X2_PutString("hPa");
				
				USART1_PutString("Press: ");
				USART1_PutChar(pressure / 100000 + 0x30);
				USART1_PutChar(pressure / 10000 % 10+ 0x30);
				USART1_PutChar(pressure / 1000 % 10 + 0x30);
				USART1_PutChar(pressure / 100 % 10 + 0x30);
				USART1_PutChar('.');
				USART1_PutChar(pressure / 10 % 10 + 0x30);
				USART1_PutChar(pressure % 10 + 0x30);
				USART1_PutChar('\n');
			}
			tP ++;
		}
		else{
			if(written2 == 0){
				LCD16X2_Gotoxy(1,1);
				LCD16X2_PutString("Press:");
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar('O');
				LCD16X2_PutChar('F');
				LCD16X2_PutChar('F');
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar(' ');
				LCD16X2_PutChar(' ');
				tP = 0;
				written2 = 1;
			}
		}
		
		if(tT >= timeT){
			tT = 0;
		}
		
		if(tH >= timeH){
			tH = 0;
		}
		
		if(tP >= timeP){
			tP = 0;
		}
		delay_ms(10);
	}
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){
		c = USART1_GetChar();
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '/'){
			if(c == 's' && start == 0){
				start = 1;
				i = 0;
				count = 0;
			}
			else if(start == 1 && c == 'e'){
				start = 0;
				i = 0;
				available = 1;
			}
			else if(start == 1){
				msg[i] = c;
				i ++;
				count ++;
			}
		}
	}
}

void handleInput(void){
	// Bat tat sensor
	if(count <= 2){
		if(msg[0] == 'B'){
			if(msg[1] == '1'){
				digitalWrite(PB0, LOW);
				sensor1 = 1;
				SHT3x_Read_T_H(I2C1, &temperature, &humidity);
				timeT = storeT;
				timeH = storeH;
			}
			else{
				digitalWrite(PB1, LOW);
				sensor2 = 1;
				BMP180_UpdateCalibrationData();
				timeP = storeP;
			}
		}
		else{
			if(msg[1] == '1'){
				digitalWrite(PB0, HIGH);
				sensor1 = 0;
				written1 = 0;
				tT = 0;
				tH = 0;
			}
			else{
				digitalWrite(PB1, HIGH);
				sensor2 = 0;
				written2 = 0;
				tP = 0;
			}
		}
	}
	// Xu li lenh lay mau
	else{
		uint8_t sT = 0;
		uint8_t sP = 0;
		uint8_t sH = 0;
		uint8_t index = 0;
		timeT = 0;
		timeP = 0;
		timeH = 0;
		for(index = 0; index < count; index++){
			if(sT == 1 && (msg[index] >= '0' && msg[index] <= '9')){
				if(timeT == 0){
					timeT = msg[index] - '0';
				}
				else{
					timeT *= 10;
					timeT += msg[index] - '0';
				}
			}
			if(sP == 1 && (msg[index] >= '0' && msg[index] <= '9')){
				if(timeP == 0){
					timeP = msg[index] - '0';
				}
				else{
					timeP *= 10;
					timeP += msg[index] - '0';
				}
			}
			if(sH == 1 && (msg[index] >= '0' && msg[index] <= '9')){
				if(timeH == 0){
					timeH = msg[index] - '0';
				}
				else{
					timeH *= 10;
					timeH += msg[index] - '0';
				}
			}
			
			
			if(msg[index] == 'T'){
				sT = 1;
				sP = 0;
				sH = 0;
			}
			else if(msg[index] == 'P'){
				sT = 0;
				sP = 1;
				sH = 0;
			}
			else if(msg[index] == 'H'){
				sT = 0;
				sP = 0;
				sH = 1;
			}
		}
		
		if(timeT != 0){
			timeT = timeT < 1000 ? timeT*100 : storeT;
			storeT = timeT;
		}
		else{
			timeT = storeT;
		}
		
		if(timeH != 0){
			timeH = timeH < 1000 ? timeH*100 : storeH;
			storeH = timeH;
		}
		else{
			timeH = storeH;
		}
		
		if(timeP != 0){
			timeP = timeP < 1000 ? timeP*100 : storeP;
			storeP = timeP;
		}
		else{
			timeP = storeP;
		}
		
		tT = 0;
		tH = 0;
		tP = 0;
		
	}
}
