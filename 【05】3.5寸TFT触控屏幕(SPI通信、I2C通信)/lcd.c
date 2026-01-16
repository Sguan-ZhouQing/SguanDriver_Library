/*
 * @Author: 星必尘Sguan
 * @Date: 2025-04-27 21:49:28
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-07-09 11:49:44
 * @FilePath: \demo_STM32F407code\Hardware\lcd.c
 * @Description: [已完成]LCD屏幕显示的驱动程序
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 
#include "Yaocang.h"
#include "tim.h"

extern TIM_HandleTypeDef htim1;
extern uint16_t current_brightness_value;
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
extern DMA_HandleTypeDef hdma_spi1_tx;

//画笔颜色,背景颜色
uint16_t POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16_t DeviceCode;	 

/**
 * @brief 向LCD写入寄存器命令
 * @param data 要写入的寄存器命令值
 */
void LCD_WR_REG(uint8_t data)
{ 
	LCD_CS_CLR;     
	LCD_RS_CLR;	  
	SPI_WriteByte(SPI1,data);
	LCD_CS_SET;	
}

/**
 * @brief 向LCD写入数据
 * @param data 要写入的数据值
 */
void LCD_WR_DATA(uint8_t data)
{
	LCD_CS_CLR;
	LCD_RS_SET;
	SPI_WriteByte(SPI1,data);
	LCD_CS_SET;
}

/**
 * @brief 从LCD读取数据
 * @return 读取到的数据
 */
uint8_t LCD_RD_DATA(void)
{
	uint8_t data;
	LCD_CS_CLR;
	LCD_RS_SET;
	SPI_SetSpeed(SPI1,0);
	data = SPI_WriteByte(SPI1,0xFF);
	SPI_SetSpeed(SPI1,1);
	LCD_CS_SET;
	return data;
}

/**
 * @brief 向指定寄存器写入值
 * @param LCD_Reg 寄存器地址
 * @param LCD_RegValue 要写入的寄存器值
 */
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/**
 * @brief 读取指定寄存器的值
 * @param LCD_Reg 要读取的寄存器地址
 * @return 读取到的寄存器值
 */
uint8_t LCD_ReadReg(uint8_t LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);
	return LCD_RD_DATA();
}

/**
 * @brief 准备写入GRAM(显存)
 */
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/**
 * @brief 准备读取GRAM(显存)
 */
void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}	 

/**
 * @brief 向LCD写入16位数据
 * @param Data 要写入的16位数据
 */
void Lcd_WriteData_16Bit(uint16_t Data)
{	
	LCD_CS_CLR;
	LCD_RS_SET;
	SPI_WriteByte(SPI1,Data>>8);
	SPI_WriteByte(SPI1,Data);
	LCD_CS_SET;
}

/**
 * @brief 从LCD读取16位数据
 * @return 读取到的16位数据
 */
uint16_t Lcd_ReadData_16Bit(void)
{
	uint16_t r,g;
	LCD_CS_CLR;
	LCD_RS_CLR;	  
	SPI_WriteByte(SPI1,lcddev.rramcmd);
	SPI_SetSpeed(SPI1,0);
	LCD_RS_SET;
	SPI_WriteByte(SPI1,0xFF);
	r = SPI_WriteByte(SPI1,0xFF);
	g = SPI_WriteByte(SPI1,0xFF);
	SPI_SetSpeed(SPI1,1);
	LCD_CS_SET;
	r<<=8;
	r|=g;
	return r;
}

/**
 * @brief 在指定坐标画点
 * @param x X坐标
 * @param y Y坐标
 */
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/**
 * @brief 读取指定坐标点的颜色值
 * @param x X坐标
 * @param y Y坐标
 * @return 该点的颜色值
 */
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
	uint16_t color;
	LCD_SetCursor(x,y);//设置光标位置 
	color = Lcd_ReadData_16Bit();
	return color;
}

/**
 * @brief 清屏函数
 * @param Color 清屏颜色
 */
void LCD_Clear(uint16_t Color)
{
	unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
			SPI_WriteByte(SPI1,Color>>8);
			SPI_WriteByte(SPI1,Color);
		}
	}
	LCD_CS_SET;
} 

/**
 * @brief LCD GPIO初始化
 */
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	      
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin =  LCD_RST_Pin| LCD_RS_Pin| LCD_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//普通输出模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;//上拉
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// LCD_LED=1;  //点亮背光
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_brightness_value);
}

/**
 * @brief LCD复位函数
 */
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}

/**
 * @brief LCD初始化函数
 * 包含SPI初始化、GPIO初始化、复位和寄存器配置
 */
