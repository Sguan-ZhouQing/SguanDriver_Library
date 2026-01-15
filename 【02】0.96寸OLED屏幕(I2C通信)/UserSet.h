#ifndef __USERSET_H
#define __USERSET_H

// 单片机驱动声明,例如:
#include "stm32f4xx_hal.h"

// 单片机的函数调用重写:
void I2C_SendData(uint8_t ADDRESS, uint8_t *cmd_data, uint8_t num){
    HAL_I2C_Master_Transmit(&hi2c1, ADDRESS, cmd_data, num, HAL_MAX_DELAY);
}

/* 核心函数使用方法示例
#include "OLED.h"

int main(){
    OLED_Init();
    while (1){
        OLED_ShowString(0,0,"Hello World!",OLED_OLED_8X16);
        OLED_Update();
    }
}
*/
#endif // USERSET_H
