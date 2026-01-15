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

/*
按照UserSet.h里面的要求，依次修改驱动声明和引脚定义、函数编写...就可以让驱动跑起来了
According to the requirements in UserSet.h, 
you just need to modify the driver declarations and pin definitions, 
write the functions one by one... 
and the driver will be able to run.
*/

/*
基于江协科技的OLED源代码改写，所有版权解释权归江协科技所有。
本此驱动函数修改，只是在原驱动基础上，添加了适配其他mcu开发的函数接口
*/