void LCD_Init(void)
{  
	// SPI1_Init(); //硬件SPI初始化
	LCD_GPIOInit();//LCD GPIO初始化										 
 	LCD_RESET(); //LCD 复位
//*************3.5 ST7796S IPS初始化**********//	
	LCD_WR_REG(0x11);     

	delay_ms(120);                //Delay 120ms

	LCD_WR_REG(0x36);     // Memory Data Access Control MY,MX~~
	LCD_WR_DATA(0x48);   

	LCD_WR_REG(0x3A);     
	LCD_WR_DATA(0x55);   

	LCD_WR_REG(0xF0);     // Command Set Control
	LCD_WR_DATA(0xC3);   

	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x96);   

	LCD_WR_REG(0xB4);     
	LCD_WR_DATA(0x02);   

	LCD_WR_REG(0xB7);     
	LCD_WR_DATA(0xC6);   

	LCD_WR_REG(0xC0);     
	LCD_WR_DATA(0xC0);   
	LCD_WR_DATA(0x00);   

	LCD_WR_REG(0xC1);     
	LCD_WR_DATA(0x13);   

	LCD_WR_REG(0xC2);     
	LCD_WR_DATA(0xA7);   

	LCD_WR_REG(0xC5);     
	LCD_WR_DATA(0x21);   

	LCD_WR_REG(0xE8);     
	LCD_WR_DATA(0x40);   
	LCD_WR_DATA(0x8A);   
	LCD_WR_DATA(0x1B);   
	LCD_WR_DATA(0x1B);   
	LCD_WR_DATA(0x23);   
	LCD_WR_DATA(0x0A);   
	LCD_WR_DATA(0xAC);   
	LCD_WR_DATA(0x33);   

	LCD_WR_REG(0xE0);     
	LCD_WR_DATA(0xD2);   
	LCD_WR_DATA(0x05);   
	LCD_WR_DATA(0x08);   
	LCD_WR_DATA(0x06);   
	LCD_WR_DATA(0x05);   
	LCD_WR_DATA(0x02);   
	LCD_WR_DATA(0x2A);   
	LCD_WR_DATA(0x44);   
	LCD_WR_DATA(0x46);   
	LCD_WR_DATA(0x39);   
	LCD_WR_DATA(0x15);   
	LCD_WR_DATA(0x15);   
	LCD_WR_DATA(0x2D);   
	LCD_WR_DATA(0x32);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA(0x96);   
	LCD_WR_DATA(0x08);   
	LCD_WR_DATA(0x0C);   
	LCD_WR_DATA(0x09);   
	LCD_WR_DATA(0x09);   
	LCD_WR_DATA(0x25);   
	LCD_WR_DATA(0x2E);   
	LCD_WR_DATA(0x43);   
	LCD_WR_DATA(0x42);   
	LCD_WR_DATA(0x35);   
	LCD_WR_DATA(0x11);   
	LCD_WR_DATA(0x11);   
	LCD_WR_DATA(0x28);   
	LCD_WR_DATA(0x2E);   

	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x3C);   
	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x69);  

	delay_ms(120);                
	LCD_WR_REG(0x21);     
	LCD_WR_REG(0x29);

	LCD_direction(USE_HORIZONTAL);//设置LCD显示方向 
	LCD_Clear(WHITE);//清全屏白色
}

/**
 * @brief 设置显示窗口区域
 * @param xStar 起始X坐标
 * @param yStar 起始Y坐标
 * @param xEnd 结束X坐标
 * @param yEnd 结束Y坐标
 */
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/**
 * @brief 设置光标位置
 * @param Xpos X坐标
 * @param Ypos Y坐标
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/**
 * @brief 设置LCD显示方向
 * @param direction 方向参数(0-3)
 */
void LCD_direction(uint8_t direction)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	lcddev.rramcmd=0x2E;
	lcddev.dir = direction%4;
	switch(lcddev.dir){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(1<<6));
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
		break;	
		default:break;
	}		
}	 

/**
 * @brief 读取LCD ID
 * @return LCD的ID值
 */
uint16_t LCD_Read_ID(void)
{
	uint8_t i,val[3] = {0};
	LCD_WR_REG(0xF0);     // Command Set Control
	LCD_WR_DATA(0xC3);   
	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x96);  
	LCD_CS_CLR;
	for(i=1;i<4;i++)
	{
		LCD_RS_CLR;	  
		SPI_WriteByte(SPI1,0xFB);
		LCD_RS_SET;
		SPI_WriteByte(SPI1,0x10+i);
		LCD_RS_CLR;	  
		SPI_WriteByte(SPI1,0xD3);
		SPI_SetSpeed(SPI1,0);
		LCD_RS_SET;
		val[i-1] = SPI_WriteByte(SPI1,0xFF);
		SPI_SetSpeed(SPI1,1);
		LCD_RS_CLR;	  
		SPI_WriteByte(SPI1,0xFB);
		LCD_RS_SET;
		SPI_WriteByte(SPI1,0x00);	
	}
	LCD_CS_SET;
	LCD_WR_REG(0xF0);     // Command Set Control
	LCD_WR_DATA(0x3C);   
	LCD_WR_REG(0xF0);     
	LCD_WR_DATA(0x69);  
	lcddev.id=val[1];
	lcddev.id<<=8;
	lcddev.id|=val[2];
	return lcddev.id;
}


/**
 * @brief 使用DMA填充颜色区域
 * @param x1 区域起始X坐标
 * @param y1 区域起始Y坐标
 * @param x2 区域结束X坐标
 * @param y2 区域结束Y坐标
 * @param color_p 颜色数据数组(RGB565格式)
 */
void lcd_color_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color_p)
{
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    LCD_SetWindows(x1, y1, x2, y2);
    LCD_CS_CLR;
    LCD_RS_SET;
    
    // 定义缓冲区大小（根据可用内存调整）
    #define BUF_SIZE 256
    uint8_t buffer[BUF_SIZE * 2];
    uint32_t processed = 0;
    
    while(processed < size) {
        uint32_t chunk = (size - processed) > BUF_SIZE ? BUF_SIZE : (size - processed);
        // 预处理数据到缓冲区
        for(uint32_t i = 0; i < chunk; i++) {
            buffer[i*2] = color_p[processed + i] >> 8;    // 高字节
            buffer[i*2+1] = color_p[processed + i] & 0xFF; // 低字节
        }
        // 批量传输
        HAL_SPI_Transmit_DMA(&hspi1, buffer, chunk * 2);
        // 等待传输完成
        while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
        processed += chunk;
    }
    LCD_CS_SET;
}
