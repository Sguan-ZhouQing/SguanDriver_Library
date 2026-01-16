#ifndef __ESP_H
#define __ESP_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>  // 确保包含此头文件以使用 uint32_t
#include <string.h>
#include "printf.h"

typedef enum
{
  Esp_OK       = 0x00U,
  Esp_ERROR    = 0x01U,
} Esp_StatusTypeDef;


static void ESP_SendString(const char *Str);
Esp_StatusTypeDef ESP_Init(void);
void ESP_SendMQTTVar(const char* var_name, float value);
void ESP_SendMQTTBool(const char* var_name, bool value);
void ESP_SendMQTTInt(const char* var_name, int value);
void ESP_SendMQTTArray(const char* var_name, uint8_t* array, int size);


#endif // ESP_H
