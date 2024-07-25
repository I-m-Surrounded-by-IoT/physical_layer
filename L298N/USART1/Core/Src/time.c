#include "time.h"



//设置定时时间，获取第一次系统节拍值
void SetTime(tsTimeType *TimeType,uint32_t TimeInter)
{

    TimeType->TimeStart = HAL_GetTick();
    TimeType->TimeInter = TimeInter;
}

uint8_t  CompareTime(tsTimeType *TimeType)
{
    //系统节拍值减第一次系统节拍值，是否大于设置定时时间，超过返回1
    return ((HAL_GetTick()-TimeType->TimeStart) >= TimeType->TimeInter);


}