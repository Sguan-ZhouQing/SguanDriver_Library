#ifndef PCA9685_H
#define PCA9685_H

#include "stm32g4xx_hal.h"

// PCA9685默认地址(A5-A0 = 0)
#define PCA9685_ADDRESS 0x40

// 寄存器地址
#define PCA9685_MODE1_REG      0x00
#define PCA9685_MODE2_REG      0x01
#define PCA9685_SUBADR1_REG    0x02
#define PCA9685_SUBADR2_REG    0x03
#define PCA9685_SUBADR3_REG    0x04
#define PCA9685_ALLCALL_REG    0x05
#define PCA9685_LED0_ON_L_REG  0x06
#define PCA9685_LED0_ON_H_REG  0x07
#define PCA9685_LED0_OFF_L_REG 0x08
#define PCA9685_LED0_OFF_H_REG 0x09
#define PCA9685_PRESCALE_REG   0xFE

// Mode1寄存器位定义
#define PCA9685_RESTART 0x80
#define PCA9685_EXTCLK  0x40
#define PCA9685_AI      0x20
#define PCA9685_SLEEP   0x10
#define PCA9685_SUB1    0x08
#define PCA9685_SUB2    0x04
#define PCA9685_SUB3    0x02
#define PCA9685_ALLCALL 0x01

// Mode2寄存器位定义
#define PCA9685_INVRT   0x10
#define PCA9685_OCH     0x08
#define PCA9685_OUTDRV  0x04
#define PCA9685_OUTNE0  0x01
#define PCA9685_OUTNE1  0x02

// LED输出控制位
#define PCA9685_LED_FULL_ON  0x10
#define PCA9685_LED_FULL_OFF 0x10

// 舵机的PWM频率(50Hz)
#define PCA9685_SERVO_FREQ 50

// 内部振荡器频率(25MHz)
#define PCA9685_OSC_CLOCK 25000000

// 舵机控制范围
#define SERVO_180_MIN_PULSE 500   // 0.5ms
#define SERVO_180_MAX_PULSE 2500  // 2.5ms
#define SERVO_270_MIN_PULSE 500   // 0.5ms
#define SERVO_270_MAX_PULSE 2500  // 2.5ms
#define SERVO_360_MIN_PULSE 500   // 0.5ms
#define SERVO_360_MAX_PULSE 2500  // 2.5ms

// 舵机类型枚举
typedef enum {
    SERVO_180_DEGREE,  // 180度舵机
    SERVO_270_DEGREE,  // 270度舵机
    SERVO_360_DEGREE   // 360度连续旋转舵机
} ServoType;

// PCA9685句柄结构体
typedef struct {
    I2C_HandleTypeDef *hi2c;     // I2C句柄
    uint8_t address;             // 设备地址
    float pwm_frequency;         // PWM频率
} PCA9685_HandleTypeDef;

// 函数声明
HAL_StatusTypeDef PCA9685_Init(PCA9685_HandleTypeDef *hpca, I2C_HandleTypeDef *hi2c, uint8_t address, float pwm_freq);
HAL_StatusTypeDef PCA9685_Reset(PCA9685_HandleTypeDef *hpca);
HAL_StatusTypeDef PCA9685_SetPWMFreq(PCA9685_HandleTypeDef *hpca, float freq);
HAL_StatusTypeDef PCA9685_SetPWM(PCA9685_HandleTypeDef *hpca, uint8_t channel, uint16_t on, uint16_t off);
HAL_StatusTypeDef PCA9685_SetServoAngle(PCA9685_HandleTypeDef *hpca, uint8_t channel, ServoType type, float value);
HAL_StatusTypeDef PCA9685_SetServoSpeed(PCA9685_HandleTypeDef *hpca, uint8_t channel, float speed);

#endif // PCA9685_H

