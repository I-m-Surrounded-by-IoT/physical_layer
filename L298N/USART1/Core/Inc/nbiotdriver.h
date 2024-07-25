#ifndef _NBIOTDRIVER_H_
#define _NBIOTDRIVER_H_

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
//AT指令数据状态(成功，超时，未收到)定义
typedef enum
{
	SUCCESS_REC = 0,   //成功
	TIME_OUT,					 //超时
	NO_REC             //未收到
}teATStatus;

//入网状态
typedef enum
{
	NB_IDIE = 0,  //NB空闲
	NB_SEND,      //NB发送
	NB_WAIT,		  //NB等待
	NB_ACCESS     //NB入网完成
}teNB_TaskStatus;


typedef enum
{
	
AT = 0,
AT_QSCLK,
AT_CSCON,
AT_CEREG,
AT_CGATT,
AT_CGPADDR,
AT_QMTOPEN ,
AT_QMTCONN,
AT_QMTPUB,
AT_QMTCLOSE,
AT_QICLOSE


}teATCmdNum;

//定义AT指令数据结构
typedef struct
{
	char *ATSendStr; 	    //发送AT指令数据字符串
	char *ATRecStr;  	    //接收响应字符串
	uint16_t TimeOut;   	//超时时间定义
	teATStatus ATStatus;    //数据状态(成功，超时，未收到)
	uint8_t RtyNum;         //重发次数
}tsATCmds;

void NB_Init(void);
void NB_Task(void);

extern uint8_t Send_Data_Flag;
extern QueueHandle_t semphore_handle;
#endif