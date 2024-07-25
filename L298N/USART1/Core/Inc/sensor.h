#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "stm32f1xx_hal.h"

extern uint32_t Voltage[3];
extern uint32_t ADC_Value[4] ;

extern float TDS_value;
extern float PH_Value;
extern float TSW_Value ;
extern float temper ;

void Get_ADC_Value(void);
void Sensor_Value(void);
void Show_Sersor_Value(void);
void Show_Sersor_ADC_Value(void);
void Show_Sersor_Voltage(void);

void Sersor_Value_test(void);

#endif