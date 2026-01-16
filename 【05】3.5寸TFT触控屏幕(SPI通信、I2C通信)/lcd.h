/*
 * @Author: 星必尘Sguan
 * @Date: 2025-04-27 21:49:28
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-13 16:21:00
 * @FilePath: \test_SPIscreen\Hardware\lcd.h
 * @Description: 
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"
#include "stm32f4xx_hal.h"       // 替换为具体型号（如 stm32f4xx_hal.h）
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "mySPI.h"


typedef struct  
{										    
	uint16_t width;			
	uint16_t height;			
	uint16_t id;				
	uint8_t  dir;			
	uint16_t	 wramcmd;		
	uint16_t  rramcmd;   
	uint16_t  setxcmd;		
	uint16_t  setycmd;		
}_lcd_dev; 	

extern _lcd_dev lcddev;	

#define USE_HORIZONTAL  	 3 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

#define LCD_W 320
#define LCD_H 480

extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR; 

// #define LED  13        

// #define	LCD_LED PBout(LED) 
#define GPIO_TYPE GPIOB 		 
#define LCD_CS  15
#define LCD_RS  14
#define LCD_RST 12

#define LCD_CS_PIN   GPIO_PIN_15
#define LCD_RS_PIN   GPIO_PIN_14
#define LCD_RST_PIN  GPIO_PIN_12
#define LCD_GPIO     GPIOB

#define LCD_CS_SET   HAL_GPIO_WritePin(LCD_GPIO, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_RS_SET   HAL_GPIO_WritePin(LCD_GPIO, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RST_SET  HAL_GPIO_WritePin(LCD_GPIO, LCD_RST_PIN, GPIO_PIN_SET)

#define LCD_CS_CLR   HAL_GPIO_WritePin(LCD_GPIO, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_RS_CLR   HAL_GPIO_WritePin(LCD_GPIO, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_RST_CLR  HAL_GPIO_WritePin(LCD_GPIO, LCD_RST_PIN, GPIO_PIN_RESET)

#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40 
#define BRRED 		0XFC07 
#define GRAY  		0X8430 

#define DARKBLUE    0X01CF	
#define LIGHTBLUE   0X7D7C	
#define GRAYBLUE    0X5458 
 
#define LIGHTGREEN  0X841F 
#define LIGHTGRAY   0XEF5B 
#define LGRAY 		0XC618 

#define LGRAYBLUE   0XA651 
#define LBBLUE      0X2B12 
	    															  
void LCD_Init(void);
void LCD_Clear(uint16_t Color);	 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y);    
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

uint8_t LCD_RD_DATA(void);
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_DATA(uint8_t data);
uint8_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction );
uint16_t LCD_Read_ID(void);
void lcd_color_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color_p);

#endif  
