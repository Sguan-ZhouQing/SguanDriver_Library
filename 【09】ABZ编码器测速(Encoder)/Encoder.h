#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32g4xx_hal.h"
#include "data_parameters.h"

typedef struct{
    float Rad;
    float Pos;
    float Speed;
    uint8_t Dir_n; // 与电机实际旋转方向相同，电机正向旋转
    float Kalman_R; // 噪声越大，参数越大
    float Kalman_Q; // 系统反应越快，参数越大
    float Offset; // 编码器偏置
}ENCODER_STRUCT;


void Encoder_Init(void);
float Encoder_GetRad(void);
float Encoder_GetPos(void);
float Encoder_FilterSpeed(void);
void Encoder_Tick(void);

#endif // ENCODER_H
