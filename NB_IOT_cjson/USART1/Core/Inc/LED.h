#ifndef _LED_H_
#define _LED_H_

#include "stm32f1xx_hal.h"

#define LED_NUMBER 3

//指示灯
typedef enum
{
	LED_NET = 0, 	//入网指示灯
	LED_RX,			 	//发送指示灯
	LED_TX      	//接收指示灯


}teLedNums;


//LED灯任务闪烁状态
typedef enum
{
	LED_STOP = 0,	//停止
	LED_RUN,      //运行
	LED_DELAY     //延迟
}teLedTaskStatus;


void LedOn(teLedNums LedNums);
void LedOff(teLedNums LedNums);
void LedInit(void);

void SetLedRun(teLedNums LedNums);


void LedTask(void);

#endif