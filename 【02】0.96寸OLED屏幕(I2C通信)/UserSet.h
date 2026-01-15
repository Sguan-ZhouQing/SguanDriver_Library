#ifndef __USERSET_H
#define __USERSET_H

// 单片机驱动声明,例如:
#include "stm32f4xx_hal.h"

// 单片机的函数调用重写:
void I2C_SendData(uint8_t ADDRESS, uint8_t *cmd_data, uint8_t num){
    HAL_I2C_Master_Transmit(&hi2c1, ADDRESS, cmd_data, num, HAL_MAX_DELAY);
}

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
