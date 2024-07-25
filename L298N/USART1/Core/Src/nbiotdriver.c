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

static teATCmdNum ATRecCmdNum; //赋值解析命令，用于解析判断，存的命令序号
static tsTimeType TimeNB;    	 //超时定时
static teNB_TaskStatus NB_TaskStatus; //NB入网状态
static teATCmdNum ATCurrentCmdNum;   //当前发送的指令
static teATCmdNum ATNextCmdNum;      //下一条发送的指令
static uint8_t CurrentRty;    //当前的重发次数
uint8_t Send_Data_Flag = 0;
QueueHandle_t semphore_handle;


#define DEVICE_ID 1

#define MTQQ_CLIENT_ID test

#define MTQQ_PASSWORD test

#define MTQQ_ADDRESS 1.94.163.250

#define MTQQ_PORT 1883



tsATCmds ATCmds[] =
{
    {"AT\r\n","OK",4000,NO_REC,3},
    {"AT+QSCLK=0\r\n","OK",4000,NO_REC,3},
    {"AT+CSCON?\r\n","OK",4000,NO_REC,3},
    {"AT+CEREG?\r\n","OK",4000,NO_REC,3},
    {"AT+CGATT?\r\n","OK",4000,NO_REC,3},
    {"AT+CGPADDR?\r\n","OK",4000,NO_REC,3},
    {"AT+QMTOPEN=0,\"1.94.115.111\",1883\r\n","OK",4000,NO_REC,3},
    {"AT+QMTCONN=0,\"device-2\",\"0\",\"test11\"\r\n","OK",4000,NO_REC,3},
    {"AT+QMTPUB=0,1,2,0,\"device/2/report\", 28","OK",8000,NO_REC,3},
    {"AT+QMTCLOSE=0\r\n","OK",4000,NO_REC,3},


    //AT+QMTPUB=0,1,2,0,\"device/1/report\", 28
//	{"AT+QIOPEN=0,0,\"TCP\",\"1.15.27.206\",12300\r\n","OK",8000,NO_REC,3},
//	{"AT+QISEND=0,7\r\n","OK",2000,NO_REC,3},
//	{"{hello}","OK",2000,NO_REC,3},
    {"AT+QICLOSE=0\r\n","OK",2000,NO_REC,3},


};

