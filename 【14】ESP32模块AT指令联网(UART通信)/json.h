#ifndef __JSON_H
#define __JSON_H

#include "printf.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "cJSON.h"

// 全局变量声明
extern float temp;
extern float humi;
extern float strees;
extern float distance;
extern bool power_flag;
extern bool motor_flag;
extern uint32_t timer;
extern uint8_t command;
extern uint8_t device_ID;
//药仓占用情况全局变量
extern uint8_t yaocang_2;
extern uint8_t yaocang_4;
extern uint8_t yaocang_7;
extern uint8_t yaocang_9;

// 函数声明
void parse_mqtt_message(const char *message);
void parse_json_data(const char *message) ;
void Json_ProcessData_Tick(void);



#endif // JSON_H
