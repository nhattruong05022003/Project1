#ifndef _SOFT_I2C_
#define _SOFT_I2C_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

// IO (doi chan tai day) (CRL(32 bit) : pin 0 ->7, CRH(32 bit) : pin 8 -> 15) (moi pin co 4 bit de config pin)
// Xem thanh ghi trong file Thanh ghi CRL, CRH

// PB9IN (input pull up, pull down)
#define SDA_IN()   {GPIOB->CRH&=0xFFFFFF0F/*xoa 4 bit cau hinh cho Pin9*/;GPIOB->CRH|=8<<4/*1000 dich trai 4 bit*/;} 
// PB9OUT (output push-pull maxspeed 50MHz)
#define SDA_OUT()  {GPIOB->CRH&=0xFFFFFF0F/*xoa 4 bit cau hinh cho Pin9*/;GPIOB->CRH|=3<<4/*11 dich trai 4 bit*/;}


#define I2C_SCL PBout(8) // thu vien sys.h
#define I2C_SDA PBout(9) // thu vien sys.h
// READ_SDA dung khi muon doc du lieu tu chan SDA
#define READ_SDA PBin(9) // thu vien sys.h 


///////////////////////////////////////////////////////////
// Khoi tao I2C
void I2c_Init(void);
// Start
void I2C_Start(void);
// Stop
void I2C_Stop(void);
// Master gui 1 byte den slave
void I2C_Send_Byte(u8 txd);
// Master doc 1 byte tu slave gui den
u8 I2C_Read_Byte(unsigned char ack);
// Master cho slave gui ack den
u8 I2C_Wait_Ack(void);
// Master gui ack den slave
void I2C_Ack(void);
// Master gui nack den slave
void I2C_NAck(void);
///////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif