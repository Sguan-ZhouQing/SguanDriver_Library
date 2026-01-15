#ifndef __USERSET_H
#define __USERSET_H

// 单片机驱动声明,例如:
#include "stm32f4xx_hal.h"

// 单片机引脚使用修改,例如:
#define SDA_Port	GPIOA
#define SCL_Port    GPIOB
#define SDA_Pin		GPIO_PIN_11
#define SCL_Pin		GPIO_PIN_12

// 单片机的函数调用重写（GPIO开漏输出）:
void GPIO_SDA_SetBits(uint8_t BitValue){
    // GPIO输出高低电平
    HAL_GPIO_WritePin(SDA_Port, SDA_Pin, (GPIO_PinState)BitValue);
}
void GPIO_SCL_SetBits(uint8_t BitValue){
    // GPIO输出高低电平
    HAL_GPIO_WritePin(SCL_Port, SCL_Pin, (GPIO_PinState)BitValue);
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
