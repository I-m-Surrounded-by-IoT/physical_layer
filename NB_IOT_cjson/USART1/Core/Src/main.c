/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include <stdio.h>
#include <string.h>
#include "LED.h"
#include "nbiotdriver.h"
#include "cjson.h"
#include "sensor.h"
#include "NRF24L01.h"
#include "L298N.h"
#include "ps.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

void printf_show(void);
void Motor_show(void);

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t Buf[32] = {0} ;

cJSON *interest;
cJSON *cjson_geo_point;
char *cPrint;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    uint32_t count = 0;
    HAL_ADCEx_Calibration_Start(&hadc1);
    OLED_Init();
//	NRF24L01_Init();
    printf("system is ok!\r\n");
    printf("wait 5s NB!\r\n");

//    OLED_ShowString(1, 1, "ok1!");
//    EnableUartIT();
////    LedInit();
//    NB_Init();
//    M1_Stop();
//	M2_Stop();
//        cJSON *interest = cJSON_CreateObject();
//		cJSON_AddNumberToObject(interest,"timestamp",1111111111);
//		cjson_geo_point = cJSON_CreateObject();
//		cJSON_AddNumberToObject(cjson_geo_point,"lat",222222222);
//		cJSON_AddNumberToObject(cjson_geo_point,"lon",333333333); 
//		cJSON_AddItemToObject(interest,"geo_point",cjson_geo_point); 
//		cJSON_AddNumberToObject(interest,"TDS",44444444);
//		cJSON_AddNumberToObject(interest,"TSW",555555555);
//		cJSON_AddNumberToObject(interest,"PH",666666666);
//		cJSON_AddNumberToObject(interest,"temperv",777777777); 
//		cPrint = cJSON_Print(interest);
//		printf("%s\r\n",cPrint);
//        uint8_t cPrint_strlen = strlen((char*)cPrint);
//		printf("cPrint_strlen_size%d\r\n",cPrint_strlen);

    // All functions accept NULL input, and return NULL on error.

//		SetLedRun(0);
//		SetLedRun(1);
//		SetLedRun(2);

    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();

    /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
//		printf("%s\r\n",cPrint);
//       Show_Sersor_Value();
//        if(R_IRQ() == 0)
//		{
//		  printf_show();
//		  Motor_show();
//		}
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}


void printf_show(void)
{
    Reveive(Buf);
    if(Buf[0] == Xleft)
    {
        printf("Xleft");
    }
    if(Buf[0] == Xright)
    {
        printf("Xright");
    }
    if(Buf[0] == Ybefore)
    {
        printf("Ybefore");
    }
    if(Buf[0] == Yaffer)
    {
        printf("Yaffer");
    }
    if(Buf[0] == 0)
    {
        printf("nochange");
    }

}

void Motor_show(void)
{
    Reveive(Buf);
    if(Buf[0] == Xleft)
    {
        M1_Stop();
        M2_Forward();
    }
    if(Buf[0] == Xright)
    {
        M2_Stop();
        M1_Forward();
    }
    if(Buf[0] == Ybefore)
    {
        M1_Forward();
        M2_Forward();
    }
    if(Buf[0] == Yaffer)
    {
        M1_Stop();
        M2_Stop();
    }
    if(Buf[0] == 0)
    {
        M1_Stop();
        M2_Stop();
    }
}



/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM2) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
