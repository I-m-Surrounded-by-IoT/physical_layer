/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

#include "OLED.h"
#include "string.h"
#include <stdio.h>

//接收缓存�?
uint8_t Uart1Rx = 0;
uint8_t Uart2Rx = 0;
uint8_t Uart1Rxbuff[256] = {0};		//接收缓冲
uint8_t Uart1_Rx_Cnt = 0;		//接收缓冲计数

tsUsart2type Usart2type;

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(uartHandle->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
    else if(uartHandle->Instance==USART2)
    {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART2 DMA Init */
        /* USART2_RX Init */
        hdma_usart2_rx.Instance = DMA1_Channel6;
        hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_rx.Init.Mode = DMA_NORMAL;
        hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART2_IRQn, 4, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

    if(uartHandle->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
    else if(uartHandle->Instance==USART2)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

        /* USART2 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);

        /* USART2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

void EnableUartIT(void)
{
    //使能uart1串口接收中断

    //串口中断接收，以中断方式接收指定长度数据
    HAL_UART_Receive_IT(&huart1,(uint8_t *)&Uart1Rx,1);

    //使能uart2串口接收中断(�?启空闲中�?)
    __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);

    //清除空闲中断标志(上电会置标志�?)
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);

    //串口中断接收，以中断方式接收指定长度数据(接收数据放在Usart2type.Usart2DMARecBuff)
    HAL_UART_Receive_DMA(&huart2,Usart2type.Usart2DMARecBuff,USART2_DMA_REC_SIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        Uart1Rxbuff[Uart1_Rx_Cnt++] = Uart1Rx;   //接收数据转存

        if((Uart1Rxbuff[Uart1_Rx_Cnt-1] == 0x0A)&&(Uart1Rxbuff[Uart1_Rx_Cnt-2] == 0x0D)) //判断结束�?
        {
            printf("usart1 rec buff is %s\r\n",Uart1Rxbuff);
            while((USART2->SR&0x40) == 0);
            HAL_UART_Transmit(&huart2,Uart1Rxbuff,sizeof(Uart1Rxbuff),0xffff);
            Uart1_Rx_Cnt = 0;
            memset(Uart1Rxbuff,0x00,256);

        }

//		 //棿查是否设置了指定的UART1标志,当接收寄存器不为空，返回1
//		if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE) == SET)
//		{
//		 //判断USAET2发鿁是否完房
//		while((USART2->SR&0x40) == 0);
//		 //将USART1中断接收的数�?,赋忼给USART2,USART的接收和发鿁共用�?个寄存器
//		printf("rec buff is %s\r\n",&Uart1Rx);
//		HAL_UART_Transmit(&huart2,&Uart1Rx,1,1000);
//		OLED_ShowChar(2,1, Uart1Rx);
//		}
//
    }


    //判断是否为USART2
    if(huart->Instance == USART2)
    {
        //判断是否有未处理完的数据
        if(Usart2type.Usart2RecLen > 0)
        {
            //将DMA转运的数据的接收缓存区copy到接收缓存区(接收缓存区有未处理的数据，接收缓存区加前面的长度Usart2type.Usart2RecLen)
            memcpy(&Usart2type.Usart2RecBuff[Usart2type.Usart2RecLen],Usart2type.Usart2DMARecBuff,Usart2type.Usart2DMARecLen);
            //将接收的数据长度进行累加
            Usart2type.Usart2RecLen += Usart2type.Usart2DMARecLen;
        }
        else
        {
            //将DMA转运的数据的接收缓存区copy到接收缓存区(没有未处理的数据)
            memcpy(Usart2type.Usart2RecBuff,Usart2type.Usart2DMARecBuff,Usart2type.Usart2DMARecLen);
            Usart2type.Usart2RecLen = Usart2type.Usart2DMARecLen;
        }
        //清除DMA产生的数�?,进行下一次操�?
        memset(Usart2type.Usart2DMARecBuff,0,Usart2type.Usart2DMARecLen);
        //接收成功的标志位
        Usart2type.Usart2RecFlag = 1;

    }
    HAL_UART_Receive_IT(&huart1,&Uart1Rx,1);
}

/* USER CODE END 1 */
