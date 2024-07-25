/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
#include "queue.h"
#include "OLED.h"
#include "stdio.h"
#include "sensor.h"
#include "nbiotdriver.h"
#include "usart.h"
#include "cjson.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN Variables */
/*任务配,堆栈内存大小,任务优先级*/

/*START_TASK任务,创建其他两个任务*/

void START_TASK( void * pvParameters );
#define START_TASK_PRIO         osPriorityNormal
#define START_TASK_STACK_SIZE   128
TaskHandle_t  START_TASK_handler;

void NB_IOT_task( void * pvParameters );
#define NB_IOT_task_PRIO         osPriorityNormal + 1
#define NB_IOT_task_STACK_SIZE   128
TaskHandle_t  NB_IOT_task_handler;

void Send_Data_task( void * pvParameters );
#define Send_Data_task_PRIO         osPriorityNormal + 1
#define Send_Data_task_STACK_SIZE   128
TaskHandle_t  Send_Data_task_handler;

void Sensor_task(void * pvParameters );
#define Sensor_task_PRIO         osPriorityNormal
#define Sensor_task_STACK_SIZE   128
TaskHandle_t  Sensor_task_handler;



/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */

    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    xTaskCreate((TaskFunction_t)START_TASK,"START_TASK",START_TASK_STACK_SIZE,NULL,START_TASK_PRIO,&START_TASK_handler);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
//	OLED_Clear();
    uint32_t StartDefault_num = 0;
//    OLED_ShowString(1,1,"hello world");
//	 UBaseType_t task_stack_min = 0;
    uint32_t count = 0;
//	BaseType_t err = 0;
//	cJSON *interest;
//	cJSON *cjson_geo_point;
//	char *cPrint;
//	char NbSendDate[256];
//	
    /* Infinite loop */
    for(;;)
    {
		 printf("StartDefault运行次数:%d！！！!\r\n",++StartDefault_num);
//         err = xSemaphoreTake(semphore_handle,portMAX_DELAY);

//         if(err == pdPASS)
//		 {
////			vTaskSuspendAll();
//			taskENTER_CRITICAL();
//			cJSON *interest = cJSON_CreateObject();
//			cJSON_AddNumberToObject(interest,"timestamp",1111111111);
//			cjson_geo_point = cJSON_CreateObject();
//			cJSON_AddNumberToObject(cjson_geo_point,"lat",222222222);
//			cJSON_AddNumberToObject(cjson_geo_point,"lon",333333333); 
//			cJSON_AddItemToObject(interest,"geo_point",cjson_geo_point); 
//	        cJSON_AddNumberToObject(interest,"TDS",44444444);
//            cJSON_AddNumberToObject(interest,"TSW",555555555);
//            cJSON_AddNumberToObject(interest,"PH",666666666);
//            cJSON_AddNumberToObject(interest,"temperv",777777777); 
//			cPrint = cJSON_Print(interest);
//			printf("%s\r\n",cPrint);
//			memset(NbSendDate,0,100);
//			uint8_t cPrint_strlen = strlen((char*)cPrint);
//	        sprintf(NbSendDate,"AT+QMTPUB=0,1,2,0,\"device/2/report\", %d\r\n",cPrint_strlen);
//            HAL_UART_Transmit(&huart2,(uint8_t*)NbSendDate,strlen(NbSendDate),1000);

//            HAL_UART_Transmit(&huart2,(uint8_t*)cPrint,cPrint_strlen,1000);

//            HAL_UART_Transmit(&huart1,(uint8_t*)cPrint,cPrint_strlen,1000);
//			taskEXIT_CRITICAL();
////			xTaskResumeAll();   
//			xSemaphoreGive(semphore_handle);
//			count++;
//			if(count==3)
//			{
//			  Send_Data_Flag = 0;
//			}
         vTaskDelay(1000);
		 }


//		OLED_ShowNum(2,1,count++,3);
     
    }
    /* USER CODE END StartDefaultTask */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void START_TASK( void * pvParameters )
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)NB_IOT_task,"NB_IOT_task",NB_IOT_task_STACK_SIZE,NULL,NB_IOT_task_PRIO,&NB_IOT_task_handler);
    xTaskCreate((TaskFunction_t)Sensor_task,"Sensor_task",Sensor_task_STACK_SIZE,NULL,Sensor_task_PRIO,&Sensor_task_handler);
	xTaskCreate((TaskFunction_t)Send_Data_task,"Send_Data_task",Send_Data_task_STACK_SIZE,NULL,Send_Data_task_PRIO,&Send_Data_task_handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void NB_IOT_task( void * pvParameters )
{
//    EnableUartIT();
//    NB_Init();
    while(1)
    {   
//       taskENTER_CRITICAL();
////		if(Send_Data_Flag != 1)
////		{
//		 NB_Task();
////		} 
////		else
////		{
////		 vTaskDelay(1000);
////		}
//        
//		taskEXIT_CRITICAL();
        vTaskDelay(100);
    }

}

void Sensor_task (void * pvParameters )
{

    while(1)
    {
		taskENTER_CRITICAL();
		Sersor_Value_test();
		taskEXIT_CRITICAL();
		vTaskDelay(10);
    }
}

void Send_Data_task(void * pvParameters )
{

    while(1)
    {
	  printf("nb_ok!");
      vTaskDelay(1000);
    }
}


/* USER CODE END Application */

