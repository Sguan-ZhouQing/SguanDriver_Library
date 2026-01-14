/*
 * @Author: 星必尘Sguan
 * @Date: 2025-03-27 11:10:16
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-03-27 11:43:59
 * @FilePath: \demo_MyCar\Hardware\sr04.c
 * @Description: SR04超声波测距模块函数
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "sr04.h"

extern TIM_HandleTypeDef htim3;

#define SR04_Output GPIO_PIN_3
#define SR04_Output_Port GPIOA
#define SR04_Input GPIO_PIN_2
#define SR04_Input_Port GPIOA

uint16_t SR04_Count;
float SR04_Distance;


/**
 * @description: 指令耗时RCC延时函数(微妙us级)
 * @param {uint32_t} udelay
 * @return {*}
 */
void RCCdelay_us(uint32_t udelay)
{
  __IO uint32_t Delay = udelay * 72 / 8;//(SystemCoreClock / 8U / 1000000U)
  do
  {
    __NOP();
  }
  while (Delay --);
}



/**
 * @description: 启动SR04模块并开始发送信号
 * @return {*}
 */
void Get_Distance(void)
{
    HAL_GPIO_WritePin(SR04_Output_Port,SR04_Output,GPIO_PIN_SET);
    RCCdelay_us(12);
    HAL_GPIO_WritePin(SR04_Output_Port,SR04_Output,GPIO_PIN_RESET);
}



/**
 * @description: 重写中断计数的回调函数，并获取现在的距离
 * @param {uint16_t} GPIO_Pin
 * @return {*}
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (HAL_GPIO_ReadPin(SR04_Input_Port,SR04_Input)==GPIO_PIN_SET)
    {
        __HAL_TIM_SetCounter(&htim3,0);
        HAL_TIM_Base_Start(&htim3);
    }
    else
    {
        HAL_TIM_Base_Stop(&htim3);
        SR04_Count=__HAL_TIM_GetCounter(&htim3);
        SR04_Distance=SR04_Count*0.017;
    }
}


