#ifndef _PORT_H_
#define _PORT_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define LCD_PORT	GPIOA
#define EN_LCD_PIN	GPIO_Pin_2
#define RS_LCD_PIN	GPIO_Pin_0
#define RW_LCD_PIN	GPIO_Pin_1


#define DATA_LCD_PORT	GPIOA
#define D4_LCD				GPIO_Pin_4
#define D5_LCD				GPIO_Pin_5
#define D6_LCD				GPIO_Pin_6
#define D7_LCD				GPIO_Pin_7


#ifdef __cplusplus
}
#endif

#endif 