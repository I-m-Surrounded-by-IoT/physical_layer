#include "stm32f1xx_hal.h"
#include "adc.h"
#include "OLED.h"
#include "DS18B20_all.h"
#include "FreeRTOS.h"
#include "task.h"

float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float compensationCoefficient=1.0;
float compensationVolatge;
float kValue=1.67;
float TEMP_Value=0.0;
float averageVoltage=0;


float TSW_Value = 0.0;

float TSW_Voltage = 0;

float PH_Value = 0;

float temper = 0.0;

uint32_t ADC_Value[3] = {0};
float Voltage[4] = {0};

//Temperture
float Temperture_Value_Conversion(void)
{
    temper=DS18B20_GetTemperture();

}

void Get_ADC_Value(void)
{
    for(uint8_t i=0; i<50; i++)
    {

        for(uint8_t i=0; i<3; i++)
        {
            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, 50);
            ADC_Value[i] = ADC_Value[i] + HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);
    }
    for(uint8_t i=0; i<3; i++)
    {
        ADC_Value[i] = ADC_Value[i]/50;
    }
}


//TDS
float TDS_Value_Conversion(void)
{

    float compensationCoefficient;
    float compensationVolatge;
    float temperature=Temperture_Value_Conversion();  						//温度补偿

    averageVoltage	=(float)ADC_Value[0]/4096*3.3;

    compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    compensationVolatge = averageVoltage / compensationCoefficient;
    TDS_value = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;
    return  TDS_value ;

}


//TSW
float TSW_Value_Conversion(void)
{
    float temperature=Temperture_Value_Conversion();
    float temperature_check = 0;
    float Voltage_standard = 0;
    float K = 0 ;


    temperature_check = -0.0192*(temperature-25);
    TSW_Voltage = (float)ADC_Value[1]/4096*3.3;
    TSW_Voltage = TSW_Voltage/0.66;
    Voltage_standard = TSW_Voltage - temperature_check;
    K =865.68 *  Voltage_standard;
    TSW_Value = -865.68*TSW_Voltage + K ;

    return  TSW_Value ;
}

//PH
float PH_Value_Conversion(void)
{
    float PH_Voltage = 0;
    PH_Voltage = (float)ADC_Value[2]/4096*3.3;
    PH_Value=-5.7541*PH_Voltage+16.654;
    return PH_Value;
}


void Sensor_Value(void)
{
    Get_ADC_Value();
    TDS_Value_Conversion();
    TSW_Value_Conversion();
    PH_Value_Conversion();
	taskENTER_CRITICAL();
    Temperture_Value_Conversion();
	taskEXIT_CRITICAL();
}


void Sersor_Value_test(void)
{
   Sensor_Value();
    taskENTER_CRITICAL();
	//TDS
    OLED_ShowString(1, 1, "TDS:");
	OLED_ShowChar(1,9,'.');
    OLED_ShowNum(1, 5, TDS_value, 4);
    OLED_ShowNum(1, 10, (uint16_t)(TDS_value * 100) % 100, 2);

    //TWS 算法有问题
    OLED_ShowString(2, 1, "TSW:");
	OLED_ShowChar(2,9,'.');
    OLED_ShowNum(2, 5, TSW_Voltage, 4);
    OLED_ShowNum(2, 10, (uint16_t)(TSW_Voltage * 100) % 100, 2);

    //PH
    OLED_ShowString(3, 1, "PH:");
	OLED_ShowChar(3,6,'.');
    OLED_ShowNum(3, 4, PH_Value, 2);
    OLED_ShowNum(3, 7, (uint16_t)(PH_Value * 1000) % 1000, 3);

    //temper
    OLED_ShowString(4,1,"T:");
    if(temper<0)
    {
        OLED_ShowChar(4,3,'-');
    }
    else
    {
        OLED_ShowChar(4,3,'+');
    }
    OLED_ShowNum(4,4,temper,3);
    OLED_ShowChar(4,7,'.');
    OLED_ShowNum(4,8,(unsigned long)(temper*10000)%10000,4);
	taskEXIT_CRITICAL();
}


void Show_Sersor_Value(void)
{
	Sensor_Value();
    //TDS
    OLED_ShowString(1, 1, "TDS:");
	OLED_ShowChar(1,9,'.');
    OLED_ShowNum(1, 5, TDS_value, 4);
    OLED_ShowNum(1, 10, (uint16_t)(TDS_value * 100) % 100, 2);

    //TWS 算法有问题
    OLED_ShowString(2, 1, "TSW:");
	OLED_ShowChar(2,9,'.');
    OLED_ShowNum(2, 5, TSW_Voltage, 4);
    OLED_ShowNum(2, 10, (uint16_t)(TSW_Voltage * 100) % 100, 2);

    //PH
    OLED_ShowString(3, 1, "PH:");
	OLED_ShowChar(3,6,'.');
    OLED_ShowNum(3, 4, PH_Value, 2);
    OLED_ShowNum(3, 7, (uint16_t)(PH_Value * 1000) % 1000, 3);

    //temper
    OLED_ShowString(4,1,"T:");
    if(temper<0)
    {
        OLED_ShowChar(4,3,'-');
    }
    else
    {
        OLED_ShowChar(4,3,'+');
    }
    OLED_ShowNum(4,4,temper,3);
    OLED_ShowChar(4,7,'.');
    OLED_ShowNum(4,8,(unsigned long)(temper*10000)%10000,4);

}

void Show_Sersor_ADC_Value(void)
{
    Sensor_Value();
    OLED_ShowString(1, 1, "ADValu1:");
    OLED_ShowString(2, 1, "ADValu2:");
    OLED_ShowString(3, 1, "ADValu3:");


    OLED_ShowNum(1, 9, ADC_Value[0], 4);
    OLED_ShowNum(2, 9, ADC_Value[1], 4);
    OLED_ShowNum(3, 9, ADC_Value[2], 4);

}

void Show_Sersor_Voltage(void)
{
    Sensor_Value();
    OLED_ShowString(1, 1, "Volatge:0.00V");
    Voltage[2] = (float)ADC_Value[2] / 4095 * 3.3;
    OLED_ShowNum(1, 9, Voltage[2], 1);
    OLED_ShowNum(1, 11, (uint16_t)(Voltage[2] * 100) % 100, 2);

//		OLED_ShowString(1, 1, "Volatge:0.00V");
//		OLED_ShowString(2, 1, "Volatge:0.00V");
//		OLED_ShowString(3, 1, "Volatge:0.00V");
//		OLED_ShowString(4, 1, "Volatge:0.00V");
//
//		Voltage[0] = (float)ADC_Value[0] / 4095 * 3.3;
//	  Voltage[1] = (float)ADC_Value[1] / 4095 * 3.3;
//		Voltage[2] = (float)ADC_Value[2] / 4095 * 3.3;
//		Voltage[3] = (float)ADC_Value[3] / 4095 * 3.3;
//
//
//		OLED_ShowNum(1, 9, Voltage[0], 1);
//		OLED_ShowNum(1, 11, (uint16_t)(Voltage[0] * 100) % 100, 2);
//
//		OLED_ShowNum(2, 9, Voltage[1], 1);
//		OLED_ShowNum(2, 11, (uint16_t)(Voltage[1] * 100) % 100, 2);
//
//		OLED_ShowNum(3, 9, Voltage[2], 1);
//		OLED_ShowNum(3, 11, (uint16_t)(Voltage[2] * 100) % 100, 2);
//
//		OLED_ShowNum(4, 9, Voltage[3], 1);
//		OLED_ShowNum(4, 11, (uint16_t)(Voltage[3] * 100) % 100, 2);
}
