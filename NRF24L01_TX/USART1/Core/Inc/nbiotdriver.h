#ifndef _NBIOTDRIVER_H_
#define _NBIOTDRIVER_H_

#include "stm32f1xx_hal.h"

//AT指令数据状态(成功，超时，未收到)定义
typedef enum
{
	SUCCESS_REC = 0,   //成功
	TIME_OUT,					 //超时
	NO_REC             //未收到
}teATStatus;

typedef enum
{
	AT_CFUN0 = 0,
	AT_CGSN,
	AT_NRB,
	AT_NCDP,
	AT_CFUN1,
	AT_CIMI,
	AT_CMEE,
	AT_CGDCONT,
	AT_NNMI,
	AT_CGATT,
	AT_CGPADDR,
	//AT_NMGS,
	//AT_IDIE

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


#endif