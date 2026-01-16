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

/*
按照UserSet.h里面的要求，依次修改驱动声明和引脚定义、函数编写...就可以让驱动跑起来了
According to the requirements in UserSet.h, 
you just need to modify the driver declarations and pin definitions, 
write the functions one by one... 
and the driver will be able to run.
*/
