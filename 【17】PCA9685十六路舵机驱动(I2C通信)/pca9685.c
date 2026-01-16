/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-02 12:29:41
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-05-06 19:15:30
 * @FilePath: \test_2804FocMotor\Hardware\pca9685.c
 * @Description: PCA9685驱动16路舵机的函数
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "pca9685.h"
#include <math.h>

// 私有函数声明
static HAL_StatusTypeDef PCA9685_WriteRegister(PCA9685_HandleTypeDef *hpca, uint8_t reg, uint8_t value);
static HAL_StatusTypeDef PCA9685_ReadRegister(PCA9685_HandleTypeDef *hpca, uint8_t reg, uint8_t *value);

/**
  * @brief  初始化PCA9685设备
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  hi2c: 指向I2C_HandleTypeDef结构的指针
  * @param  address: PCA9685的I2C地址
  * @param  pwm_freq: 期望的PWM频率(Hz)
  * @retval HAL状态
  */
HAL_StatusTypeDef PCA9685_Init(PCA9685_HandleTypeDef *hpca, I2C_HandleTypeDef *hi2c, uint8_t address, float pwm_freq) {
    HAL_StatusTypeDef status;
    
    hpca->hi2c = hi2c;
    hpca->address = address << 1; // 左移7位地址
    hpca->pwm_frequency = pwm_freq;
    
    // 复位设备
    status = PCA9685_Reset(hpca);
    if (status != HAL_OK) return status;
    
    // 设置PWM频率
    status = PCA9685_SetPWMFreq(hpca, pwm_freq);
    if (status != HAL_OK) return status;
    
    // 配置MODE2寄存器
    // OUTDRV = 1 (推挽结构)
    // INVRT = 0 (输出逻辑状态不反转)
    // OUTNE = 01 (当OE=1时，如果OUTDRV=1则输出高电平)
    status = PCA9685_WriteRegister(hpca, PCA9685_MODE2_REG, PCA9685_OUTDRV | PCA9685_OUTNE0);
    
    return status;
}

/**
  * @brief  复位PCA9685设备
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @retval HAL状态
  */
HAL_StatusTypeDef PCA9685_Reset(PCA9685_HandleTypeDef *hpca) {
    // 使设备进入睡眠模式
    HAL_StatusTypeDef status = PCA9685_WriteRegister(hpca, PCA9685_MODE1_REG, PCA9685_SLEEP);
    if (status != HAL_OK) return status;
    
    // 等待振荡器停止(500us)
    HAL_Delay(1);
    
    // 清除睡眠位并启用自动增量
    return PCA9685_WriteRegister(hpca, PCA9685_MODE1_REG, PCA9685_AI);
}

/**
  * @brief  设置PWM频率
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  freq: 期望的频率(Hz)
  * @retval HAL状态
  */
HAL_StatusTypeDef PCA9685_SetPWMFreq(PCA9685_HandleTypeDef *hpca, float freq) {
    // 计算预分频值
    float prescaleval = (PCA9685_OSC_CLOCK / (freq * 4096.0f)) - 1;
    uint8_t prescale = (uint8_t)(prescaleval + 0.5f);
    
    // 保存频率
    hpca->pwm_frequency = freq;
    
    // 使设备进入睡眠模式
    HAL_StatusTypeDef status = PCA9685_WriteRegister(hpca, PCA9685_MODE1_REG, PCA9685_SLEEP);
    if (status != HAL_OK) return status;
    
    // 设置预分频
    status = PCA9685_WriteRegister(hpca, PCA9685_PRESCALE_REG, prescale);
    if (status != HAL_OK) return status;
    
    // 唤醒设备
    status = PCA9685_WriteRegister(hpca, PCA9685_MODE1_REG, PCA9685_AI);
    if (status != HAL_OK) return status;
    
    // 等待振荡器稳定
    HAL_Delay(1);
    
    // 启用重启
    uint8_t mode1;
    status = PCA9685_ReadRegister(hpca, PCA9685_MODE1_REG, &mode1);
    if (status != HAL_OK) return status;
    
    mode1 |= PCA9685_RESTART;
    return PCA9685_WriteRegister(hpca, PCA9685_MODE1_REG, mode1);
}

/**
  * @brief  设置特定通道的PWM输出
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  channel: 通道号(0-15)
  * @param  on: ON计数器值(0-4095)
  * @param  off: OFF计数器值(0-4095)
  * @retval HAL状态
  */
