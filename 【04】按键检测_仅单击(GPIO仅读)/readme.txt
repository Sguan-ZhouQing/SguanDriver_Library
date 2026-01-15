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

/*
按照UserSet.h里面的要求，依次修改驱动声明和引脚定义、函数编写...就可以让驱动跑起来了
According to the requirements in UserSet.h, 
you just need to modify the driver declarations and pin definitions, 
write the functions one by one... 
and the driver will be able to run.
*/
