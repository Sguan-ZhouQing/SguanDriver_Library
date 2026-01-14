/*
 * @Author: 星必尘Sguan
 * @Date: 2025-05-26 15:32:04
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-07-09 23:26:41
 * @FilePath: \demo_STM32F407code\Hardware\json.c
 * @Description: 实现json数据的接收和解析
 * @Key_GPIO: NULL;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "json.h"
#include "gpio.h"
#include "esp.h"
#include "RTC_Timer.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart2;
extern uint8_t esp_readBuffer[esp_BUFFER_SIZE];
extern volatile bool rx_complete;
#define MAX_JSON_LEN 512  // 根据实际JSON最大长度调整

// 全局变量定义
float temp = 0.0f;                 // AHT20传感器上传的温度（展示用）
float humi = 0.0f;                 // AGT20上传的湿度数据（展示用）
float strees = 0.0f;               // 压力传感器数值（展示用）
float my_distance = 0.0f;             // 距离传感器，测人与药仓的距离（展示用）
bool power_flag = true;            // 电控板的电源开关（控制）
bool motor_flag = true;            // 电机强制归零开关（控制）
uint32_t timer = 1751629025;       // 单片机获取的时间戳
uint8_t command = 0;               // 存取药的命令0-24（控制）
uint8_t device_ID = 0;             // 用户身份核验
//药仓占用情况全局变量
uint8_t yaocang_2 = 0;
uint8_t yaocang_4 = 0;
uint8_t yaocang_7 = 0;
uint8_t yaocang_9 = 0;


/**
 * @description: 解析MQTT消息，提取JSON部分
 * @param {char} *message MQTT原始消息
 * @return {void}
 */
void parse_mqtt_message(const char *message) {
    // 查找JSON数据的起始位置
    const char *json_start = strchr(message, '{');
    if (json_start == NULL) {
        return; // 没有找到JSON数据
    }
    // 调用JSON解析函数
    parse_json_data(json_start);
}


/**
 * @brief 解析JSON数据并更新全局变量
 * @param message 原始MQTT消息字符串
 */
void parse_json_data(const char *message) {
    // 跳过前缀找到JSON起始位置
    const char *json_start = strchr(message, '{');
    if (!json_start) {
        return;
    }

    // 使用静态缓冲区
    static char json_buffer[MAX_JSON_LEN + 1];
    size_t json_len = strlen(json_start);
    
    // 检查长度是否超过缓冲区
    if (json_len > MAX_JSON_LEN) {
        json_len = MAX_JSON_LEN;
    }
    
    // 复制JSON数据
    strncpy(json_buffer, json_start, json_len);
    json_buffer[json_len] = '\0';

    // 确保JSON完整闭合
    char *json_end = strrchr(json_buffer, '}');
    if (!json_end) {
        return;
    }
    *(json_end + 1) = '\0'; // 强制终止

    // 解析JSON
    cJSON *root = cJSON_Parse(json_buffer);
    if (!root) {
        return;
    }

    // 提取params对象
    cJSON *params = cJSON_GetObjectItemCaseSensitive(root, "params");
    if (!params) {
        cJSON_Delete(root);
        return;
    }

    // 解析command
    cJSON *cmd_item = cJSON_GetObjectItemCaseSensitive(params, "command");
    if (cJSON_IsNumber(cmd_item)) {
        command = (uint8_t)cmd_item->valueint;
    }

    // 解析yaocang_x字段（同时只会出现其中一个）
    cJSON *yaocang_item = NULL;
    
    yaocang_item = cJSON_GetObjectItemCaseSensitive(params, "yaocang_2");
    if (cJSON_IsNumber(yaocang_item)) {
        yaocang_2 = (uint8_t)yaocang_item->valueint;
        ESP_SendMQTTInt("yaocang_2", yaocang_2);
    }
    
    yaocang_item = cJSON_GetObjectItemCaseSensitive(params, "yaocang_4");
    if (cJSON_IsNumber(yaocang_item)) {
        yaocang_4 = (uint8_t)yaocang_item->valueint;
        ESP_SendMQTTInt("yaocang_4", yaocang_4);
    }
    
    yaocang_item = cJSON_GetObjectItemCaseSensitive(params, "yaocang_7");
    if (cJSON_IsNumber(yaocang_item)) {
        yaocang_7 = (uint8_t)yaocang_item->valueint;
        ESP_SendMQTTInt("yaocang_7", yaocang_7);
    }
    
    yaocang_item = cJSON_GetObjectItemCaseSensitive(params, "yaocang_9");
    if (cJSON_IsNumber(yaocang_item)) {
        yaocang_9 = (uint8_t)yaocang_item->valueint;
        ESP_SendMQTTInt("yaocang_9", yaocang_9);
    }

    // 解析其他字段
    cJSON *item = NULL;
    
    // 时间戳（13位转10位）
    item = cJSON_GetObjectItemCaseSensitive(params, "timer");
    if (cJSON_IsNumber(item)) {
        char timer_str[14];
        snprintf(timer_str, sizeof(timer_str), "%.0f", item->valuedouble);
        if (strlen(timer_str) > 10) timer_str[10] = '\0';
        timer = (uint32_t)strtoul(timer_str, NULL, 10);
        //返回参数到云平台
        ESP_SendMQTTInt("timer", timer);
        //及时更新现在的时间
        RTC_TimerUpdate();
    }

    // 布尔值
    item = cJSON_GetObjectItemCaseSensitive(params, "power_flag");
    if (cJSON_IsBool(item)) {
        power_flag = cJSON_IsTrue(item);
        ESP_SendMQTTBool("power_flag", power_flag);
    }

    item = cJSON_GetObjectItemCaseSensitive(params, "motor_flag");
    if (cJSON_IsBool(item)) {
        motor_flag = cJSON_IsTrue(item);
        ESP_SendMQTTBool("motor_flag", motor_flag);
    }

    // 设备ID（暂时用不到）
    item = cJSON_GetObjectItemCaseSensitive(params, "device_ID");
    if (cJSON_IsNumber(item)) device_ID = (uint8_t)item->valueint;

    // 释放资源
    cJSON_Delete(root);
}


// 在主循环中处理
void Json_ProcessData_Tick(void) {
    if (rx_complete == true) {
        // 查找JSON起始位置（跳过"+MQTTSUBRECV..."前缀）
        char *json_start = strchr((char*)esp_readBuffer, '{');
        // OLED_ShowAsciiBuffer(0, 0, esp_readBuffer, esp_BUFFER_SIZE, OLED_6X8);
        if (json_start) {
            parse_json_data(json_start);
        }
        // 准备下一次接收
        rx_complete = false;
        memset(esp_readBuffer, 0, esp_BUFFER_SIZE);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp_readBuffer, sizeof(esp_readBuffer));
        __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
    }
}
