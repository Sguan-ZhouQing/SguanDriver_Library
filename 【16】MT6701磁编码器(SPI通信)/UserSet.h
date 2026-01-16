#ifndef __USERSET_H
#define __USERSET_H

// 单片机驱动声明,例如:
#include "stm32f4xx_hal.h"

// 单片机引脚使用修改,例如:
#define IRQ_Port	GPIOA
#define CE_Port		GPIOB
#define CSN_Port	GPIOB
#define IRQ_Pin		GPIO_PIN_8
#define CE_Pin		GPIO_PIN_11
#define CSN_Pin		GPIO_PIN_12

// 单片机的函数调用重写:
uint8_t GPIO_IRQ_ReadBits(void){
    // GPIO读取高低电平
    return HAL_GPIO_WritePin(IRQ_Port, IRQ_Pin);
}
void GPIO_CE_SetBits(uint8_t BitValue){
    // GPIO输出高低电平
    HAL_GPIO_WritePin(CE_Port, CE_Pin, (GPIO_PinState)BitValue);
}
void GPIO_CSN_SetBits(uint8_t BitValue){
    // GPIO输出高低电平
    HAL_GPIO_WritePin(CSN_Port, CSN_Pin, (GPIO_PinState)BitValue);
}
void SPI_SwapData(uint8_t Byte, uint8_t *rxData){
    HAL_SPI_TransmitReceive(&hspi2, &Byte, rxData, 1, HAL_MAX_DELAY);
}

// 通信密钥修改
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/* 核心函数使用方法示例
#include "nRF_Printf.h"
#include "nRF24L01.h"

int main(){
    NRF24L01_Init();
    while (1){
        nRF_Printf("Hello World!");
    }
}

void TIM_IRQHandler(void){
    nRF_Tick();
}
*/
#endif // USERSET_H
