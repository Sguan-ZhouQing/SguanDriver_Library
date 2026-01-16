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
