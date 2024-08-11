#include "main.h"
#include "nbiotdriver.h"
#include <string.h>
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "time.h"
#include "led.h"
#include "stdio.h"
#include "pb.h"
#include "device_message.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "sensor.h"
#include "cjson.h"
#include <stdlib.h> 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "driver_timer.h"
#include "unix.h"

static teATCmdNum ATRecCmdNum; //赋值解析命令，用于解析判断，存的命令序号
static tsTimeType TimeNB;    	 //超时定时
static teNB_TaskStatus NB_TaskStatus; //NB入网状态
static teATCmdNum ATCurrentCmdNum;   //当前发送的指令
static teATCmdNum ATNextCmdNum;      //下一条发送的指令
static uint8_t CurrentRty;    //当前的重发次数
uint8_t Send_Data_Flag = 0;

uint8_t taskSuspend = 0;
uint64_t unix_data;
DateTime  dateTime;
String_DateTime string_DateTime;

void conversion_string(void);
void Get_time_value(char *time);

#define DEVICE_ID 1

#define MTQQ_CLIENT_ID test

#define MTQQ_PASSWORD test

#define MTQQ_ADDRESS 1.94.163.250

#define MTQQ_PORT 1883

extern uint64_t unix_data;

tsATCmds ATCmds[] =
{
    {"AT\r\n","OK",4000,NO_REC,3},
    {"AT+QSCLK=0\r\n","OK",4000,NO_REC,3},
	{"AT+QRST=1\r\n","OK",4000,NO_REC,3},
    {"AT+CSCON?\r\n","OK",4000,NO_REC,3},
    {"AT+CEREG?\r\n","OK",4000,NO_REC,3},
    {"AT+CGATT?\r\n","OK",4000,NO_REC,3},
    {"AT+CGPADDR?\r\n","OK",4000,NO_REC,3},
    {"AT+QMTOPEN=0,\"1.94.115.111\",1883\r\n","OK",4000,NO_REC,3},
    {"AT+QMTCONN=0,\"device-2\",\"0\",\"test11\"\r\n","OK",4000,NO_REC,3},
	{"AT+QMTSUB=0,1,\"device/2/sleep\",2\r\n","OK",4000,NO_REC,3},
	{"AT+CCLK?\r\n","OK",4000,NO_REC,3},
    {"AT+QMTPUB","OK",6000,NO_REC,3},
    {"AT+QMTCLOSE=0\r\n","OK",4000,NO_REC,3},

};

uint8_t is_Strlen = 0;
BaseType_t err = 0;
uint8_t send_count = 0;
extern uint8_t sleep_mode_flag;


void ATsend(teATCmdNum ATCmdNum)
{
    cJSON *interest;
	cJSON *cjson_geo_point;
	char *cPrint;
	char NbSendDate[256];
    //清空接收缓存区
    memset(Usart2type.Usart2RecBuff,0,USART2_REC_SIZE);
    //发送数据状态为未收到
    ATCmds[ATCmdNum].ATStatus = NO_REC;
    ATRecCmdNum = ATCmdNum;
//	if(ATCmdNum >= AT_QMTOPEN)
//	{
	  vTaskDelay(4000);
//	}
    if(ATCmdNum ==AT_QMTPUB)
    {
		    cJSON *interest = cJSON_CreateObject();
			cJSON_AddNumberToObject(interest,"timestamp",unix_data);
//			cjson_geo_point = cJSON_CreateObject();
//			cJSON_AddNumberToObject(cjson_geo_point,"lat",222222222);
//			cJSON_AddNumberToObject(cjson_geo_point,"lon",333333333); 
//			cJSON_AddItemToObject(interest,"geo_point",cjson_geo_point); 
	        cJSON_AddNumberToObject(interest,"TDS",TDS_value);
            cJSON_AddNumberToObject(interest,"TSW",TSW_Value);
            cJSON_AddNumberToObject(interest,"PH",PH_Value);
            cJSON_AddNumberToObject(interest,"temperv",temper); 
			cPrint = cJSON_Print(interest);
			memset(NbSendDate,0,256);
			uint8_t cPrint_strlen = strlen((char*)cPrint);
	        sprintf(NbSendDate,"AT+QMTPUB=0,1,2,0,\"device/2/report\", %d,%s\r\n",cPrint_strlen,cPrint);
			cJSON_free(cPrint); 
			cJSON_Delete(interest);
		    taskENTER_CRITICAL();
			
     		HAL_UART_Transmit(&huart2,(uint8_t*)NbSendDate,strlen(NbSendDate),1000);
			HAL_UART_Transmit(&huart1,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),1000);		
            taskEXIT_CRITICAL();
    }
    else
    {
		taskENTER_CRITICAL();
        //发送AT指令
        HAL_UART_Transmit(&huart2,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),1000);
        //打印置电脑
        HAL_UART_Transmit(&huart1,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),1000);
		taskEXIT_CRITICAL();
    }

    //打开超时定时器
    SetTime(&TimeNB,ATCmds[ATCmdNum].TimeOut);
//    //打开发送指示灯
//    SetLedRun(LED_TX);
}



void ATRec(void)
{
	char*time_AT_cmd = "CCLK";
    //接收是否完成
    if(Usart2type.Usart2RecFlag)
    {
	
        //比较接收到的字符串与定义的字符串（OK）是否比配
        if(strstr((const char*)Usart2type.Usart2RecBuff,ATCmds[ATRecCmdNum].ATRecStr) != NULL)
        {
            //将AT指令的状态置成功状态
            ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;
			if(strstr((const char*)Usart2type.Usart2RecBuff,time_AT_cmd) != NULL)
			{   
				Get_time_value((char*)Usart2type.Usart2RecBuff);
				conversion_string();
				unix_data = dateTimeToTimestamp(dateTime)*1000;
			}

        }
        //打开接收指示灯
//        SetLedRun(LED_RX);
        //打印OK置电脑
		taskENTER_CRITICAL();
        HAL_UART_Transmit(&huart1,Usart2type.Usart2RecBuff,Usart2type.Usart2RecLen,100);
        Usart2type.Usart2RecFlag = 0;
        Usart2type.Usart2RecLen = 0;
		taskEXIT_CRITICAL();
    }
}

