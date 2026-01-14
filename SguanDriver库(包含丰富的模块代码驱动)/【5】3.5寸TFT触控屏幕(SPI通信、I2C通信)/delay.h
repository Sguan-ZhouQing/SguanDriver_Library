// delay.h
#pragma once
#include "main.h"

void delay_us(uint32_t us);
#define delay_ms(ms) HAL_Delay(ms)  // ºêÌæ»»
