/*
 * @Author: 星必尘Sguan
 * @Date: 2026-01-14 12:47:20
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2026-01-15 14:13:01
 * @FilePath: \SguanDriver库\【05】3.5寸TFT触控屏幕(SPI通信、I2C通信)\delay.c
 * @Description: 
 * 
 * Copyright (c) 2026 by $JUST, All Rights Reserved. 
 */
// delay.c (HAL 简化版)
#include "stm32f4xx_hal.h"

// 微秒延时（循环空指令实现）
void delay_us(uint32_t us) {
    uint32_t start = HAL_GetTick() * 1000;  // 当前时间（微秒）
    while ((HAL_GetTick() * 1000 - start) < us) { /* 等待 */ }
}

// 毫秒延时直接调用 HAL
// 注：无需实现 delay_ms，直接用 HAL_Delay()
