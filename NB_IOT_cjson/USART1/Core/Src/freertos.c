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
#include "NRF24L01.h"
#include "L298N.h"
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
void Motor_show(void);
void printf_show(void);
void Motor_Iot_show(void);
void printf_Iot_show(void);
/*任务配,堆栈内存大小,任务优先级*/

/*START_TASK任务,创建其他两个任务*/

void START_TASK( void * pvParameters );
#define START_TASK_PRIO         osPriorityNormal
#define START_TASK_STACK_SIZE   128
TaskHandle_t  START_TASK_handler;

void NB_IOT_task( void * pvParameters );
#define NB_IOT_task_PRIO         osPriorityNormal 
#define NB_IOT_task_STACK_SIZE   1000
TaskHandle_t  NB_IOT_task_handler;

void Motor_task( void * pvParameters );
#define Motor_task_PRIO         osPriorityNormal + 2
#define Motor_task_STACK_SIZE   128
TaskHandle_t  Motor_task_handler;

void Sensor_task(void * pvParameters );
#define Sensor_task_PRIO         osPriorityNormal
#define Sensor_task_STACK_SIZE   128
TaskHandle_t  Sensor_task_handler;

void Resume_task2(void * pvParameters );
#define Resume_task2_PRIO         osPriorityNormal + 2
#define Resume_task2_STACK_SIZE   128
TaskHandle_t  Resume_task2_handler;

#define Xleft    	     1
#define Xright           2
#define Ybefore    	   	 3
#define Yaffer    		 4

uint8_t Buf[32] = {0} ;
extern uint8_t taskSuspend;
QueueHandle_t semphore_handle;
QueueHandle_t semphore_Flag_handle;
extern uint8_t send_data_flag;
extern uint8_t EXIT_Flag;
extern uint8_t Motor_Data;


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
	semphore_handle = xSemaphoreCreateBinary();
	 if(semphore_handle != NULL)
	 {
		printf("二值信号量创建成功！！！！\r\n");
	 }
	 semphore_Flag_handle = xSemaphoreCreateBinary();
	 if(semphore_Flag_handle != NULL)
	 {
		printf("二值信号量创建成功！！！！\r\n");
	 }
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  xTaskCreate((TaskFunction_t)START_TASK,"START_TASK",START_TASK_STACK_SIZE,NULL,START_TASK_PRIO,&START_TASK_handler);
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
    uint32_t count = 0;

//	
    /* Infinite loop */
    for(;;)
    {
		 printf("StartDefault运行次数:%d！！！!\r\n",++StartDefault_num);
         vTaskDelay(1000);
	}


//		OLED_ShowNum(2,1,count++,3);
     
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void START_TASK( void * pvParameters )
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)NB_IOT_task,"NB_IOT_task",NB_IOT_task_STACK_SIZE,NULL,NB_IOT_task_PRIO,&NB_IOT_task_handler);
    xTaskCreate((TaskFunction_t)Sensor_task,"Sensor_task",Sensor_task_STACK_SIZE,NULL,Sensor_task_PRIO,&Sensor_task_handler);
	xTaskCreate((TaskFunction_t)Resume_task2,"Resume_task2",Resume_task2_STACK_SIZE,NULL,Resume_task2_PRIO,&Resume_task2_handler);
	xTaskCreate((TaskFunction_t)Motor_task,"Motor_task",Motor_task_STACK_SIZE,NULL,Motor_task_PRIO,&Motor_task_handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void NB_IOT_task( void * pvParameters )
{
    EnableUartIT();
    NB_Init();
	BaseType_t err = 0;
    while(1)
    {   
	  NB_Task();
	  if(taskSuspend == 1)
	  {
		  if(semphore_Flag_handle != NULL)
		  {
				err = xSemaphoreGive(semphore_Flag_handle);
				if(err == pdPASS)
				{
					printf("信号量释放成功！！！\r\n");
				}
				else
				{
					printf("信号量释放失败！！！\r\n");
				}
		  }
		  
	  }
      vTaskDelay(100);
    }

}

void Sensor_task (void * pvParameters )
{
    while(1)
    {
		Sersor_Value_test();
		vTaskDelay(4000);
    }
}

void Resume_task2(void * pvParameters )
{
	
   while(1)
   {
	 //一定时间后进行数据上报
	  if(xSemaphoreTake(semphore_Flag_handle, portMAX_DELAY) == pdTRUE)
	  {
		vTaskSuspend(NB_IOT_task_handler);
		printf("任务被挂起！！！\r\n");
		vTaskDelay(30000);
		vTaskResume(NB_IOT_task_handler); 
		NB_Send_Data(); 
		taskSuspend = 0;
		printf("恢复task1！！！!\r\n");
	  }
	 
	 vTaskDelay(100);
   }
}

void Motor_task( void * pvParameters )
{

	while(1)
	{   
//        if (xSemaphoreTake(semphore_handle, portMAX_DELAY) == pdTRUE)
//		{
//			printf_Iot_show();
//			Motor_Iot_show();
//			Motor_Data = 0;
//		}
		//		
		printf("Motor_task runing  !!!!\r\n");
		if(R_IRQ() == 0)
		{
		  printf_show();
		  Motor_show();
		}
		vTaskDelay(100);
	}
}


void printf_show(void)
{
	taskENTER_CRITICAL();
    Reveive(Buf);
	taskEXIT_CRITICAL();
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
	taskENTER_CRITICAL();
    Reveive(Buf);
	taskEXIT_CRITICAL();
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

void printf_Iot_show(void)
{

    if(Motor_Data == Xleft)
    {
        printf("Xleft");
    }
    if(Motor_Data == Xright)
    {
        printf("Xright");
    }
    if(Motor_Data == Ybefore)
    {
        printf("Ybefore");
    }
    if(Motor_Data == Yaffer)
    {
        printf("Yaffer");
    }
    if(Motor_Data == 0)
    {
        printf("nochange");
    }

}

void Motor_Iot_show(void)
{

    if(Motor_Data== Xleft)
    {
        M1_Stop();
        M2_Forward();
    }
    if(Motor_Data == Xright)
    {
        M2_Stop();
        M1_Forward();
    }
    if(Motor_Data == Ybefore)
    {
        M1_Forward();
        M2_Forward();
    }
    if(Motor_Data == Yaffer)
    {
        M1_Stop();
        M2_Stop();
    }
    if(Motor_Data == 0)
    {
        M1_Stop();
        M2_Stop();
    }
}


/* USER CODE END Application */

