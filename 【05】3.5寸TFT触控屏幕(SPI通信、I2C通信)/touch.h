/*
 * @Author: 星必尘Sguan
 * @Date: 2026-01-14 12:47:20
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-01-16 02:58:18
 * @FilePath: \SguanDriver库\【05】3.5寸TFT触控屏幕(SPI通信、I2C通信)\touch.h
 * @Description: 
 * 
 * Copyright (c) 2026 by $JUST, All Rights Reserved. 
 */
#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"
#include "ft6336.h"
#include "stm32f4xx_hal.h"

#define TP_PRES_DOWN 0x80  
#define TP_CATH_PRES 0x40  
										    
typedef struct
{
	uint8_t (*init)(void);			
	uint8_t (*scan)(void);				
	uint16_t x[CTP_MAX_TOUCH]; 		
	uint16_t y[CTP_MAX_TOUCH];		
	uint8_t  sta;					
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	
    
uint8_t TP_Init(void);								
 		  
#endif
