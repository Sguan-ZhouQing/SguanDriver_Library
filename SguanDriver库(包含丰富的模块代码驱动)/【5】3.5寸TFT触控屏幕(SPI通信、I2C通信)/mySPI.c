/*
 * @Author: 星必尘Sguan
 * @Date: 2025-04-27 21:49:22
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-21 15:04:18
 * @FilePath: \demo_STM32F407code\Hardware\mySPI.c
 * @Description: [已完成]硬件SPI函数的编写
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "mySPI.h"

uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte)
{
    // HAL_SPI_TransmitReceive(&hspi1, &Byte, &rx_data, 1, HAL_MAX_DELAY);
	uint8_t rx_data;
    HAL_SPI_TransmitReceive_DMA(&hspi1, &Byte, &rx_data, 1);
    // HAL_SPI_Transmit_DMA(&hspi1, &Byte, 1);
    return rx_data;
}

void SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t SpeedSet)
{
	SPIx->CR1 &= ~SPI_CR1_BR;  // 清除原有分频设置
    if(SpeedSet == 1) {
        SPIx->CR1 |= SPI_BAUDRATEPRESCALER_2;  // 高速模式
    } else {
        // SPIx->CR1 |= SPI_BAUDRATEPRESCALER_16; // 低速模式
        SPIx->CR1 |= SPI_BAUDRATEPRESCALER_2;  // 高速模式
    }
    SPIx->CR1 |= SPI_CR1_SPE;  // 重新使能SPI
}

void SPI1_Init(void)	
{
	__HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // GPIO初始化（CubeMX生成）
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // SPI参数配置
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.CRCPolynomial = 7;
    HAL_SPI_Init(&hspi1);
}
