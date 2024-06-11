#ifndef __LCD16x2_H
#define __LCD16x2_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"	
#include "port.h"

/*
	KHAI BÁO CÁC HÀM CHO LCD
*/
void LCD16X2_Init ( void );      								//ham khoi tao LCD16x2
void LCD16X2_Enable(void);       								//ham tao xung EN
void LCD16X2_Clear(void);            						//xoa noi dung hien thi tren LCD16x2
void LCD16X2_Send4Bit(uint8_t data );						//ham goi du lieu 4 bit den LCD16x2
void LCD16X2_SendCommand (uint8_t  command,uint8_t reg );//Goi 1 byte du lieu den LCD
void LCD16X2_Gotoxy(uint8_t  x, uint8_t  y);			//ham thiet lap vi tri con tro
void LCD16X2_PutChar ( uint8_t  data );					//ham viet mot ky tu len LCD16x2
void LCD16X2_PutString (uint8_t *s); 						// hien thi chuoi ky tu ra LCD     
void LCD16X2_NumBer(uint32_t number,uint8_t leng);//ham viet mot chuoi so len LCD16x2
void LCD16X2_controblink_xy(uint8_t x, uint8_t y);	//hàm hien thi con tro nhap nhay
void LCD16X2_contro_off_xy(uint8_t x, uint8_t y);	//hàm tat con tro nhap nhay
#ifdef __cplusplus
}
#endif

#endif
