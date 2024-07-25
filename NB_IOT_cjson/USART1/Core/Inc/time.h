#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f1xx_hal.h"

//软件定时工作任务数据结构
typedef struct
{
	uint32_t TimeStart; //开始(获取第一次系统节拍值)
	uint32_t TimeInter; //定时间隔(设置定时时间)

}tsTimeType;

void SetTime(tsTimeType *TimeType,uint32_t TimeInter);
uint8_t  CompareTime(tsTimeType *TimeType);

#endif