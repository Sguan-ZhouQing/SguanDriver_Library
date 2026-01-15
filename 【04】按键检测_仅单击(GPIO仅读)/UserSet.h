#ifndef __USERSET_H
#define __USERSET_H

// 单片机驱动声明,例如:
#include "stm32f4xx_hal.h"

// 单片机引脚使用修改,例如:
#define Key1_GPIO_Port GPIOB
#define Key1_Pin GPIO_PIN_10
#define Key2_GPIO_Port GPIOB
#define Key2_Pin GPIO_PIN_11
#define Key3_GPIO_Port GPIOB
#define Key3_Pin GPIO_PIN_12

// 单片机的函数调用重写:
uint8_t GPIO_GetState(void){
    if (HAL_GPIO_ReadPin(Key1_GPIO_Port,Key1_Pin) == 1){
        return 1;
    }
    if (HAL_GPIO_ReadPin(Key2_GPIO_Port,Key2_Pin) == 1){
        return 2;
    }
    if (HAL_GPIO_ReadPin(Key3_GPIO_Port,Key3_Pin) == 1){
        return 3;
    }
    return 0;
}

/* 核心函数使用方法示例
#include "Key.h"

int main(){
    while (1){
        uint8_t num = Key_GetNum();
        switch (num)
        {
        case 1:

            break;
        case 2:
        
            break;
        case 3:
        
            break;
        default:
            break;
        }
    }
}

void TIM_IRQHandler(void){
    Key_Tick();
}
*/
#endif // USERSET_H
