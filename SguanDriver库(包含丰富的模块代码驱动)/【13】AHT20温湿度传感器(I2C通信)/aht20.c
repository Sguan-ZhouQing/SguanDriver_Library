/*
 * @Author: 星必尘Sguan
 * @Date: 2025-03-30 12:28:26
 * @LastEditors: 星必尘Sguan|3464647102@qq.com
 * @LastEditTime: 2025-07-10 02:06:44
 * @FilePath: \demo_STM32F407code\Hardware\aht20.c
 * @Description: 简易的I2C读写寄存器，读取当前的温度和湿度的数值大小
 * @Key_GPIO: PB7 -> I2C1_SDA; PB6 -> I2C1_SCL;
 * 
 * Copyright (c) 2025 by $JUST, All Rights Reserved. 
 */
#include "aht20.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "esp.h"
#include "json.h"

extern float temp;	//声明外部温度变量
extern float humi;	//声明外部湿度变量

#define AHT20_ADDRESS 0x70
//这里定义从机地址，由于AHT20从机为0x38。
//因为这个AHT20 ，从机地址为7位，所以需要左移一位，变成8位。
//所以，0x38，左移一位后，变成了0x70

/**
 * @description: 温湿度传感器初始化函数
 * @return {*}
 */
void AHT20_Init()
{
	uint8_t readBuffer;//用来存放，AHT20工作状态是否正常的数据。是否为0x38
	// HAL_Delay(50);//由于模块启动，需要等待40ms
	osDelay(50);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);
	//&&hi2c1,表示需要操作的i2c的端口。
	//AHT20_ADDRESS,表示需要操作的从机地址。因为是读取数据，这个函数，会将AHT20——ADDRESS变成0x71
	//&readBuffer,读取到的数据，需要放的地方。
	//1,表示，需要读取1位数据
	// HAL_MAX_DELAY,一直等待下去，直到接收完成。
	if((readBuffer & 0x08)==0x00)	//判断，AHT20，bit[3]是否为1。不为1，就进入程序进行初始化。
	{
		uint8_t sendBuffer[3]={0xBE,0x08,0x00};//需要定义这个数组，存放，初始化的数据。
		HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer,3, HAL_MAX_DELAY);
		//&&hi2c1,表示需要操作的i2c的端口。
		//AHT20_ADDRESS,表示需要操作的从机地址。
		//sendBuffe,需要发送的数据，读取到的数据，需要放的地方。
		//3,表示，需要发送3位数据
		// HAL_MAX_DELAY,一直等待下去，直到发送完成。
	}
}

/**
 * @description: 读取AHT20的实际函数（想要在主程序加延时反复调用）
 * @param {float} *Temperature
 * @param {float} *Humidity
 * @return {*}
 */
static void AHT20_Red(AHT20_t *Variable)
{
	uint8_t sendBuffer[3]={0xAC,0x33,0x00};
	uint8_t readBuffer[6];
	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
	//&hi2c1,需要操作I2c的设备
	// AHT20_ADDRESS.温度传感器的I2C地址
	//sendBuffer,需要发送的指令（发送后就可以获得数据）
	// 3,需要发送的数据长度
	//HAL_MAX_DELAY,一直等待发送完成
	// HAL_Delay(100);//--------AHT20 接收到信息后，需要等待100ms，让其完成测量
	osDelay(100);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);
	//&hi2c1.获取这个接口的数据。
	//AHT20_ADDRESS，需要获取的从机地址。
	//readBuffer,需要保存到的地址。
	//6,需要保存的数据长度。
	//HAL_MAX_DELAY,一直等待6个数据，保存完成。
	if((readBuffer[0] & 0x80)==0x00)
	{
		uint32_t data=0;
		data=((uint32_t)readBuffer[3]>>4)+((uint32_t)readBuffer[2]<<4)+((uint32_t)readBuffer[1]<<12);
		Variable->AHT20_Humi = data * 100.0f / (1<<20);

		data=(((uint32_t)readBuffer[3]& 0xf)<<16)+((uint32_t)readBuffer[4]<<8)+(uint32_t)readBuffer[5];
		Variable->AHT20_Temp = data * 200.0f/(1<<20)-50;
	}
}

/**
 * @description: （广域）主函数反复调用
 * @return {*}
 */
void AGT20_Upadte_Tick(void)
{
	osDelay(500);
	AHT20_t Upate_temp;
	AHT20_Red(&Upate_temp);
	temp = Upate_temp.AHT20_Temp;
	ESP_SendMQTTVar("temp",temp);
	osDelay(1200);
	AHT20_Red(&Upate_temp);
	humi = Upate_temp.AHT20_Humi;
	ESP_SendMQTTVar("humi",humi);
	osDelay(500);
}
