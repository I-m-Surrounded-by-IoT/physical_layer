#include "main.h"
#include "oled.h"

 uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0)
	{
		HAL_Delay(20);
//		while (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 0);
		KeyNum = 1;
		HAL_Delay(20);
		
	}
	if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 0)
	{
		HAL_Delay(20);
		KeyNum = 2;
//		while (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 0);
		HAL_Delay(20);
		
	}

	if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 0)
	{
		HAL_Delay(20); 
		KeyNum = 3;
//		while (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 0);
		HAL_Delay(20);
		
	}
	if (HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == 0)
	{
		HAL_Delay(20); 
		KeyNum = 4;
//		while (HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin) == 0);
		HAL_Delay(20);
		
	}	
	return KeyNum;
}

void show_key(void)
{
	uint8_t Key_value = Key_GetNum();
	
	if(Key_value == 3)
	{
		OLED_ShowString(1,4,"Xright") ;
	}

	if(Key_value == 4)
	{
		OLED_ShowString(1,4,"Xleft") ;
	}
	
	if(Key_value == 1)
	{
		OLED_ShowString(1,4,"Ybefore") ;
	}
	
	if(Key_value == 2)
	{
		OLED_ShowString(1,4,"Yaffer") ;
	}
	if(Key_value == 0)
	{
		OLED_ShowString(1,4,"nochang") ;
	}
}









