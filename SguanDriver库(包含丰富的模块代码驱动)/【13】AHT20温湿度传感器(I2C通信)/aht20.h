/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-20 13:08:06
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-07-08 03:01:16
 * @FilePath: \demo_STM32F407code\Hardware\aht20.h
 * @Description: 
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#ifndef __AHT20_H
#define __AHT20_H

#include "stm32f4xx_hal.h"

typedef struct {
    float AHT20_Temp;
    float AHT20_Humi;
} AHT20_t;


void AHT20_Init(void);
void AGT20_Upadte_Tick(void);


#endif
