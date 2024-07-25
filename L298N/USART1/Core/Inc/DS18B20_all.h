#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "stm32f1xx_hal.h"

#include "delay.h"

/*  DS18B20时钟端口、引脚定义 */
#define DS18B20_PORT 			GPIOA  
#define DS18B20_PIN 			(GPIO_PIN_11)

   	
uint8_t DS18B20_Init(void);			//初始化DS18B20
float DS18B20_GetTemperture(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);		//读出一个字节
uint8_t DS18B20_Read_Bit(void);		//读出一个位
uint8_t DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Reset(void);			//复位DS18B20   

#endif