/*
 * @Author: 星必尘Sguan
 * @Date: 2025-04-17 15:44:34
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-07-19 09:03:43
 * @FilePath: \demo_STM32F407code\Hardware\esp.c
 * @Description: 实现esp8266_01s连接OneNET物联网模型;
 * @Key_GPIO: PA2 -> USART2_TX; PA3 -> UASRT2_RX;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "esp.h"
#include "cmsis_os.h"
#include "Buzzer.h"

extern UART_HandleTypeDef huart2;
extern uint8_t esp_readBuffer[esp_BUFFER_SIZE];
extern uint16_t esp_rx_index;


/**
 * @description: AT指令定义（已添加\r\n）
 * @return {*}
 */
static const char *esp_ATmain = "AT\r\n";
static const char *espSet_StationMode = "AT+CWMODE=1\r\n";
static const char *espStart_DHCP = "AT+CWDHCP=1,1\r\n";
// static const char *espConnet_Wifi = "AT+CWJAP=\"星必尘Sguan\",\"sguan12345\"\r\n";
static const char *espSet_Token = "AT+MQTTUSERCFG=0,1,\"d1\",\"REc53ZzF42\",\"version=2018-10-31&res=products%2FREc53ZzF42%2Fdevices%2Fd1&et=1890079256&method=md5&sign=T%2FpNtLjJbXU6BjSW1apFdQ%3D%3D\",0,0,\"\"\r\n";
static const char *espSet_SeverStation = "AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n";
static const char *espReply_Topic = "AT+MQTTSUB=0,\"$sys/REc53ZzF42/d1/thing/property/post/reply\",1\r\n";
static const char *espSet_Topic = "AT+MQTTSUB=0,\"$sys/REc53ZzF42/d1/thing/property/set\",0\r\n";
// static char mqtt_pub_cmd[] = "AT+MQTTPUB=0,\"$sys/REc53ZzF42/d1/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"temp\\\":{\\\"value\\\":45}}}\",0,0";


/**
 * @description: 函数封装ESP8266串口发送信息的功能，简化工作流程;
 * @param {uint8_t} *Str
 * @return {*}
 */
static void ESP_SendString(const char *Str)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)Str, strlen(Str), HAL_MAX_DELAY);
    // HAL_Delay(100);   //适当延时，确保模块的初始化工作正常进行
}

/**
 * @brief 初始化ESP8266并连接MQTT
 * @return Esp_OK: 成功, Esp_ERROR: 失败
 */
Esp_StatusTypeDef ESP_Init(void) {
    // 依次发送AT指令并检查响应
    // HAL_Delay(800);
    osDelay(600);
    ESP_SendString(esp_ATmain);
    // HAL_Delay(800);
    osDelay(1200);
    ESP_SendString(espSet_StationMode);
    // HAL_Delay(800);
    osDelay(1200);
    ESP_SendString(espStart_DHCP);
    // HAL_Delay(800);
    osDelay(1200);
    // ESP_SendString(espConnet_Wifi);
    // HAL_Delay(6000);
    ESP_SendString(espSet_Token);
    // HAL_Delay(2000);
    osDelay(2400);
    ESP_SendString(espSet_SeverStation);
    // HAL_Delay(8000);
    osDelay(8000);
    ESP_SendString(espReply_Topic);
    // HAL_Delay(1200);
    osDelay(1600);
    ESP_SendString(espSet_Topic);
    //启用串口2的DMA传输，用于接收消息更新
    HAL_StatusTypeDef temp = HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp_readBuffer, sizeof(esp_readBuffer));
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
    if (temp == HAL_OK){
        Buzzer_Control();
        return Esp_OK;
    }
    return Esp_ERROR; // 检查所有指令是否均返回OK
}

/**
 * @description: 发送MQTT发布消息，动态替换变量和值
 * @param {const char*} var_name 变量名（如"temp"、"humi"等）
 * @param {float} value 变量的值
 * @return {void}
 */
void ESP_SendMQTTVar(const char* var_name, float value) {
    // 创建足够大的缓冲区
    char buffer[256];
    // 格式化MQTT发布命令
    snprintf(buffer, sizeof(buffer), 
             "AT+MQTTPUB=0,\"$sys/REc53ZzF42/d1/thing/property/post\","
             "\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%.2f}}}\",0,0\r\n",
             var_name, value);
    // 发送命令
    ESP_SendString(buffer);
}

/**
 * @description: 发送MQTT发布消息，针对布尔型变量
 * @param {const char*} var_name 变量名（如"power_flag"、"motor_flag"）
 * @param {bool} value 布尔值
 * @return {void}
 */
void ESP_SendMQTTBool(const char* var_name, bool value) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), 
             "AT+MQTTPUB=0,\"$sys/REc53ZzF42/d1/thing/property/post\","
             "\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%s}}}\",0,0\r\n",
             var_name, value ? "true" : "false");
    ESP_SendString(buffer);
}

/**
 * @description: 发送MQTT发布消息，针对整型变量
 * @param {const char*} var_name 变量名（如"command"、"device_ID"）
 * @param {int} value 整数值
 * @return {void}
 */
void ESP_SendMQTTInt(const char* var_name, int value) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), 
             "AT+MQTTPUB=0,\"$sys/REc53ZzF42/d1/thing/property/post\","
             "\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%d}}}\",0,0\r\n",
             var_name, value);
    ESP_SendString(buffer);
}

/**
 * @description: 发送数组数据到 OneNET（适配官方要求的 JSON 格式）
 * @param {const char*} var_name 变量名（如 "warehouse_left"）
 * @param {uint8_t*} array 数组指针
 * @param {int} size 数组大小
 * @return {void}
 */
void ESP_SendMQTTArray(const char* var_name, uint8_t* array, int size) {
    char buffer[512];
    char array_str[128] = "[";

    // 1. 构建数组字符串（如 "[1,0,1,0,1,0]"）
    for (int i = 0; i < size; i++) {
        char temp[8];
        snprintf(temp, sizeof(temp), "%d", array[i]);
        strcat(array_str, temp);
        if (i < size - 1) strcat(array_str, ",");
    }
    strcat(array_str, "]");

    // 2. 生成完整的 JSON（包含 "value" 和 "version"）
    snprintf(buffer, sizeof(buffer), 
             "AT+MQTTPUB=0,\"$sys/REc53ZzF42/d1/thing/property/post\","
             "\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%s}}}\",0,0\r\n",
             var_name, array_str);  // 也可使用时间戳作为动态 id

    // // 3. 调试输出（可选）
    // printf("[OneNET] Sending: %s\n", buffer);
    // 4. 发送数据
    ESP_SendString(buffer);
}