HAL_StatusTypeDef PCA9685_SetPWM(PCA9685_HandleTypeDef *hpca, uint8_t channel, uint16_t on, uint16_t off) {
    uint8_t data[4];
    
    // 计算寄存器地址
    uint8_t reg = PCA9685_LED0_ON_L_REG + (4 * channel);
    
    // 准备数据
    data[0] = on & 0xFF;          // LEDn_ON_L
    data[1] = (on >> 8) & 0x0F;   // LEDn_ON_H
    data[2] = off & 0xFF;         // LEDn_OFF_L
    data[3] = (off >> 8) & 0x0F;  // LEDn_OFF_H
    
    // 写入寄存器数据
    return HAL_I2C_Mem_Write(hpca->hi2c, hpca->address, reg, I2C_MEMADD_SIZE_8BIT, data, 4, HAL_MAX_DELAY);
}

/**
  * @brief  设置舵机角度
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  channel: 通道号(0-15)
  * @param  type: 舵机类型(180, 270或360度)
  * @param  value: 角度值(180/270度)或速度(360度)
  * @retval HAL状态
  */
HAL_StatusTypeDef PCA9685_SetServoAngle(PCA9685_HandleTypeDef *hpca, uint8_t channel, ServoType type, float value) {
    // 将值限制在有效范围内
    if (type == SERVO_180_DEGREE) {
        value = fmaxf(0.0f, fminf(180.0f, value));
    } else if (type == SERVO_270_DEGREE) {
        value = fmaxf(0.0f, fminf(270.0f, value));
    } else {
        // 对于360度舵机，请使用SetServoSpeed函数
        return HAL_ERROR;
    }
    
    // 根据舵机类型计算脉冲宽度
    float pulse_width;
    if (type == SERVO_180_DEGREE) {
        pulse_width = SERVO_180_MIN_PULSE + (value / 180.0f) * (SERVO_180_MAX_PULSE - SERVO_180_MIN_PULSE);
    } else { // SERVO_270_DEGREE
        pulse_width = SERVO_270_MIN_PULSE + (value / 270.0f) * (SERVO_270_MAX_PULSE - SERVO_270_MIN_PULSE);
    }
    
    // 将脉冲宽度转换为tick计数(每个tick是周期的1/4096)
    float period = 1000000.0f / hpca->pwm_frequency; // 周期(微秒)
    uint16_t ticks = (uint16_t)((pulse_width / period) * 4096.0f);
    
    // 设置PWM(总是从0开始，在计算的tick结束)
    return PCA9685_SetPWM(hpca, channel, 0, ticks);
}

/**
  * @brief  设置连续旋转舵机的速度
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  channel: 通道号(0-15)
  * @param  speed: 速度(-1.0到1.0，0表示停止)
  * @retval HAL状态
  */
HAL_StatusTypeDef PCA9685_SetServoSpeed(PCA9685_HandleTypeDef *hpca, uint8_t channel, float speed) {
    // 将速度限制在有效范围内
    speed = fmaxf(-1.0f, fminf(1.0f, speed));
    
    // 计算脉冲宽度
    float neutral = (SERVO_360_MIN_PULSE + SERVO_360_MAX_PULSE) / 2.0f;
    float pulse_width;
    
    if (speed == 0) {
        pulse_width = neutral; // 停止
    } else if (speed > 0) {
        // 正向(顺时针)
        pulse_width = neutral + speed * (SERVO_360_MAX_PULSE - neutral);
    } else {
        // 反向(逆时针)
        pulse_width = neutral + speed * (neutral - SERVO_360_MIN_PULSE);
    }
    
    // 将脉冲宽度转换为tick计数
    float period = 1000000.0f / hpca->pwm_frequency; // 周期(微秒)
    uint16_t ticks = (uint16_t)((pulse_width / period) * 4096.0f);
    
    // 设置PWM(总是从0开始，在计算的tick结束)
    return PCA9685_SetPWM(hpca, channel, 0, ticks);
}

/**
  * @brief  向PCA9685寄存器写入值
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  reg: 寄存器地址
  * @param  value: 要写入的值
  * @retval HAL状态
  */
static HAL_StatusTypeDef PCA9685_WriteRegister(PCA9685_HandleTypeDef *hpca, uint8_t reg, uint8_t value) {
    return HAL_I2C_Mem_Write(hpca->hi2c, hpca->address, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
}

/**
  * @brief  从PCA9685寄存器读取值
  * @param  hpca: 指向PCA9685_HandleTypeDef结构的指针
  * @param  reg: 寄存器地址
  * @param  value: 存储读取值的指针
  * @retval HAL状态
  */
static HAL_StatusTypeDef PCA9685_ReadRegister(PCA9685_HandleTypeDef *hpca, uint8_t reg, uint8_t *value) {
    return HAL_I2C_Mem_Read(hpca->hi2c, hpca->address, reg, I2C_MEMADD_SIZE_8BIT, value, 1, HAL_MAX_DELAY);
}