//NB发送重启
void NB_Init(void)
{

    NB_TaskStatus = NB_SEND;
    ATCurrentCmdNum = AT;
    ATNextCmdNum = AT_QSCLK;

}

void NB_Send_Data(void)
{

    NB_TaskStatus = NB_SEND;
    ATCurrentCmdNum = AT_CCLK;
    ATNextCmdNum = AT_QMTPUB;

}

//入网是否成功

void NB_Task(void)
{
    while(1)
    {
        switch(NB_TaskStatus)
        {
        case NB_IDIE:

            break;
        case NB_SEND:
            //判断是否是当前指令,是当前指令，则读取重复次数3
            if(ATCurrentCmdNum != ATNextCmdNum)
            {
                CurrentRty = ATCmds[ATCurrentCmdNum].RtyNum;
            }
            //发送AT指令
            ATsend(ATCurrentCmdNum);
            //置等待状态，等待状态(用于接收响应)
            if(is_Strlen == 0)
            {
                NB_TaskStatus = NB_WAIT;
            }
            return;
        case NB_WAIT:
            //接收响应
			 ATRec();
            //判断是否接收成功
            if(ATCmds[ATCurrentCmdNum].ATStatus == SUCCESS_REC)
            {
                if(ATCurrentCmdNum == AT_QMTPUB)
				{
					ATNextCmdNum = AT_QMTPUB;
                    NB_TaskStatus = NB_SEND;
					send_count++;
					if(send_count == 3)
					{
					   send_count = 0;
 					   taskSuspend = 1;
					   return;
					}
				
					break;
				}
                //最后一个AT指令
                else if(ATCurrentCmdNum == AT_QICLOSE)
                {
                    NB_Init();
                    return;
                }
                else
                {
                    ATCurrentCmdNum += 1;
                    ATNextCmdNum = ATCurrentCmdNum+1;
                    NB_TaskStatus = NB_SEND;
                    break;
                }
            }
            //没接收成功，判断是否超时
            else if (CompareTime(&TimeNB))
            {

                ATCmds[ATCurrentCmdNum].ATStatus = TIME_OUT;
                //超时次数
                if(CurrentRty > 0)
                {
                    CurrentRty--;
                    //重新发送当前指令
                    ATNextCmdNum = ATCurrentCmdNum;
                    NB_TaskStatus = NB_SEND;
                    break;
                }
                else
                {
                    NB_Init();
                    return;
                }
            }
			vTaskDelay(1000);
            return;
        case NB_ACCESS:
//            LedOn(LED_NET);
//            ATCurrentCmdNum = AT_QMTPUB;
//            ATNextCmdNum = AT_QMTCLOSE;
//            NB_TaskStatus = NB_SEND;

            break;
        default:
            return;
        }
    }

}


void Get_time_value(char *time) 
{
    char *subString = time;
    char *subStringNext;

    // 提取年月日
    subString = strstr(time, "\"") + 1; // 跳过 "+CCLK: " 部分
    subStringNext = strstr(subString, "/");
    if (subStringNext != NULL) {
        memcpy(string_DateTime.year, subString, subStringNext - subString);
        subString = subStringNext + 1;
        subStringNext = strstr(subString, "/");
        if (subStringNext != NULL) {
            memcpy(string_DateTime.month, subString, subStringNext - subString);
            subString = subStringNext + 1;
            subStringNext = strstr(subString, ","); // 找到 "," 分隔符
            if (subStringNext != NULL) {
                memcpy(string_DateTime.day, subString, subStringNext - subString);
                subString = subStringNext + 1; 
            } else {
                // 处理错误情况
                return;
            }
        } else {
            // 处理错误情况
            return;
        }
    } else {
        // 处理错误情况
        return;
    }

    // 提取时分，逻辑与上面类似
      for (int i = 0; i < 2; i++) {
        subStringNext = strstr(subString, ":");
        if (subStringNext != NULL) {
            switch (i) {
                case 0:
                    memcpy(string_DateTime.hour, subString, subStringNext - subString);
                    break;
                case 1:
                    memcpy(string_DateTime.minute, subString, subStringNext - subString);
                    break;
                // case 2 不再需要 break，循环到最后一次时，subStringNext 已经指向秒的末尾
            }
            subString = subStringNext + 1; 
        } else {
            // 处理错误情况
            return;
        }
    }
	// 提取秒
	subStringNext = strstr(subString, "+"); // 尝试寻找 "+" 作为时区信息开始的标志
    if (subStringNext != NULL) {
        // 如果找到了 "+" 号，说明有时区信息，需要向前退2位来获取秒数
        memcpy(string_DateTime.second, subString, subStringNext - subString); 
    } else {
        // 如果没有找到 "+" 号，说明没有时区信息，直接复制剩余部分作为秒数
        strcpy(string_DateTime.second, subString);
    }
	
} 

void conversion_string(void)
{
	sscanf(string_DateTime.year, "%hu",&dateTime.year);
	sscanf(string_DateTime.month, "%hu",&dateTime.month);
	sscanf(string_DateTime.day, "%hu",&dateTime.day);
	sscanf(string_DateTime.hour, "%hu",&dateTime.hour);
	sscanf(string_DateTime.minute, "%hu",&dateTime.minute);
	sscanf(string_DateTime.second, "%hu",&dateTime.second);
}
