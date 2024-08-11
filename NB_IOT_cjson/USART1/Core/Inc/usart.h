/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

#define USART2_DMA_REC_SIZE 400 
#define USART2_REC_SIZE 400	
	
	
typedef struct
{
	uint8_t Usart2RecFlag;     	//数据接收标志�?
	uint16_t Usart2DMARecLen;  	//获取DMA接收的长�?
	uint16_t Usart2RecLen;      //解析缓存区的长度
	uint8_t Usart2DMARecBuff[USART2_DMA_REC_SIZE];  //DMA转运UASRT数据缓存�?
	uint8_t Usart2RecBuff[USART2_REC_SIZE];  //解析缓存�?
}tsUsart2type;

//AT指令接收数据结构
extern tsUsart2type Usart2type;
	
	
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

extern void EnableUartIT(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