uint8_t is_Strlen = 0;
BaseType_t err = 0;

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
    //赋值解析命令，用于解析判断
    ATRecCmdNum = ATCmdNum;

    if(ATCmdNum ==AT_QMTPUB)
    {
		    cJSON *interest = cJSON_CreateObject();
			cJSON_AddNumberToObject(interest,"timestamp",1111111111);
			cjson_geo_point = cJSON_CreateObject();
			cJSON_AddNumberToObject(cjson_geo_point,"lat",222222222);
			cJSON_AddNumberToObject(cjson_geo_point,"lon",333333333); 
			cJSON_AddItemToObject(interest,"geo_point",cjson_geo_point); 
	        cJSON_AddNumberToObject(interest,"TDS",44444444);
            cJSON_AddNumberToObject(interest,"TSW",555555555);
            cJSON_AddNumberToObject(interest,"PH",666666666);
            cJSON_AddNumberToObject(interest,"temperv",777777777); 
			cPrint = cJSON_Print(interest);
			printf("%s\r\n",cPrint);
			memset(NbSendDate,0,100);
			uint8_t cPrint_strlen = strlen((char*)cPrint);
		    printf("cPrint_strlen_size%d\r\n",cPrint_strlen);
	        sprintf(NbSendDate,"AT+QMTPUB=0,1,2,0,\"device/2/report\", %d\r\n",cPrint_strlen);
            HAL_UART_Transmit(&huart2,(uint8_t*)NbSendDate,strlen(NbSendDate),1000);

            HAL_UART_Transmit(&huart2,(uint8_t*)cPrint,cPrint_strlen,1000);

            HAL_UART_Transmit(&huart1,(uint8_t*)cPrint,cPrint_strlen,1000);
//		Send_Data_Flag = 1;
//        semphore_handle = xSemaphoreCreateBinary();
//        if(semphore_handle != NULL)
//        {
//            printf("chuangjian_changgong\r\n");
//        }
//        if(semphore_handle != NULL)
//        {
//            err = xSemaphoreGive(semphore_handle);
//            if(err == pdPASS)
//            {
//                printf("shifang_chengong\r\n");;
//            }
//            else
//            {
//                printf("shifang_shipai\r\n");
//            }
//        }
//        char NbSendDate[256];

//        memset(NbSendDate,0,100);


//        Show_Sersor_Value();
//        cJSON *interest = cJSON_CreateObject();

//        cJSON_AddNumberToObject(interest,"TDS",TDS_value);
//        cJSON_AddNumberToObject(interest,"TSW",TSW_Value);
//        cJSON_AddNumberToObject(interest,"PH",PH_Value);
//        cJSON_AddNumberToObject(interest,"temperv",temper);
//        char *cPrint = cJSON_Print(interest);
//        printf("cJSON_Print：\n%s\n", cPrint);
//        uint8_t cPrint_strlen = strlen((char*)cPrint);
//        sprintf(NbSendDate,"AT+QMTPUB=0,1,2,0,\"device/%d/report\", %d\r\n",DEVICE_ID,cPrint_strlen);
//        HAL_UART_Transmit(&huart2,(uint8_t*)NbSendDate,strlen(NbSendDate),1000);
//        //发送AT指令
//        HAL_UART_Transmit(&huart2,(uint8_t*)cPrint,cPrint_strlen,1000);
//        //打印置电脑
//        HAL_UART_Transmit(&huart1,(uint8_t*)cPrint,cPrint_strlen,1000);
//        free(cPrint);

//        cJSON *interest = cJSON_CreateObject();// 插入元素，对应 键值对
//        cJSON_AddItemToObject(interest, "TDS", cJSON_CreateString((char*)&TDS)); // 当值是字符串时，需要使用函数cJSON_CreateString()创建
//        cJSON_AddItemToObject(interest, "TSW", cJSON_CreateString((char*)&TSW_Value));
//				cJSON_AddItemToObject(interest, "PH", cJSON_CreateString((char*)&PH_Value));
//				cJSON_AddItemToObject(interest, "temper", cJSON_CreateString((char*)&temper));
//        char *cPrint = cJSON_Print(interest);
//        char *cPrintUnformatted = cJSON_PrintUnformatted(interest);
//        printf("cJSON_Print：\n%s\n", cPrint);                       // cJSON_Print：有做格式调整
//				printf("cJSON_PrintUnformatted：\n%s\n", cPrintUnformatted); //

//				sprintf(NbSendDate,"AT+QMTPUB=0,1,2,0,\"device/%d/report\", %d\r\n",DEVICE_ID,strlen((char*)cPrint));
//        // 返回的字符串指针需要自己释放
//        free(cPrint);
//        free(cPrintUnformatted);

//          //发送AT指令
//        HAL_UART_Transmit(&huart2,(uint8_t*)NbSendDate,strlen(NbSendDate),1000);
//				HAL_Delay(50);
//				HAL_UART_Transmit(&huart2,(uint8_t*)interest,strlen((char*)cPrint),1000);
//        //打印置电脑
//        HAL_UART_Transmit(&huart1,(uint8_t*)interest,strlen((char*)cPrint),1000);



//        if (pb_encode(&o_stream, api_collection_GeoPoint_fields, &message))
//        {
//						sprintf(NbSendDate,"AT+QMTPUB=0,1,2,0,\"device/%d/report\", %d\r\n",DEVICE_ID,o_stream.bytes_written);
//						HAL_UART_Transmit(&huart2,(uint8_t*)NbSendDate,strlen(NbSendDate),1000);
//					  HAL_UART_Transmit(&huart2,buffer,o_stream.bytes_written,1000);

//        }

    }
    else
    {
        //发送AT指令
        HAL_UART_Transmit(&huart2,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),1000);
        //打印置电脑
        HAL_UART_Transmit(&huart1,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),1000);
//        HAL_Delay(2000);
    }

    //打开超时定时器
    SetTime(&TimeNB,ATCmds[ATCmdNum].TimeOut);
    //打开发送指示灯
    SetLedRun(LED_TX);
}

void ATRec(void)
{
    //接收是否完成
    if(Usart2type.Usart2RecFlag)
    {
        //比较接收到的字符串与定义的字符串（OK）是否比配
        if(strstr((const char*)Usart2type.Usart2RecBuff,ATCmds[ATRecCmdNum].ATRecStr) != NULL)
        {
            //将AT指令的状态置成功状态
            ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;

        }
        //打开接收指示灯
        SetLedRun(LED_RX);
        //打印OK置电脑
        HAL_UART_Transmit(&huart1,Usart2type.Usart2RecBuff,Usart2type.Usart2RecLen,100);
        Usart2type.Usart2RecFlag = 0;
        Usart2type.Usart2RecLen = 0;
    }
}

//NB发送重启
void NB_Init(void)
{

    NB_TaskStatus = NB_SEND;
    ATCurrentCmdNum = AT;
    ATNextCmdNum = AT_QSCLK;

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
//			if(ATCurrentCmdNum != AT_QMTPUB)
//			{
			   ATRec();
//			}
//			else
//			{
//				if(Send_Data_Flag == 1)
//				{
//					return;
//				}
//			}
            //判断是否接收成功
            if(ATCmds[ATCurrentCmdNum].ATStatus == SUCCESS_REC)
            {

                //最后一个AT指令
                if(ATCurrentCmdNum == AT_QICLOSE)
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


