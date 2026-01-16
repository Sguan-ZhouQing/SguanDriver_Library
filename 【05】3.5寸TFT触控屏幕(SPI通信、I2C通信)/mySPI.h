/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-20 13:08:06
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-07-07 12:45:13
 * @FilePath: \demo_STM32F407code\Hardware\mySPI.h
 * @Description: 
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#ifndef _SPI_H_
#define _SPI_H_

#include "spi.h"
#include "sys.h"
#include "stm32f4xx_hal.h"

//除了SPI时钟信号以及SPI读、写信号引脚不可更改，其他引脚都可以更改
//SPI的时钟引脚定义固定为PB3
//SPI的读数据引脚定义固定为PB4
//SPI的写数据引脚定义固定为PB5
 
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte);
void SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t SpeedSet);
void SPI1_Init(void);


#endif
